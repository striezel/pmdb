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
#include "../../bbcode/BBCode.hpp"

int main()
{
  //We just use the b code for testing.
  SimpleBBCode simple("b");

  // Populate map with strings for testing.
  //   Keys:   input string
  //   Values: expected output string
  std::map<std::string, std::string> tests;
  // Text without code should not change.
  tests["This text should be unchanged."] = "This text should be unchanged.";
  // [b]...[/b] should be replaced
  tests["don't be [b]bold[/b] here"]      = "don't be <b>bold</b> here";
  // ...even if the first [B] is upper case
  tests["don't be [B]bold[/b] here"]      = "don't be <b>bold</b> here";
  // ... or the last one ...
  tests["don't be [b]bold[/B] here"]      = "don't be <b>bold</b> here";
  // .. or even both.
  tests["don't be [B]bold[/B] here"]      = "don't be <b>bold</b> here";
  // Just a single [b] should not change the text.
  tests["don't be [b]bold here"]          = "don't be [b]bold here";
  // ...and neither should a single [/b].
  tests["don't be bo[/b]ld here"]          = "don't be bo[/b]ld here";

  //iterate over all given strings and chech, if they get the expected result
  std::map<std::string, std::string>::const_iterator iter = tests.begin();
  while (iter != tests.end())
  {
    std::string text = iter->first;
    simple.applyToText(text);
    if (text != iter->second)
    {
      std::cout << "Error: Some code was not transformed properly!\n"
                << "Instance: SimpleBBCode(\"b\")\n"
                << "Original text:   \""<<iter->first<<"\"\n"
                << "Expected result: \""<<iter->second<<"\"\n"
                << "Actual result:   \""<<text<<"\"\n";
      return 1;
    }
    ++iter;
  } //while


  /* upper-case B code -> should be the same as lower-case b */
  SimpleBBCode simple_upper("B");
  iter = tests.begin();
  while (iter != tests.end())
  {
    std::string text = iter->first;
    simple_upper.applyToText(text);
    if (text != iter->second)
    {
      std::cout << "Error: Some code was not transformed properly!\n"
                << "Instance: SimpleBBCode(\"B\")\n"
                << "Original text:   \""<<iter->first<<"\"\n"
                << "Expected result: \""<<iter->second<<"\"\n"
                << "Actual result:   \""<<text<<"\"\n";
      return 1;
    }
    ++iter;
  } //while

  /* test with multi-character code */
  // We just use the blob code for testing.
  // (Not a real-life code, but hey... it's all about the basic principle.)
  SimpleBBCode simple_longer("blob");
  tests.clear(); //clear entries for single character code
  // Text without code should not change.
  tests["This text should be unchanged."] = "This text should be unchanged.";
  // [b]...[/b] should be replaced
  tests["don't be [blob]bold[/blob] here"]      = "don't be <blob>bold</blob> here";
  // ...even if the first [B] is upper case
  tests["don't be [BlOb]bold[/blob] here"]      = "don't be <blob>bold</blob> here";
  // ... or the last one ...
  tests["don't be [blob]bold[/BLOb] here"]      = "don't be <blob>bold</blob> here";
  // .. or even both.
  tests["don't be [BLOB]bold[/BLOB] here"]      = "don't be <blob>bold</blob> here";
  // Just a single [b] should not change the text.
  tests["don't be [blob]bold here"]          = "don't be [blob]bold here";
  // ...and neither should a single [/b].
  tests["don't be bo[/blob]ld here"]          = "don't be bo[/blob]ld here";

  iter = tests.begin();
  while (iter != tests.end())
  {
    std::string text = iter->first;
    simple_longer.applyToText(text);
    if (text != iter->second)
    {
      std::cout << "Error: Some code was not transformed properly!\n"
                << "Instance: SimpleBBCode(\"blob\")\n"
                << "Original text:   \""<<iter->first<<"\"\n"
                << "Expected result: \""<<iter->second<<"\"\n"
                << "Actual result:   \""<<text<<"\"\n";
      return 1;
    }
    ++iter;
  } //while

  //all test cases matched the expected result
  return 0;
}
