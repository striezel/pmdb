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

#ifndef PMDB_OPEN_FILE_HPP
#define PMDB_OPEN_FILE_HPP

#include "FolderMap.hpp"

/** \brief Attempts to open the first folder index HTML file in a browser.
 *
 * \param fm         folder mappings for the message database
 * \param html_dir   directory where the HTML files reside
 */
void openFirstIndexFile(const FolderMap& fm, const std::string& html_dir);

#endif // PMDB_OPEN_FILE_HPP
