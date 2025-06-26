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

#ifndef PMDB_HTMLOPTIONS_HPP
#define PMDB_HTMLOPTIONS_HPP

#include <string>
#include "bbcode/HTMLStandard.hpp"

struct HTMLOptions
{
  public:
    /// The HTML standard version to use when creating HTML files.
    HTMLStandard standard{HTMLStandard::HTML4_01};

    /// Whether to convert new line characters to line breaks in (X)HTML.
    bool nl2br{true};

    /// Whether NOT o parse [LIST] codes.
    bool noList{false};

    /// Whether table classes (table, row, cell) should be used.
    bool useTableClasses{false};

    /// class for grids in <table>
    std::string classTable;

    /// class for grids in <tr>
    std::string classRow;

    /// class for grids in <td>
    std::string classCell;
};

#endif // PMDB_HTMLOPTIONS_HPP
