/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015, 2022, 2025  Dirk Stolle

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
#include "../../../code/bbcode/Smilie.hpp"

struct TData
{
  TData(const std::string& txt, const std::string& fURL, const HTMLStandard the_standard, const std::string& out)
  : text(txt),
    forumURL(fURL),
    standard(the_standard),
    output(out)
  {
  }

  std::string text;
  std::string forumURL;
  HTMLStandard standard;

  std::string output;
}; // struct

TEST_CASE("Smilie")
{
  const std::string URL = "https://for.um/path/";

  SECTION("smilie with relative URL")
  {
    Smilie happy_relative(":)", "happy.png", UrlType::Relative);

    // Populate map with data for tests.
    std::vector<TData> tests;
    // empty string - should be unchanged
    tests.push_back(TData("", URL, HTMLStandard::XHTML, ""));
    tests.push_back(TData("", URL, HTMLStandard::HTML4_01, ""));
    // text without smilie code - no changes in output
    tests.push_back(TData("foo bar baz", URL, HTMLStandard::XHTML, "foo bar baz"));
    tests.push_back(TData("foo bar baz", URL, HTMLStandard::HTML4_01, "foo bar baz"));
    // just smilie code
    tests.push_back(TData(":)", URL, HTMLStandard::XHTML,  "<img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\" />"));
    tests.push_back(TData(":)", URL, HTMLStandard::HTML4_01, "<img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\">"));
    // two consecutive smilies
    tests.push_back(TData(":):)", URL, HTMLStandard::XHTML,  "<img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\" /><img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\" />"));
    tests.push_back(TData(":):)", URL, HTMLStandard::HTML4_01, "<img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\"><img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\">"));
    // smilie within text
    tests.push_back(TData("Hey! :) Happy face here.", URL, HTMLStandard::XHTML,  "Hey! <img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\" /> Happy face here."));
    tests.push_back(TData("Hey! :) Happy face here.", URL, HTMLStandard::HTML4_01, "Hey! <img src=\"" + URL + "happy.png\" alt=\":)\" border=\"0\"> Happy face here."));

    // Iterate over all given data and check, if the output is as expected.
    for (const auto& element: tests)
    {
      std::string input(element.text);
      happy_relative.applyToText(input, element.forumURL, element.standard);
      REQUIRE( input == element.output );
    }
  }


  SECTION("smilie with absolute URL")
  {
    Smilie happy_absolute(":)", "happy.png", UrlType::Absolute);

    // Populate map with data for tests.
    std::vector<TData> tests;
    // empty string - should be unchanged
    tests.push_back(TData("", URL, HTMLStandard::XHTML, ""));
    tests.push_back(TData("", URL, HTMLStandard::HTML4_01, ""));
    // text without smilie code - no changes in output
    tests.push_back(TData("foo bar baz", URL, HTMLStandard::XHTML, "foo bar baz"));
    tests.push_back(TData("foo bar baz", URL, HTMLStandard::HTML4_01, "foo bar baz"));
    // just smilie code
    tests.push_back(TData(":)", URL, HTMLStandard::XHTML,  "<img src=\"happy.png\" alt=\":)\" border=\"0\" />"));
    tests.push_back(TData(":)", URL, HTMLStandard::HTML4_01, "<img src=\"happy.png\" alt=\":)\" border=\"0\">"));
    // two consecutive smilies
    tests.push_back(TData(":):)", URL, HTMLStandard::XHTML,  "<img src=\"happy.png\" alt=\":)\" border=\"0\" /><img src=\"happy.png\" alt=\":)\" border=\"0\" />"));
    tests.push_back(TData(":):)", URL, HTMLStandard::HTML4_01, "<img src=\"happy.png\" alt=\":)\" border=\"0\"><img src=\"happy.png\" alt=\":)\" border=\"0\">"));
    // smilie within text
    tests.push_back(TData("Hey! :) Happy face here.", URL, HTMLStandard::XHTML,  "Hey! <img src=\"happy.png\" alt=\":)\" border=\"0\" /> Happy face here."));
    tests.push_back(TData("Hey! :) Happy face here.", URL, HTMLStandard::HTML4_01, "Hey! <img src=\"happy.png\" alt=\":)\" border=\"0\"> Happy face here."));

    for (const auto& element: tests)
    {
      std::string input(element.text);
      happy_absolute.applyToText(input, element.forumURL, element.standard);
      REQUIRE( input == element.output );
    }
  }
}
