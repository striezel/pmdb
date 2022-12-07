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
#include "../../code/bbcode/TableBBCode.hpp"

TEST_CASE("TableBBCode")
{
  // some rather standard table BB code
  TableBBCode table("table");

  // Populate map with strings for testing.
  //   Keys:   input string
  //   Values: expected output string
  std::map<std::string, std::string> tests;
  // empty string stays unchanged
  tests[""] = "";
  // unchanged, no code
  tests["There is no table here."] = "There is no table here.";
  // simple table, one cell only
  tests["[table][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td>Content goes here.</td></tr></table>";
  // simple table, two cells in one row
  tests["[table][tr][td]Content[/td][td]more text[/td][/tr][/table]"]
     = "<table><tr><td>Content</td><td>more text</td></tr></table>";
  // simple table, two cells only in two rows
  tests["[table][tr][td]Content[/td][/tr][tr][td]more text[/td][/tr][/table]"]
      = "<table><tr><td>Content</td></tr><tr><td>more text</td></tr></table>";
  // four cells in two rows: two cells in each row
  tests["[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]"]
      = "<table><tr><td>one</td><td>two</td></tr><tr><td>three</td><td>four</td></tr></table>";
  // same, but with colspan
  tests["[table][tr][td=\"colspan: 2\"]one[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]"]
      = "<table><tr><td colspan=\"2\">one</td></tr><tr><td>three</td><td>four</td></tr></table>";

  // table with width
  tests["[table=\"width: 250\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table width=\"250\"><tr><td>Content goes here.</td></tr></table>";
  // table with width in percent
  tests["[table=\"width: 25%\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table width=\"25%\"><tr><td>Content goes here.</td></tr></table>";

  // cell with width
  tests["[table][tr][td=\"width: 250\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td width=\"250\">Content goes here.</td></tr></table>";
  // cell with width in percent
  tests["[table][tr][td=\"width: 25%\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td width=\"25%\">Content goes here.</td></tr></table>";

  // table with align left
  tests["[table=\"align: left\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table align=\"left\"><tr><td>Content goes here.</td></tr></table>";
  // table with align right
  tests["[table=\"align: right\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table align=\"right\"><tr><td>Content goes here.</td></tr></table>";
  // table with align center
  tests["[table=\"align: center\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table align=\"center\"><tr><td>Content goes here.</td></tr></table>";

  // cell with align left
  tests["[table][tr][td=\"align: left\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td align=\"left\">Content goes here.</td></tr></table>";
  // cell with align right
  tests["[table][tr][td=\"align: right\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td align=\"right\">Content goes here.</td></tr></table>";
  // cell with align center
  tests["[table][tr][td=\"align: center\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td align=\"center\">Content goes here.</td></tr></table>";

  // cell with border
  tests["[table][tr][td=\"class: grid\"]Content goes here.[/td][/tr][/table]"]
      = "<table><tr><td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>";
  // row with border - border style gets applied to child elements, too
  tests["[table][tr=\"class: grid\"][td]Content goes here.[/td][/tr][/table]"]
      = "<table>"
       + std::string("<tr style=\"border: 1px solid #000000; border-collapse: collapse;\">")
       + "<td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>";
  // table with border - border style gets applied to child elements, too
  tests["[table=\"class: grid\"][tr][td]Content goes here.[/td][/tr][/table]"]
      = "<table style=\"border: 1px solid #000000; border-collapse: collapse;\">"
       + std::string("<tr style=\"border: 1px solid #000000; border-collapse: collapse;\">")
       + "<td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>";


  // iterate over all given strings and check, if they get the expected result
  for (const auto& [key, value]: tests)
  {
    std::string text = key;
    table.applyToText(text);
    REQUIRE( text == value );
  }
}
