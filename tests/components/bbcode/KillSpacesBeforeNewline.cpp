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

#include <vector>
#include "../../locate_catch.hpp"
#include "../../../code/bbcode/TextProcessor.hpp"

TEST_CASE("KillSpacesBeforeNewline")
{
  KillSpacesBeforeNewline preProc;

  SECTION("empty text")
  {
    std::string text = "";
    preProc.applyToText(text);
    // Text is still empty text.
    REQUIRE( text == "" );
  }

  SECTION("text without new lines")
  {
    const std::string original = "This is [b]bold[/b], is it?";
    std::string text = original;
    preProc.applyToText(text);
    // Text is still the same.
    REQUIRE( text == original );
  }

  SECTION("text with space before new line")
  {
    std::string text = "line 1\nline 2 \nline 3 \nline 4";
    preProc.applyToText(text);
    // Redundant spaces are gone.
    REQUIRE( text == "line 1\nline 2\nline 3\nline 4" );
  }

  SECTION("text with multiple spaces before new line")
  {
    std::string text = "line 1\nline 2  \nline 3   \nline 4";
    preProc.applyToText(text);
    // Redundant spaces are gone.
    REQUIRE( text == "line 1\nline 2\nline 3\nline 4" );
  }
}
