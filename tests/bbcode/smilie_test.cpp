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
#include <vector>
#include "../../code/bbcode/Smilie.hpp"

struct TData
{
  TData(const std::string& txt, const std::string& fURL, const bool xhtml, const std::string& out)
  : text(txt),
    forumURL(fURL),
    XHTML(xhtml),
    output(out)
  {
  }

  std::string text;
  std::string forumURL;
  bool XHTML;

  std::string output;
}; //struct

int main()
{
  const std::string URL = "https://for.um/path/";

  Smilie happy_relative(":)", "happy.png", true);

  // Populate map with data for tests.
  std::vector<TData> tests;
  //empty string - should be unchanged
  tests.push_back(TData("", URL, true, ""));
  tests.push_back(TData("", URL, false, ""));
  //text without smilie code - no changes in output
  tests.push_back(TData("foo bar baz", URL, true, "foo bar baz"));
  tests.push_back(TData("foo bar baz", URL, false, "foo bar baz"));
  //just smilie code
  tests.push_back(TData(":)", URL, true,  "<img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\" />"));
  tests.push_back(TData(":)", URL, false, "<img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\">"));
  //two consecutive smilies
  tests.push_back(TData(":):)", URL, true,  "<img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\" /><img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\" />"));
  tests.push_back(TData(":):)", URL, false, "<img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\"><img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\">"));
  //smilie within text
  tests.push_back(TData("Hey! :) Happy face here.", URL, true,  "Hey! <img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\" /> Happy face here."));
  tests.push_back(TData("Hey! :) Happy face here.", URL, false, "Hey! <img src=\""+URL+"happy.png\" alt=\":)\" border=\"0\"> Happy face here."));

  //Iterate over all given data and check, if the output is as expected.
  std::vector<TData>::const_iterator iter = tests.begin();
  while (iter != tests.end())
  {
    std::string input(iter->text);
    happy_relative.applyToText(input, iter->forumURL, iter->XHTML);
    if (input != iter->output)
    {
      std::cout << "Error: Output text does not match the expected result!\n"
                << "Output:   \"" << input << "\"\n"
                << "Expected: \"" << iter->output << "\"\n";
      return 1;
    } //if
    ++iter;
  } //while
  std::cout << "Passed all smilie tests with relative URL!\n";


  //smilie without relative URL
  Smilie happy_absolute(":)", "happy.png", false);

  // Populate map with data for tests.
  tests.clear();
  //empty string - should be unchanged
  tests.push_back(TData("", URL, true, ""));
  tests.push_back(TData("", URL, false, ""));
  //text without smilie code - no changes in output
  tests.push_back(TData("foo bar baz", URL, true, "foo bar baz"));
  tests.push_back(TData("foo bar baz", URL, false, "foo bar baz"));
  //just smilie code
  tests.push_back(TData(":)", URL, true,  "<img src=\"happy.png\" alt=\":)\" border=\"0\" />"));
  tests.push_back(TData(":)", URL, false, "<img src=\"happy.png\" alt=\":)\" border=\"0\">"));
  //two consecutive smilies
  tests.push_back(TData(":):)", URL, true,  "<img src=\"happy.png\" alt=\":)\" border=\"0\" /><img src=\"happy.png\" alt=\":)\" border=\"0\" />"));
  tests.push_back(TData(":):)", URL, false, "<img src=\"happy.png\" alt=\":)\" border=\"0\"><img src=\"happy.png\" alt=\":)\" border=\"0\">"));
  //smilie within text
  tests.push_back(TData("Hey! :) Happy face here.", URL, true,  "Hey! <img src=\"happy.png\" alt=\":)\" border=\"0\" /> Happy face here."));
  tests.push_back(TData("Hey! :) Happy face here.", URL, false, "Hey! <img src=\"happy.png\" alt=\":)\" border=\"0\"> Happy face here."));

  iter = tests.begin();
  while (iter != tests.end())
  {
    std::string input(iter->text);
    happy_absolute.applyToText(input, iter->forumURL, iter->XHTML);
    if (input != iter->output)
    {
      std::cout << "Error: Output text does not match the expected result!\n"
                << "Output:   \"" << input << "\"\n"
                << "Expected: \"" << iter->output << "\"\n";
      return 1;
    } //if
    ++iter;
  } //while
  std::cout << "Passed all smilie tests with non-relative URL!\n";

  return 0;
}
