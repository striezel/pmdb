/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
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
#include <string>
#include <tuple> // available from C++11 onwards
#include <vector>
#include "../../bbcode/HorizontalRuleBBCode.hpp"

typedef std::tuple<std::string, std::string, std::string> TestData;

int main()
{
  //We just use the hr code for testing.
  HorizontalRuleBBCode rule_html("hr", false);
  HorizontalRuleBBCode rule_xhtml("hr", true);
  /* upper-case HR code -> should be the same as lower-case hr */
  HorizontalRuleBBCode rule_html_up("HR", false);
  HorizontalRuleBBCode rule_xhtml_up("HR", true);

  // Populate vector with strings for testing.
  //   1st element: input string
  //   2nd element: expected output string for HTML variant
  //   3rd element: expected output string for XHTML variant
  std::vector<TestData> tests;
  // Text without code should not change.
  tests.push_back(TestData("This text should be unchanged.",
                           "This text should be unchanged.",
                           "This text should be unchanged."));
  // [hr][/hr] should be replaced
  tests.push_back(TestData("Rule [hr][/hr] Britannia",
                           "Rule <hr> Britannia",
                           "Rule <hr /> Britannia"));
  // ...even if the first [HR] is upper case
  tests.push_back(TestData("Rule [HR][/hr] Britannia",
                           "Rule <hr> Britannia",
                           "Rule <hr /> Britannia"));
  // ... or the last one ...
  tests.push_back(TestData("Rule [hr][/HR] Britannia",
                           "Rule <hr> Britannia",
                           "Rule <hr /> Britannia"));
  // .. or even both.
  tests.push_back(TestData("Rule [HR][/HR] Britannia",
                           "Rule <hr> Britannia",
                           "Rule <hr /> Britannia"));
  // Just a single [hr] should not change the text.
  tests.push_back(TestData("Rule [hr] Britannia",
                           "Rule [hr] Britannia",
                           "Rule [hr] Britannia"));
  // ...and neither should a single [/hr].
  tests.push_back(TestData("Rule [/hr] Britannia",
                           "Rule [/hr] Britannia",
                           "Rule [/hr] Britannia"));

  //iterate over all given strings and check, if they get the expected result
  for (auto&& item : tests)
  {
    //HTML, lower case BBCode name
    std::string text = std::get<0>(item);
    rule_html.applyToText(text);
    if (text != std::get<1>(item))
    {
      std::cout << "Error: Output from HTML version of horizontal rule code does not match the expected result!\n"
                << "Text:     " << std::get<0>(item) << std::endl
                << "Result:   " << text << std::endl
                << "Expected: " << std::get<1>(item) << std::endl;
      return 1;
    }

    //XHTML, lower case BBCode name
    text = std::get<0>(item);
    rule_xhtml.applyToText(text);
    if (text != std::get<2>(item))
    {
      std::cout << "Error: Output from XHTML version of horizontal rule code does not match the expected result!\n"
                << "Text:     " << std::get<0>(item) << std::endl
                << "Result:   " << text << std::endl
                << "Expected: " << std::get<2>(item) << std::endl;
      return 1;
    }

    //HTML, upper case BBCode name
    text = std::get<0>(item);
    rule_html_up.applyToText(text);
    if (text != std::get<1>(item))
    {
      std::cout << "Error: Output from HTML version of upper case horizontal rule code does not match the expected result!\n"
                << "Text:     " << std::get<0>(item) << std::endl
                << "Result:   " << text << std::endl
                << "Expected: " << std::get<1>(item) << std::endl;
      return 1;
    }

    //XHTML, upper case BBCode name
    text = std::get<0>(item);
    rule_xhtml_up.applyToText(text);
    if (text != std::get<2>(item))
    {
      std::cout << "Error: Output from XHTML version of upper case horizontal rule code does not match the expected result!\n"
                << "Text:     " << std::get<0>(item) << std::endl
                << "Result:   " << text << std::endl
                << "Expected: " << std::get<2>(item) << std::endl;
      return 1;
    }
  } //for (range-based)

  //all test cases matched the expected result
  return 0;
}
