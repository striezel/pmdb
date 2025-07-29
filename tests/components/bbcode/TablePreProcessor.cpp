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

TEST_CASE("TablePreProcessor")
{
  const TablePreProcessor preProc("tr", "td");

  SECTION("empty text")
  {
    std::string text = "";
    preProc.applyToText(text);
    // Text is still empty text.
    REQUIRE( text == "" );
  }

  SECTION("table code without new lines and spaces")
  {
    const std::string original = "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]";
    std::string text = original;
    preProc.applyToText(text);
    // Text is still the same.
    REQUIRE( text == original );
  }

  SECTION("text with new line before [tr]")
  {
    std::string text = "[table]\n[tr][td]one[/td][td]two[/td][/tr]\n[tr][td]three[/td][td]four[/td][/tr][/table]";
    preProc.applyToText(text);
    // Line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }

  SECTION("text with space and new line before [tr]")
  {
    std::string text = "[table]\n[tr][td]one[/td][td]two[/td][/tr]  \n[tr][td]three[/td][td]four[/td][/tr][/table]";
    preProc.applyToText(text);
    // Spaces and line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }

  SECTION("text with new line before [td]")
  {
    std::string text = "[table][tr]\n[td]one[/td]\n[td]two[/td][/tr][tr]\n[td]three[/td]\n[td]four[/td][/tr][/table]";
    preProc.applyToText(text);
    // Line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }

  SECTION("text with spaces and new line before [td]")
  {
    std::string text = "[table][tr] \n[td]one[/td]  \n[td]two[/td][/tr][tr]   \n[td]three[/td]    \n[td]four[/td][/tr][/table]";
    preProc.applyToText(text);
    // Spaces and line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }

  SECTION("spaces and line feeds inside cells are not removed")
  {
    std::string text = "[table][tr][td]one one[/td][td]two\ntwo[/td][/tr][tr][td]three 3\nthree[/td][td]four four\nfour four[/td][/tr][/table]";
    preProc.applyToText(text);
    // Spaces and line feeds are gone.
    REQUIRE( text == "[table][tr][td]one one[/td][td]two\ntwo[/td][/tr][tr][td]three 3\nthree[/td][td]four four\nfour four[/td][/tr][/table]" );
  }

  SECTION("text with new line before [td] and [tr]")
  {
    std::string text = "[table]\n[tr]\n[td]one[/td]\n[td]two[/td]\n[/tr]\n[tr]\n[td]three[/td]\n[td]four[/td]\n[/tr][/table]";
    preProc.applyToText(text);
    // Line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }

  SECTION("text with spaces and new line before [td] and [tr]")
  {
    std::string text = "[table]\n[tr]  \n[td]one[/td] \n[td]two[/td]   \n[/tr] \n[tr] \n[td]three[/td]   \n[td]four[/td]  \n[/tr][/table]";
    preProc.applyToText(text);
    // Spaces and line feeds are gone.
    REQUIRE( text == "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]" );
  }
}
