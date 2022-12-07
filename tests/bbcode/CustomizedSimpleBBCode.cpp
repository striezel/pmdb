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
#include "../../code/bbcode/CustomizedSimpleBBCode.hpp"

TEST_CASE("CustomizedSimpleBBCode")
{
  SECTION("multi-character code")
  {
    // We just call the code for testing "rand" (as in random).
    // (Not a real-life code, but hey... it's all about the basic principle.)
    CustomizedSimpleBBCode customizedSimple("rand", "<random_start>", "</random_end>");

    // Populate map with strings for testing.
    //   Keys:   input string
    //   Values: expected output string
    std::map<std::string, std::string> tests;
    // Text without code should not change.
    tests["This text should be unchanged."]       = "This text should be unchanged.";
    // [rand]...[/rand] should be replaced
    tests["don't be [rand]random[/rand] here"]    = "don't be <random_start>random</random_end> here";
    // ...even if the first [RAND] is upper case
    tests["don't be [RAND]random[/rand] here"]    = "don't be <random_start>random</random_end> here";
    // ... or the last one ...
    tests["don't be [rand]random[/RAND] here"]    = "don't be <random_start>random</random_end> here";
    // .. or even both.
    tests["don't be [RAND]arbitrary[/RAND] here"] = "don't be <random_start>arbitrary</random_end> here";
    // Just a single [rand] should not change the text.
    tests["don't be [rand]randomized here"]       = "don't be [rand]randomized here";
    // ...and neither should a single [/rand].
    tests["don't be mysterious[/rand] here"]      = "don't be mysterious[/rand] here";

    // iterate over all given strings and check, if they get the expected result
    for (const auto& [key, value]: tests)
    {
      std::string text = key;
      customizedSimple.applyToText(text);
      REQUIRE( text == value );
    }


    /* upper-case RAND code -> should be the same as lower-case rand */
    CustomizedSimpleBBCode customizedSimple_upper("RAND", "<random_start>", "</random_end>");
    for (const auto& [key, value]: tests)
    {
      std::string text = key;
      customizedSimple_upper.applyToText(text);
      REQUIRE( text == value );
    }
  }

  SECTION("single-character code")
  {
    /* test with single-character code */
    // We just use the r code for testing.
    CustomizedSimpleBBCode customizedSimple_singleChar("r", "<random_start>", "</random_end>");
    std::map<std::string, std::string> tests;
    // Text without code should not change.
    tests["This text should be unchanged. Really, really."] = "This text should be unchanged. Really, really.";
    // [rand]...[/rand] should be replaced
    tests["don't be [r]random[/r] here"]      = "don't be <random_start>random</random_end> here";
    // ...even if the first [R] is upper case
    tests["don't be [R]random[/r] here"]      = "don't be <random_start>random</random_end> here";
    // ... or the last one ...
    tests["don't be [r]random[/R] here"]      = "don't be <random_start>random</random_end> here";
    // .. or even both.
    tests["don't be [R]arbitrary[/R] here"]      = "don't be <random_start>arbitrary</random_end> here";
    // Just a single [R] should not change the text.
    tests["don't be [r]randomized here"]          = "don't be [r]randomized here";
    // ...and neither should a single [/r].
    tests["don't be mysterious[/r] here"]          = "don't be mysterious[/r] here";

    for (const auto& [key, value]: tests)
    {
      std::string text = key;
      customizedSimple_singleChar.applyToText(text);
      REQUIRE( text == value );
    }
  }
}
