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
#include "../../../code/bbcode/SpoilerBBCode.hpp"

TEST_CASE("SpoilerBBCode")
{
  const SpoilerBBCode spoiler("spoiler");

  SECTION("no spoiler code")
  {
     std::string text = "There is no spoiler here.";
     const std::string expected = text;
     spoiler.applyToText(text);
     REQUIRE( text == expected );
  }

  SECTION("spoiler code")
  {
     std::string text = "There is a spoiler.\n[SPOILER]Secret text.[/SPOILER]\nDone.";
     const std::string expected = "There is a spoiler.\n<div style=\"margin: 5px;\"><span>Spoiler: (zum Lesen bitte Text markieren)</span><div style=\"background:#999; color: #999; border: 1px solid #ff0000; padding: 5px;\"><div style=\" border-color:#4F4F4F #CECECE #CECECE #4F4F4F; border-width: 1px; border-style: solid; margin: -2px;\">Secret text.</div></div></div>\nDone.";
     spoiler.applyToText(text);
     REQUIRE( text == expected );
  }
}
