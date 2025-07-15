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

#ifndef PMDB_FUNCTIONS_HPP
#define PMDB_FUNCTIONS_HPP

#include <vector>
#include "filters/FilterUser.hpp"
#include "FolderMap.hpp"
#include "MessageDatabase.hpp"

/** \brief Prints title, date and folder of all messages that match at least
 *         one of the given filters to standard output.
 *
 * \param mdb      the database containing the messages
 * \param fm       folder mappings for the message database
 * \param filters  the filters to match against
 */
void showFilteredMessages(const MessageDatabase& mdb, const FolderMap& fm, const std::vector<FilterUser>& filters);

/** \brief Saves messages to the default save directory.
 *
 * \param mdb          the database containing the messages
 * \param fm           folder mappings for the message database
 * \param compression  if set to Compression::zlib, the saved files will be
 *                     compressed with zlib
 * \param check        whether or not to perform a safety check to avoid mixing
 *                     compressed and uncompressed messages
 * \return Returns zero, if all messages could be saved.
 *         Returns non-zero exit code, if an error occurred.
 */
int saveMessages(const MessageDatabase& mdb, const FolderMap& fm, const Compression compression, const CompressionCheck check);

#endif // PMDB_FUNCTIONS_HPP
