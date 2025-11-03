/*
* Copyright (C) 2025 Omega493

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <format>
#include <fstream>
#include <vector>
#include <string>

#include "utilities/get_secret_input.h"
#include "utilities/exception.h"

#include <sodium/crypto_secretstream_xchacha20poly1305.h>
#include <sodium/utils.h>

void decrypt(const std::string& input_path, const std::string& output_path) {
    // Read from the input encrypted file
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file.is_open()) throw FileError("Error: Couldn't open input file `" + input_path + '`');

    // Check the validity of the output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) throw FileError("Error: Couldn't open output file `" + output_path + '`');

    // Ask the user for the secret key
    std::cout << "Enter the secret key (hex): ";
    std::string key_hex = get_secret_input();

    if (key_hex.empty()) throw KeyError("No key was entered");

    unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    // Convert the hex key from the user's input into raw bytes
    if (sodium_hex2bin(key, sizeof(key), key_hex.c_str(), key_hex.length(), NULL, NULL, NULL) != 0) {
        throw KeyError("Invalid hex key provided");
    }

    // Read the 24-byte header from the start of the file
    unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    input_file.read(reinterpret_cast<char*>(header), sizeof(header));

    crypto_secretstream_xchacha20poly1305_state crypto_state;

    // Initialize decryption stream with the header and key
    if (crypto_secretstream_xchacha20poly1305_init_pull(&crypto_state, header, key) != 0) {
        throw KeyError("Invalid header or key");
    }

    constexpr size_t CHUNK_SIZE{ 4096 };

    // The input chunk buffer needs space for the plaintext plus the 17-byte tag (16-byte authentication tag + 1-byte control tag)
    std::vector<unsigned char> ciphertext_chunk(CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES);
    std::vector<unsigned char> decrypted_chunk(CHUNK_SIZE);
    unsigned long long decrypted_len;
    unsigned char tag;

    // Process the input file in chunks until the final tag is found
    do {
        input_file.read(reinterpret_cast<char*>(ciphertext_chunk.data()), ciphertext_chunk.size());
        size_t bytes_read = input_file.gcount();

        if (bytes_read == 0) break; // Reached EOF

        if (crypto_secretstream_xchacha20poly1305_pull(
            &crypto_state,
            decrypted_chunk.data(),
            &decrypted_len,
            &tag,
            ciphertext_chunk.data(),
            bytes_read,
            NULL, 0) != 0) {
            throw UtilException("Decryption failed. The input file maybe corrupt");
        }

        // Write decrypted plaintext chunk to the output file
        output_file.write(reinterpret_cast<const char*>(decrypted_chunk.data()), decrypted_len);

        // Check the tag to see if it was the last tag
    } while (tag != crypto_secretstream_xchacha20poly1305_TAG_FINAL);

    input_file.close();
    output_file.close();

    std::cout << std::format("Successfully decrypted `{}` to `{}`", input_path, output_path) << std::endl;
    return;
}