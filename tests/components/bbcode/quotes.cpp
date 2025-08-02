/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2025  Dirk Stolle

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
#include "../../../code/bbcode/quotes.hpp"

TEST_CASE("Quotes")
{
  SECTION("getClosingQuoteTagPosition")
  {
    SECTION("empty string")
    {
      const std::string text = "";
      const auto pos = getClosingQuoteTagPosition(text, 0);
      REQUIRE( pos == std::string::npos );
    }

    SECTION("string without quote tag")
    {
      const std::string text = "This is a text without quotes.";
      const auto pos = getClosingQuoteTagPosition(text, 0);
      REQUIRE( pos == std::string::npos );
    }

    SECTION("string quote tag (lower case)")
    {
      const std::string text = "Blah, blah.\n[quote]This is a quote.[/quote]\nMore blah.";
      const auto pos = getClosingQuoteTagPosition(text, 0);
      REQUIRE( pos == 35 );
    }

    SECTION("string quote tag (upper case)")
    {
      const std::string text = "Blah, blah.\n[QUOTE]This is a quote.[/QUOTE]\nMore blah.";
      const auto pos = getClosingQuoteTagPosition(text, 0);
      REQUIRE( pos == 35 );
    }
  }

  SECTION("handleQuotes")
  {
    const std::string forum_url = "https://for.um/";

    SECTION("empty text")
    {
      const std::string text = "";
      const auto with_quotes = handleQuotes(text, forum_url);
      REQUIRE( text == with_quotes );
    }

    SECTION("text without quotes")
    {
      const std::string text = "Blah, blah.\nThere is no quote.\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      REQUIRE( text == with_quotes );
    }

    SECTION("simple quote")
    {
      const std::string text = "Blah, blah.\n[quote]This is a quote.[/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "This is a quote."
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("quote with name")
    {
      const std::string text = "Blah, blah.\n[quote=World]Hello![/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>World</strong>\n"
                                 + "           </div>\n"
                                 + "           <div style=\"font-style:italic\">"
                                 + "Hello!"
                                 + "</div>\n"
                                 + "        </td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("quote with name and post number")
    {
      const std::string text = "Blah, blah.\n[quote=World;123654789]Hello![/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "      <td class=\"alt2\" style=\"border:1px inset\">\n"
                                 + "        <div>\n"
                                 + "            Zitat von <strong>World</strong>\n"
                                 + "            <a href=\"https://for.um/showthread.php?p=123654789#post123654789"
                                 + "\" rel=\"nofollow\"><img class=\"inlineimg\" src=\"img/buttons/viewpost.gif\" border=\"0\" alt=\"Beitrag anzeigen\"></a>\n"
                                 + "        </div>\n"
                                 + "        <div style=\"font-style:italic\">"
                                 + "Hello!"
                                 + "</div>\n      </td>\n    </tr>\n"
                                 + "    </table>\n  </td></tr>\n</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }
  }
}
