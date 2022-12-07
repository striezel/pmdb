/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2015, 2022  Dirk Stolle

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

#include "../locate_catch.hpp"
#include "../../code/Config.hpp"

TEST_CASE("Config")
{
  SECTION("initial state")
  {
    Config conf;
    REQUIRE( conf.getForumURL().empty() );
    REQUIRE( conf.getTPL().empty() );
    REQUIRE( conf.getSmilies().empty() );
  }

  SECTION("set / get template")
  {
    Config conf;

    conf.setTPLFile("/some/nice/path/to/file.tpl");
    // tpl file name should match value above
    REQUIRE( conf.getTPL() == "/some/nice/path/to/file.tpl" );
  }

  SECTION("set / get URL")
  {
    Config conf;

    conf.setForumURL("http://www.example.com/");
    // forum URL should match value above
    REQUIRE( conf.getForumURL() == "http://www.example.com/" );
  }
}
