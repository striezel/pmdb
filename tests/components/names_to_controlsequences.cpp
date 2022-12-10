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
#include <vector>
#include <tuple>
#include "../../code/ConsoleColours.hpp"

TEST_CASE("ConsoleColours")
{
  /* Info:
     The test check, if all known colour names are properly transformed to
     their corresponding control sequences.
  */

  using TestData = std::tuple<std::string, bool, std::string>;
  std::vector<TestData> vec;

  /* add some values for known cases */
  // default - should be independent of background parameter
  vec.push_back(TestData("default", true,  colourDefault));
  vec.push_back(TestData("default", false, colourDefault));
  // other colours have different sequences for text and background colours
  vec.push_back(TestData("black",   false, colourBlack));
  vec.push_back(TestData("black",   true,  backgroundBlack));
  vec.push_back(TestData("blue",    false, colourBlue));
  vec.push_back(TestData("blue",    true,  backgroundBlue));
  vec.push_back(TestData("cyan",    false, colourCyan));
  vec.push_back(TestData("cyan",    true,  backgroundCyan));
  vec.push_back(TestData("green",   false, colourGreen));
  vec.push_back(TestData("green",   true,  backgroundGreen));
  vec.push_back(TestData("magenta", false, colourMagenta));
  vec.push_back(TestData("magenta", true,  backgroundMagenta));
  vec.push_back(TestData("red",     false, colourRed));
  vec.push_back(TestData("red",     true,  backgroundRed));
  vec.push_back(TestData("yellow",  false, colourYellow));
  vec.push_back(TestData("yellow",  true,  backgroundYellow));
  vec.push_back(TestData("white",   false, colourWhite));
  vec.push_back(TestData("white",   true,  backgroundWhite));

  for(const auto& item : vec)
  {
    const auto result = nameToControlSequence(std::get<0>(item), std::get<1>(item));
    REQUIRE( result == std::get<2>(item) );
  }
}
