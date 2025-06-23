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

#include "../locate_catch.hpp"
#include "../../code/paths.hpp"

TEST_CASE("pmdb::paths")
{
  SECTION("main()")
  {
    const std::string main = pmdb::paths::main();
    REQUIRE_FALSE( main.empty() );

    REQUIRE( main.find(".pmdb") != std::string::npos );
  }

  SECTION("html")
  {
    const std::string main = pmdb::paths::main();
    const std::string html = pmdb::paths::html();

    REQUIRE_FALSE( html.empty() );
    REQUIRE( html.find("html") != std::string::npos );

    // Must be below main path.
    REQUIRE( html.find(main) == 0 );
  }

  SECTION("conf")
  {
    const std::string main = pmdb::paths::main();
    const std::string conf = pmdb::paths::conf();

    REQUIRE_FALSE( conf.empty() );
    REQUIRE( conf.find("pmdb.conf") != std::string::npos );

    // Must be below main path.
    REQUIRE( conf.find(main) == 0 );
  }

  SECTION("colourmap")
  {
    const std::string main = pmdb::paths::main();
    const std::string colourmap = pmdb::paths::colourmap();

    REQUIRE_FALSE( colourmap.empty() );
    REQUIRE( colourmap.find("pmdb.colourmap") != std::string::npos );

    // Must be below main path.
    REQUIRE( colourmap.find(main) == 0 );
  }
}
