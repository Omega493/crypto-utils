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
#include <string>
#include <exception>

#include "utilities/exception.h"
#include "src/encrypt.hpp"
#include "src/decrypt.hpp"

#include "include/cxxopts.hpp"
#include <sodium/core.h>

int main(int argc, char* argv[]) {
    cxxopts::Options options("encryptor", "Encrypts or decrypts files");

    std::string input_file = "";
    std::string output_file = "";
    try {
        options.add_options()
            ("e,encrypt", "File to encrypt", cxxopts::value<std::string>())
            ("d,decrypt", "File to decrypt", cxxopts::value<std::string>())
            ("o,output", "Output file (optional)", cxxopts::value<std::string>())
            ("h,help", "Print usage");

        auto result = options.parse(argc, argv);

        if (result.count("h") || argc == 1) {
            std::cout << options.help();
            return 0;
        }

        if (result.count("e") && result.count("d")) {
            std::cerr << "Error: Cannot use --encrypt (-e) and --decrypt (-d) simultaneously\n" << std::endl;
            std::cout << options.help();
            return 1;
        }

        if (result.count("e")) {
            input_file = result["e"].as<std::string>();
        }
        else if (result.count("d")) {
            input_file = result["d"].as<std::string>();
        }
        else {
            std::cerr << "Error: You must specify a mode: --encrypt (-e) or --decrypt (-d)\n" << std::endl;
            std::cout << options.help();
            return 1;
        }

        if (sodium_init() < 0) {
            std::cerr << "Error: Couldn't initialize libsodium" << std::endl;
            return 1;
        }

        if (result.count("o")) {
            output_file = result["o"].as<std::string>();
        }
        else {
            size_t last_dot_pos = input_file.find_last_of('.');

            std::string base_name = "";

            if (last_dot_pos == std::string::npos || last_dot_pos == 0) {
                // Treat the whole name as the base
                base_name = input_file;
            }
            else {
                // Get the substring from the start up to the last dot
                base_name = input_file.substr(0, last_dot_pos);
            }

            if (result.count("e")) output_file = base_name + ".enc";
            else output_file = base_name + ".dec";
        }

        if (result.count("e")) encrypt(input_file, output_file);
        else if (result.count("d")) decrypt(input_file, output_file);

        return 0;
    }
    catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        std::cout << options.help();
        return 1;
    }
    catch (const UtilException& e) {
        std::cerr << "Exception thrown: " << e.what() << "\nThe program will terminate" << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << "\nThe program will now terminate" << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception thrown\nThe program will now terminate" << std::endl;
        return 1;
    }
}