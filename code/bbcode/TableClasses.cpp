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

#include "TableClasses.hpp"

// constants for default class names used in constructor
const std::string TableClasses::DefaultTableClass = "grid_table";
const std::string TableClasses::DefaultRowClass   = "grid_tr";
const std::string TableClasses::DefaultCellClass  = "grid_td";

TableClasses::TableClasses(const bool useGridClasses,
                           const std::string& tableClass,
                           const std::string& rowClass,
                           const std::string& cellClass)
: useClasses(useGridClasses), table(tableClass), row(rowClass), cell(cellClass)
{
}
