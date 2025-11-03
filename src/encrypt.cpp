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

void encrypt(const std::string& input_path, const std::string& output_path) {
    // Read from the input plaintext file
    std::ifstream input_file(input_path, std::ios::binary);
    if (!input_file.is_open()) {
        throw FileError("Error: Couldn't open input file `" + input_path + '`');
    }

    // Check the validity of the output file
    std::ofstream output_file(output_path, std::ios::binary);
    if (!output_file.is_open()) {
        throw FileError("Error: Couldn't open output file `" + output_path + '`');
    }

    // Ask the user for the secret key
    std::cout << "Enter the secret key (hex): ";
    std::string key_hex = get_secret_input();

    if (key_hex.empty()) throw KeyError("No key was entered");

    unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    // Convert the hex key from the user's input into raw bytes
    if (sodium_hex2bin(key, sizeof(key), key_hex.c_str(), key_hex.length(), NULL, NULL, NULL) != 0) {
        throw KeyError("Invalid hex key provided");
    }

    constexpr size_t CHUNK_SIZE{ 4096 };

    unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state crypto_state;

    // Initialize the stream and get the header
    crypto_secretstream_xchacha20poly1305_init_push(&crypto_state, header, key);

    // Write the header to the start of the output file
    output_file.write(reinterpret_cast<const char*>(header), sizeof(header));

    std::vector<unsigned char> plaintext_chunk(CHUNK_SIZE);

    // The ciphertext needs space for the plaintext plus an authentication tag
    std::vector<unsigned char> ciphertext_chunk(CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES);
    unsigned long long out_len;
    unsigned char tag;

    // Process the file in chunks
    do {
        input_file.read(reinterpret_cast<char*>(plaintext_chunk.data()), CHUNK_SIZE);
        size_t bytes_read = input_file.gcount();

        tag = input_file.eof() ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : crypto_secretstream_xchacha20poly1305_TAG_MESSAGE;

        crypto_secretstream_xchacha20poly1305_push(
            &crypto_state,
            ciphertext_chunk.data(),
            &out_len,
            plaintext_chunk.data(),
            bytes_read,
            NULL, 0, tag
        );

        // Write the encrypted chunk to the output file
        output_file.write(reinterpret_cast<const char*>(ciphertext_chunk.data()), out_len);
    } while (!input_file.eof());

    input_file.close();
    output_file.close();

    std::cout << std::format("Successfully encrypted `{}` to `{}`", input_path, output_path) << std::endl;
    return;
}