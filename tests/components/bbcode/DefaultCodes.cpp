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
#include "../../../code/bbcode/DefaultCodes.hpp"

TEST_CASE("default BB codes")
{
  const std::string forum_url = "https://for.um/";

  // Currently, the BBCodeParser has no way to directly get the added codes.
  // So we just check whether they are actually applied to texts instead.

  SECTION("check that [b], [i], [s] and [u] codes get applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "[b]bold[/b] [i]italics[/i] [s]strike throug[/s] [u]underlined[/u]";
    const std::string expected = "<b>bold</b> <i>italics</i> <span style=\"text-decoration:line-through;\">strike throug</span> <u>underlined</u>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("check that [sub] code gets applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "a[sub]1[/sub]+a[sub]2[/sub]=a[sub]3[/sub]";
    const std::string expected = "a<sub>1</sub>+a<sub>2</sub>=a<sub>3</sub>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("check that [sup] code gets applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "a[sup]2[/sup]+b[sup]2[/sup]=c[sup]2[/sup]";
    const std::string expected = "a<sup>2</sup>+b<sup>2</sup>=c<sup>2</sup>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("check that alignment codes get applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "[left]left text[/left]\n[right]right text[/right]\n[center]centered text[/center]";
    const std::string expected = "<div align=\"left\">left text</div><br>\n<div align=\"right\">right text</div><br>\n<center>centered text</center>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("check that [tt] code gets applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "[tt]code[/tt]";
    const std::string expected = "<tt style=\"font-size: medium\">code</tt>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("check that simple [url] code gets applied")
  {
    BBCodeParser parser;
    bbcode_default::addDefaultCodes(parser);

    const std::string input = "[url]https://example.com/?foo=bar&baz=1[/url]";
    const std::string expected = "<a href=\"https://example.com/?foo=bar&amp;baz=1\" target=\"_blank\">https://example.com/?foo=bar&amp;baz=1</a>";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  // TODO: Add more checks for the other default codes.
}
