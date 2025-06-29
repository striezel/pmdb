/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
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

#ifndef PMDB_TPL_FUNCTIONS_HPP
#define PMDB_TPL_FUNCTIONS_HPP

namespace pmdb::tpl
{

/** \brief Ensures that all required template files for HTML code generation
 *         exist by creating missing files with default template content.
 *
 * \return Returns zero in case of success.
 *         Returns a non-zero exit code, if an error occurred.
 */
int ensureFilesExist();

} // namespace

#endif // PMDB_TPL_FUNCTIONS_HPP
