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

#include "../../locate_catch.hpp"
#include "../../../code/bbcode/TableBBCode.hpp"

TEST_CASE("TableBBCode")
{
  // some rather standard table BB code
  TableBBCode table("table");

  SECTION("empty string stays unchanged")
  {
    std::string text = "";
    table.applyToText(text);
    REQUIRE( text == "" );
  }

  SECTION("text without table code is not changed")
  {
    std::string text = "There is no table here.";
    table.applyToText(text);
    REQUIRE( text == "There is no table here." );
  }

  SECTION("simple table tests")
  {
    SECTION("simple table, one cell only")
    {
      std::string text = "[table][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("simple table, two cells in one row")
    {
      std::string text = "[table][tr][td]Content[/td][td]more text[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>Content</td><td>more text</td></tr></table>" );
    }

    SECTION("simple table, two cells only in two rows")
    {
      std::string text = "[table][tr][td]Content[/td][/tr][tr][td]more text[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>Content</td></tr><tr><td>more text</td></tr></table>" );
    }

    SECTION("four cells in two rows: two cells in each row")
    {
      std::string text = "[table][tr][td]one[/td][td]two[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>one</td><td>two</td></tr><tr><td>three</td><td>four</td></tr></table>" );
    }
  }

  SECTION("colspan tests")
  {
    SECTION("four cells in two rows: two cells in each row, but with colspan")
    {
      std::string text = "[table][tr][td=\"colspan: 2\"]one[/td][/tr][tr][td]three[/td][td]four[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td colspan=\"2\">one</td></tr><tr><td>three</td><td>four</td></tr></table>" );
    }

    SECTION("six cells in two rows: three cells in each row, but with colspan")
    {
      std::string text = "[table][tr][td]one[/td][td]two[/td][td]three[/td][/tr][tr][td]four[/td][td=\"colspan: 2\"]five[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>one</td><td>two</td><td>three</td></tr><tr><td>four</td><td colspan=\"2\">five</td></tr></table>" );
    }
  }

  SECTION("width parameter tests")
  {
    SECTION("table with width in pixels")
    {
      std::string text = "[table=\"width: 250\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table width=\"250\"><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("table with width in percent")
    {
      std::string text = "[table=\"width: 25%\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table width=\"25%\"><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("cell with width in pixels")
    {
      std::string text = "[table][tr][td=\"width: 250\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td width=\"250\">Content goes here.</td></tr></table>" );
    }

    SECTION("cell with width in percent")
    {
      std::string text = "[table][tr][td=\"width: 25%\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td width=\"25%\">Content goes here.</td></tr></table>" );
    }
  }

  SECTION("alignment parameter tests")
  {
    SECTION("table with align left")
    {
      std::string text = "[table=\"align: left\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table align=\"left\"><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("table with align right")
    {
      std::string text = "[table=\"align: right\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table align=\"right\"><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("table with align center")
    {
      std::string text = "[table=\"align: center\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table align=\"center\"><tr><td>Content goes here.</td></tr></table>" );
    }

    SECTION("cell with align left")
    {
      std::string text = "[table][tr][td=\"align: left\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td align=\"left\">Content goes here.</td></tr></table>" );
    }

    SECTION("cell with align right")
    {
      std::string text = "[table][tr][td=\"align: right\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td align=\"right\">Content goes here.</td></tr></table>" );
    }

    SECTION("cell with align center")
    {
      std::string text = "[table][tr][td=\"align: center\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td align=\"center\">Content goes here.</td></tr></table>" );
    }
  }

  SECTION("grid / border tests")
  {
    SECTION("cell with border")
    {
      std::string text = "[table][tr][td=\"class: grid\"]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>" );
    }

    SECTION("cell with border using custom cell class")
    {
      const TableClasses classes(true, TableClasses::DefaultTableClass, TableClasses::DefaultRowClass, "my_cell_class_name");
      const TableBBCode code("table", classes);

      std::string text = "[table][tr][td=\"class: grid\"]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      REQUIRE( text == "<table><tr><td class=\"my_cell_class_name\">Content goes here.</td></tr></table>" );
    }

    SECTION("cell with border using default cell class")
    {
      const TableClasses classes(true, TableClasses::DefaultTableClass, TableClasses::DefaultRowClass, TableClasses::DefaultCellClass);
      const TableBBCode code("table", classes);

      std::string text = "[table][tr][td=\"class: grid\"]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      const std::string expected = "<table><tr><td class=\"" + TableClasses::DefaultCellClass + "\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("row with border - border style gets applied to child elements, too")
    {
      std::string text = "[table][tr=\"class: grid\"][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      const std::string expected = "<table>"
         + std::string("<tr style=\"border: 1px solid #000000; border-collapse: collapse;\">")
         + "<td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("row with border using custom row and cell class")
    {
      const TableClasses classes(true, TableClasses::DefaultTableClass, "my_row_class_name", "my_cell_class_name");
      const TableBBCode code("table", classes);

      std::string text = "[table][tr=\"class: grid\"][td]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      const std::string expected = "<table>"
         + std::string("<tr class=\"my_row_class_name\">")
         + "<td class=\"my_cell_class_name\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("row with border using default row and cell class")
    {
      const TableClasses classes(true, TableClasses::DefaultTableClass, TableClasses::DefaultRowClass, TableClasses::DefaultCellClass);
      const TableBBCode code("table", classes);

      std::string text = "[table][tr=\"class: grid\"][td]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      const std::string expected = "<table>"
         + std::string("<tr class=\"") + TableClasses::DefaultRowClass + "\">"
         + "<td class=\"" + TableClasses::DefaultCellClass + "\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("table with border - border style gets applied to child elements, too")
    {
      std::string text = "[table=\"class: grid\"][tr][td]Content goes here.[/td][/tr][/table]";
      table.applyToText(text);
      const std::string expected = "<table style=\"border: 1px solid #000000; border-collapse: collapse;\">"
         + std::string("<tr style=\"border: 1px solid #000000; border-collapse: collapse;\">")
         + "<td style=\"border: 1px solid #000000; border-collapse: collapse;\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("table with border using custom table, row and cell class")
    {
      const TableClasses classes(true, "my_table_class_name", "my_row_class_name", "my_cell_class_name");
      const TableBBCode code("table", classes);

      std::string text = "[table=\"class: grid\"][tr][td]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      const std::string expected = "<table class=\"my_table_class_name\">"
         + std::string("<tr class=\"my_row_class_name\">")
         + "<td class=\"my_cell_class_name\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }

    SECTION("table with border using default table, row and cell class")
    {
      const TableClasses classes(true, TableClasses::DefaultTableClass, TableClasses::DefaultRowClass, TableClasses::DefaultCellClass);
      const TableBBCode code("table", classes);

      std::string text = "[table=\"class: grid\"][tr][td]Content goes here.[/td][/tr][/table]";
      code.applyToText(text);
      const std::string expected = "<table class=\"" + TableClasses::DefaultTableClass + "\">"
         + std::string("<tr class=\"") + TableClasses::DefaultRowClass + "\">"
         + "<td class=\"" + TableClasses::DefaultCellClass + "\">Content goes here.</td></tr></table>";
      REQUIRE( text == expected );
    }
  }

  SECTION("nested tables")
  {
    SECTION("nested tables, one cell only for each")
    {
      std::string text = "[table][tr][td]content[table][tr][td]inner text[/td][/tr][/table]more content[/td][/tr][/table]";
      table.applyToText(text);
      REQUIRE( text == "<table><tr><td>content<table><tr><td>inner text</td></tr></table>more content</td></tr></table>" );
    }

    SECTION("nested tables, but BB code for outer table is incomplete")
    {
      std::string text = "[table][tr][td]content[table][tr][td]inner text[/td][/tr][/table]more content[/td][/tr] no table end tag";
      table.applyToText(text);
      // Since BB code for outer table is incomplete only the inner table gets translated into HTML.
      REQUIRE( text == "[table][tr][td]content<table><tr><td>inner text</td></tr></table>more content[/td][/tr] no table end tag" );
    }
  }
}
