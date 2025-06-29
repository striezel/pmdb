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

#include "../../locate_catch.hpp"
#include "../../../code/templates/defaults.hpp"

TEST_CASE("pmdb::tpl::defaults")
{
  SECTION("folder template is set")
  {
    const auto tpl = pmdb::tpl::defaults::folder;
    REQUIRE_FALSE( tpl.empty() );
    REQUIRE( tpl.find("@") != 0 );
  }

  SECTION("folder entry template is set")
  {
    const auto tpl = pmdb::tpl::defaults::folder_entry;
    REQUIRE_FALSE( tpl.empty() );
    REQUIRE( tpl.find("@") != 0 );
  }

  SECTION("folder list template is set")
  {
    const auto tpl = pmdb::tpl::defaults::folder_list;
    REQUIRE_FALSE( tpl.empty() );
    REQUIRE( tpl.find("@") != 0 );
  }

  SECTION("index entry template is set")
  {
    const auto tpl = pmdb::tpl::defaults::index_entry;
    REQUIRE_FALSE( tpl.empty() );
    REQUIRE( tpl.find("@") != 0 );
  }

  SECTION("message template is set")
  {
    const auto tpl = pmdb::tpl::defaults::message;
    REQUIRE_FALSE( tpl.empty() );
    REQUIRE( tpl.find("@") != 0 );
  }
}
