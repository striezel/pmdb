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

#ifndef PMDB_PATHS_HPP
#define PMDB_PATHS_HPP

#include <string>

namespace pmdb
{

struct paths
{
  public:
    /** \brief Gets the path to pmdb's main directory.
     *
     * \remarks This is usually ~/.pmdb or the equivalent.
     * \return Returns the path to pmdb's main directory.
     */
    static std::string main();

    /** \brief Gets the directory where HTML files are saved.
     *
     * \return Returns the directory where HTML files are saved.
     * \remarks This is usually ~/.pmdb/html or the equivalent.
     */
    static std::string html();


    /** \brief Gets the default path for the configuration file.
     *
     * \return Returns the default path for the configuration file.
     * \remarks This is usually ~/.pmdb/pmdb.conf or the equivalent.
     */
    static std::string conf();

    /** \brief Gets the path for the colour map.
     *
     * \return Returns the path for the colour map.
     * \remarks This is usually ~/.pmdb/pmdb.colourmap or the equivalent.
     */
    static std::string colourmap();
};

} // namespace

#endif // PMDB_PATHS_HPP
