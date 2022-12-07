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
#include "../../code/bbcode/ListBBCode.hpp"

TEST_CASE("ListBBCode")
{
  SECTION("unordered list code test")
  {
    // some rather standard list code
    ListBBCode listCode_unordered("list", true);

    // Populate map with strings for testing.
    //   Keys:   input string
    //   Values: expected output string
    std::map<std::string, std::string> tests;
    // empty string stays unchanged
    tests[""] = "";
    // unchanged, no code
    tests["There is no list here."] = "There is no list here.";
    // minimalistic list example
    tests["[list][*]item[/list]"] = "<ul><li>item</li></ul>";
    // simple list with multiple items
    tests["This is a list. [list][*]one[*]two[*]three[/list]"] = "This is a list. <ul><li>one</li><li>two</li><li>three</li></ul>";
    // simple list with multiple items - upper case codes
    tests["This is a list. [LIST][*]one[*]two[*]three[/LIST]"] = "This is a list. <ul><li>one</li><li>two</li><li>three</li></ul>";
    // simple list with multiple items - upper case first letter in codes
    tests["This is a list. [List][*]one[*]two[*]three[/List]"] = "This is a list. <ul><li>one</li><li>two</li><li>three</li></ul>";
    // simple list with multiple items - upper and lower case alternate
    tests["This is a list. [LiSt][*]one[*]two[*]three[/lIsT]"] = "This is a list. <ul><li>one</li><li>two</li><li>three</li></ul>";
    // incomplete list - code should refuse to change text
    tests["Incomplete [list][*]one[*]two[*]three"] = "Incomplete [list][*]one[*]two[*]three";
    // list without items should not be parsed
    tests["List without items [list]incoming[/list]"] = "List without items [list]incoming[/list]";
    // Nested lists should be just fine.
    tests["I heard you like lists, so [list][*]I [list][*]made[*]a[*]list[/list][*]inside[*]your list.[/list]"]
          = "I heard you like lists, so <ul><li>I <ul><li>made</li><li>a</li><li>list</li></ul></li><li>inside</li><li>your list.</li></ul>";
    // list with line breaks between items - line breaks should not be touched here
    tests[std::string("Hey!\n")
          + "[list]\n"
          + "[*]once\n"
          + "[*]there\n"
          + "[*]was a\n"
          + "[*]crash test dummy,\n"
          + "[*]who felt crashs ain't funny.\n"
          + "[/list]"] = std::string("Hey!\n<ul>\n<li>once\n</li><li>there\n</li><li>was a\n</li>")
                       + "<li>crash test dummy,\n</li><li>who felt crashs ain't funny.\n</li></ul>";

    // iterate over all given strings and check, if they get the expected result
    for (const auto& [key, value]: tests)
    {
      std::string text = key;
      listCode_unordered.applyToText(text);
      REQUIRE( text == value );
    }
  }

  SECTION("ordered list code test")
  {
    // ordered list code
    ListBBCode listCode_ordered("list", false);

    // Populate map with strings for testing.
    //   Keys:   input string
    //   Values: expected output string
    std::map<std::string, std::string> tests;
    // empty string stays unchanged
    tests[""] = "";
    // unchanged, no code
    tests["There is no list here."] = "There is no list here.";
    // minimalistic list example
    tests["[list][*]item[/list]"] = "<ol><li>item</li></ol>";
    // simple list with multiple items
    tests["This is a list. [list][*]one[*]two[*]three[/list]"] = "This is a list. <ol><li>one</li><li>two</li><li>three</li></ol>";
    // simple list with multiple items - upper case codes
    tests["This is a list. [LIST][*]one[*]two[*]three[/LIST]"] = "This is a list. <ol><li>one</li><li>two</li><li>three</li></ol>";
    // simple list with multiple items - upper case first letter in codes
    tests["This is a list. [List][*]one[*]two[*]three[/List]"] = "This is a list. <ol><li>one</li><li>two</li><li>three</li></ol>";
    // simple list with multiple items - upper and lower case alternate
    tests["This is a list. [LiSt][*]one[*]two[*]three[/lIsT]"] = "This is a list. <ol><li>one</li><li>two</li><li>three</li></ol>";
    // incomplete list - code should refuse to change text
    tests["Incomplete [list][*]one[*]two[*]three"] = "Incomplete [list][*]one[*]two[*]three";
    // list without items should not be parsed
    tests["List without items [list]incoming[/list]"] = "List without items [list]incoming[/list]";
    // Nested lists should be just fine.
    tests["I heard you like lists, so [list][*]I [list][*]made[*]a[*]list[/list][*]inside[*]your list.[/list]"]
          = "I heard you like lists, so <ol><li>I <ol><li>made</li><li>a</li><li>list</li></ol></li><li>inside</li><li>your list.</li></ol>";
    // list with line breaks between items - line breaks should not be touched here
    tests[std::string("Hey!\n")
          + "[list]\n"
          + "[*]once\n"
          + "[*]there\n"
          + "[*]was a\n"
          + "[*]crash test dummy,\n"
          + "[*]who felt crashs ain't funny.\n"
          + "[/list]"] = std::string("Hey!\n<ol>\n<li>once\n</li><li>there\n</li><li>was a\n</li>")
                       + "<li>crash test dummy,\n</li><li>who felt crashs ain't funny.\n</li></ol>";

    // iterate over all given strings and check, if they get the expected result
    for (const auto& [key, value]: tests)
    {
      std::string text = key;
      listCode_ordered.applyToText(text);
      REQUIRE( text == value );
    }
  }
}
