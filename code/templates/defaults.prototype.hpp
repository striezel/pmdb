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

#ifndef PMDB_TEMPLATES_DEFAULTS_HPP
#define PMDB_TEMPLATES_DEFAULTS_HPP

#include <string_view>

namespace pmdb::tpl::defaults
{

using namespace std::string_view_literals;

constexpr std::string_view folder = R"(@FOLDER_TPL@)"sv;

constexpr std::string_view folder_entry = R"(@FOLDER_ENTRY_TPL@)"sv;

constexpr std::string_view folder_list = R"(@FOLDER_LIST_TPL@)"sv;

constexpr std::string_view index_entry = R"(@INDEX_ENTRY_TPL@)"sv;

constexpr std::string_view message = R"(@MESSAGE_TPL@)"sv;

} // namespace

#endif // PMDB_TEMPLATES_DEFAULTS_HPP
