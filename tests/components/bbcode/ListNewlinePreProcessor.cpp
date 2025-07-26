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

TEST_CASE("ListNewlinePreProcessor")
{
  ListNewlinePreProcessor preProc;

  SECTION("empty text")
  {
    std::string text = "";
    preProc.applyToText(text);
    // Text is still empty text.
    REQUIRE( text == "" );
  }

  SECTION("text without list bb code")
  {
    const std::string original = "This is [b]bold[/b],\nis it?";
    std::string text = original;
    preProc.applyToText(text);
    // Text is still the same.
    REQUIRE( text == original );
  }

  SECTION("text with list bb code")
  {
    std::string text = "[list]\n[*]item one\n[*]second item\n[/list]";
    preProc.applyToText(text);
    // New line characters within list are gone.
    REQUIRE( text == "[list][*]item one[*]second item\n[/list]" );
  }

  SECTION("text with list bb code and outer newline characters")
  {
    std::string text = "hey.\n[list]\n[*]item one\n[*]second item\n[/list]\nBlah.\nBlah";
    preProc.applyToText(text);
    // New line characters within list are gone, but not outside.
    REQUIRE( text == "hey.\n[list][*]item one[*]second item\n[/list]\nBlah.\nBlah" );
  }
}
