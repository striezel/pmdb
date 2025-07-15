/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2025  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef PMDB_COMPRESSION_DETECTION_HPP
#define PMDB_COMPRESSION_DETECTION_HPP

#include <optional>
#include <string>
#include "Compression.hpp"


/** \brief Attempts to detect the compression used for PMs in a folder.
 *
 * \param directory   the directory that contains the private messages
 * \return Returns the compression mode (none or zlib) in case of success.
 *         Returns an empty optional, if an error occurred.
 */
std::optional<Compression> detect_compression(const std::string& directory);

#endif // PMDB_COMPRESSION_DETECTION_HPP
