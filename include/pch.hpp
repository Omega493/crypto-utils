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

#ifndef PCH_HPP
#define PCH_HPP

#pragma once

#include <iostream>
#include <format>
#include <fstream>
#include <vector>
#include <string>

#include <cstdint>

#include <utilities/get_secret_input.hpp>
#include <utilities/exception.hpp>

#include <src/encrypt.hpp>
#include <src/decrypt.hpp>

#include <sodium/core.h>
#include <sodium/crypto_secretstream_xchacha20poly1305.h>
#include <sodium/utils.h>
#include <include/cxxopts.hpp>

#endif // PCH_HPP