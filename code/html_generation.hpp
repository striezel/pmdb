/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2016, 2025  Dirk Stolle

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

#ifndef PMDB_HTML_GENERATION_HPP
#define PMDB_HTML_GENERATION_HPP

#include "FolderMap.hpp"
#include "HTMLOptions.hpp"
#include "MessageDatabase.hpp"

/** \brief Generates HTML files for the private messages.
 *
 * \param mdb          the database containing the messages
 * \param fm           folder mappings for the message database
 * \param htmlOptions  the options for HTML file generation
 * \return Returns zero, if all HTML files could be created.
 *         Returns non-zero exit code, if an error occurred.
 */
int generateHtmlFiles(const MessageDatabase& mdb, const FolderMap& fm, const HTMLOptions htmlOptions);

/** \brief Attempts to open the first folder index HTML file in a browser.
 *
 * \param fm         folder mappings for the message database
 * \param html_dir   directory where the HTML files reside
 */
void openFirstIndexFile(const FolderMap& fm, const std::string& html_dir);

#endif // PMDB_HTML_GENERATION_HPP
