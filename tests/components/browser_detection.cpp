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
#include "../../code/browser_detection.hpp"

TEST_CASE("browser detection")
{
  SECTION("detect_browser")
  {
    const auto info = detect_browser();
    #if _WIN32
    // TODO: Change this as soon as detect_browser() works on Windows, too.
    REQUIRE_FALSE( info.has_value() );
    #else
    REQUIRE( info.has_value() );
    REQUIRE_FALSE( info.value().path.empty() );
    #endif
  }

  SECTION("additional parameters")
  {
    SECTION("Chromium")
    {
      const auto params = additional_parameters(Browser::Chromium);
      REQUIRE( params.size() == 1 );
      REQUIRE( params[0] == "--incognito" );
    }

    SECTION("Firefox")
    {
      const auto params = additional_parameters(Browser::Firefox);
      REQUIRE( params.size() == 1 );
      REQUIRE( params[0] == "--private-window" );
    }
  }
}
