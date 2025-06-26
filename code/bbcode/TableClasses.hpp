/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2025  Dirk Stolle

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

#ifndef BBCODE_TABLECLASSES_HPP
#define BBCODE_TABLECLASSES_HPP

#include <string>

/// Holds information about the CSS classes used in a table BB code.
struct TableClasses
{
  public:
    // constants for default class names used in constructor
    static const std::string DefaultTableClass;
    static const std::string DefaultRowClass;
    static const std::string DefaultCellClass;

    /** \brief Creates a new instance.
     *
     * \param useGridClasses   if set to true, the generated HTML code will use
     *                         classes (as in CSS class) instead of a somewhat
     *                         bloated style attribute to generate the grids around
     *                         the table elements, if such the grids are required
     * \param tableClass       name of the class for grids on &lt;table&gt; tags
     * \param rowClass         name of the class for grids on &lt;tr&gt; tags
     * \param cellClass        name of the class for grids on &lt;td&gt; tags
     * \remarks
     * If useGridClasses is set to false, the next three parameters do not
     * have any effect on the generated HTML code.
     */
    TableClasses(const bool useGridClasses = false,
                 const std::string& tableClass = DefaultTableClass,
                 const std::string& rowClass = DefaultRowClass,
                 const std::string& cellClass = DefaultCellClass);

    bool useClasses;   /**< whether or not to use the three classes below */
    std::string table; /**< class for use in table tags */
    std::string row;   /**< class for use in tr tags */
    std::string cell;  /**< class for use in td tags */
};

#endif // BBCODE_TABLECLASSES_HPP
