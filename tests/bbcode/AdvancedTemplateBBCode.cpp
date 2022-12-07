/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
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
#include "../../code/bbcode/AdvancedTemplateBBCode.hpp"

TEST_CASE("AdvancedTemplateBBCode")
{
  // fictious tag <tag>
  AdvancedTemplateBBCode advanced("tag", MsgTemplate("<tag value=\"{..attr..}\">{..inner..}</tag>"), "inner", "attr");

  // Populate map with strings for testing.
  //   Keys:   input string
  //   Values: expected output string
  std::map<std::string, std::string> tests;
  // empty string stays unchanged
  tests[""] = "";
  // unchanged, no code
  tests["There is no code here."] = "There is no code here.";
  // simple code
  tests["[tag=abc]123[/tag]"] = "<tag value=\"abc\">123</tag>";
  // code with upper case letters
  tests["[TAG=abc]123[/TAG]"] = "<tag value=\"abc\">123</tag>";
  // only first letter in upper case
  tests["[Tag=abc]123[/Tag]"] = "<tag value=\"abc\">123</tag>";
  // mixed case code
  tests["[TaG=def]123[/tAg]"] = "<tag value=\"def\">123</tag>";
  // letters inside
  tests["[tag=qwert]abcdefgh[/tag]"] = "<tag value=\"qwert\">abcdefgh</tag>";
  // HTML entity inside - should not get escaped
  tests["[tag=aaa]&uuml;[/tag]"] = "<tag value=\"aaa\">&uuml;</tag>";
  // HTML entity inside - should not get escaped
  tests["[tag=baz]foo&quot;bar[/tag]"] = "<tag value=\"baz\">foo&quot;bar</tag>";
  // less than inside - should not get escaped
  tests["[tag=baz]foo<bar[/tag]"] = "<tag value=\"baz\">foo<bar</tag>";
  // greater than inside - should not get escaped
  tests["[tag=baz]foo>bar[/tag]"] = "<tag value=\"baz\">foo>bar</tag>";
  // another tag inside - should not get escaped
  tests["[tag=asd]<foo>bar[/tag]"] = "<tag value=\"asd\"><foo>bar</tag>";
  // start code only - should not change
  tests["[tag=abc]123"] = "[tag=abc]123";
  // end code only - should not change
  tests["123[/tag]"] = "123[/tag]";
  // incomplete again - no change
  tests["[tag=abc]123[/tag"] = "[tag=abc]123[/tag";

  // iterate over all given strings and check, if they get the expected result
  for (const auto& [key, value]: tests)
  {
    std::string text = key;
    advanced.applyToText(text);
    REQUIRE( text == value );
  }
}
