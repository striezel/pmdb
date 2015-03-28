/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Thoronador

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

#include <iostream>
#include <map>
#include "../../code/bbcode/SimpleTemplateBBCode.hpp"

int main()
{
  //fictious tag <tag>
  SimpleTemplateBBCode simple("tag", MsgTemplate("<tag id=\"{..inner..}\">{..inner..}</tag>"), "inner");

  // Populate map with strings for testing.
  //   Keys:   input string
  //   Values: expected output string
  std::map<std::string, std::string> tests;
  //empty string stays unchanged
  tests[""] = "";
  //unchanged, no code
  tests["There is no code here."] = "There is no code here.";
  //simple code
  tests["[tag]123[/tag]"] = "<tag id=\"123\">123</tag>";
  //code with upper case letters
  tests["[TAG]123[/TAG]"] = "<tag id=\"123\">123</tag>";
  //only first letter in upper case
  tests["[Tag]123[/Tag]"] = "<tag id=\"123\">123</tag>";
  //mixed case code
  tests["[TaG]123[/tAg]"] = "<tag id=\"123\">123</tag>";
  //letters inside
  tests["[tag]abcdefgh[/tag]"] = "<tag id=\"abcdefgh\">abcdefgh</tag>";
  //HTML entity inside - should not get escaped
  tests["[tag]&uuml;[/tag]"] = "<tag id=\"&uuml;\">&uuml;</tag>";
  //HTML entity inside - should not get escaped
  tests["[tag]foo&quot;bar[/tag]"] = "<tag id=\"foo&quot;bar\">foo&quot;bar</tag>";
  //less than inside - should not get escaped
  tests["[tag]foo<bar[/tag]"] = "<tag id=\"foo<bar\">foo<bar</tag>";
  //greater than inside - should not get escaped
  tests["[tag]foo>bar[/tag]"] = "<tag id=\"foo>bar\">foo>bar</tag>";
  //another tag inside - should not get escaped
  tests["[tag]<foo>bar[/tag]"] = "<tag id=\"<foo>bar\"><foo>bar</tag>";
  //start code only - should not change
  tests["[tag]123"] = "[tag]123";
  //end code only - should not change
  tests["123[/tag]"] = "123[/tag]";
  //incomplete again - no change
  tests["[tag]123[/tag"] = "[tag]123[/tag";

  //iterate over all given strings and check, if they get the expected result
  std::map<std::string, std::string>::const_iterator iter = tests.begin();
  while (iter != tests.end())
  {
    std::string text = iter->first;
    simple.applyToText(text);
    if (text != iter->second)
    {
      std::cout << "Error: Some code was not transformed properly!\n"
                << "Instance: SimpleTemplateBBCode(\"tag\", ..., \"inner\")\n"
                << "Original text:   \""<<iter->first<<"\"\n"
                << "Expected result: \""<<iter->second<<"\"\n"
                << "Actual result:   \""<<text<<"\"\n";
      return 1;
    }
    ++iter;
  } //while
  std::cout << "Passed all SimpleTemplateBBCode code tests.\n";
  return 0;
}
