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

#pragma once
#include <stdexcept>

/*
 * @brief Base class for custom exceptions
 */
class UtilException : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

/*
 * @brief Exception class for symbolizing error in opening a file
 */
class FileError : public UtilException {
public:
	using UtilException::UtilException;
};

/*
 * @brief Exception class for symbolizing error in the key
 */
class KeyError : public UtilException {
public:
	using UtilException::UtilException;
};