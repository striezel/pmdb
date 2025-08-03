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

    SECTION("quote with name in single quotes")
    {
      const std::string text = "Blah, blah.\n[quote='[W]orld']Hello![/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>[W]orld</strong>\n"
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

    SECTION("quote with name in and post number in single quotes")
    {
      const std::string text = "Blah, blah.\n[quote='[W]orld;21973207']Hello![/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "      <td class=\"alt2\" style=\"border:1px inset\">\n"
                                 + "        <div>\n"
                                 + "            Zitat von <strong>[W]orld</strong>\n"
                                 + "            <a href=\"https://for.um/showthread.php?p=21973207#post21973207"
                                 + "\" rel=\"nofollow\"><img class=\"inlineimg\" src=\"img/buttons/viewpost.gif\" border=\"0\" alt=\"Beitrag anzeigen\"></a>\n"
                                 + "        </div>\n"
                                 + "        <div style=\"font-style:italic\">"
                                 + "Hello!"
                                 + "</div>\n      </td>\n    </tr>\n"
                                 + "    </table>\n  </td></tr>\n</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: simple")
    {
      const std::string text = "Blah, blah.\n[quote]This is a quote.\n[quote]Another quote.[/quote][/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "This is a quote.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "Another quote."
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: simple outer quote, inner quote with name")
    {
      const std::string text = "Blah, blah.\n[quote]This is a quote.\n[quote=Author]Another quote.[/quote][/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "This is a quote.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>Author</strong>\n"
                                 + "           </div>\n"
                                 + "           <div style=\"font-style:italic\">"
                                 + "Another quote."
                                 + "</div>\n"
                                 + "        </td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: simple outer quote, inner quote with name and post number")
    {
      const std::string text = "Blah, blah.\n[quote]This is a quote.\n[quote=Author;987654321]Another quote.[/quote][/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "This is a quote.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "      <td class=\"alt2\" style=\"border:1px inset\">\n"
                                 + "        <div>\n"
                                 + "            Zitat von <strong>Author</strong>\n"
                                 + "            <a href=\"https://for.um/showthread.php?p=987654321#post987654321"
                                 + "\" rel=\"nofollow\"><img class=\"inlineimg\" src=\"img/buttons/viewpost.gif\" border=\"0\" alt=\"Beitrag anzeigen\"></a>\n"
                                 + "        </div>\n"
                                 + "        <div style=\"font-style:italic\">"
                                 + "Another quote."
                                 + "</div>\n      </td>\n    </tr>\n"
                                 + "    </table>\n  </td></tr>\n</table>"
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: outer quote with name, simple inner quote")
    {
      const std::string text = "Blah, blah.\n[quote=Alice]This is a quote.\n[quote]Another quote.[/quote][/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>Alice</strong>\n"
                                 + "           </div>\n"
                                 + "           <div style=\"font-style:italic\">"
                                 + "This is a quote.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "Another quote."
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "</div>\n"
                                 + "        </td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: outer quote with name and post number, simple inner quote")
    {
      const std::string text = "Blah, blah.\n[quote=Alice;42]This is a quote.\n[quote]Another quote.[/quote][/quote]\nMore blah.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Blah, blah.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "      <td class=\"alt2\" style=\"border:1px inset\">\n"
                                 + "        <div>\n"
                                 + "            Zitat von <strong>Alice</strong>\n"
                                 + "            <a href=\"https://for.um/showthread.php?p=42#post42"
                                 + "\" rel=\"nofollow\"><img class=\"inlineimg\" src=\"img/buttons/viewpost.gif\" border=\"0\" alt=\"Beitrag anzeigen\"></a>\n"
                                 + "        </div>\n"
                                 + "        <div style=\"font-style:italic\">"
                                 + "This is a quote.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">"
                                 + "Another quote."
                                 + "</td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "</div>\n      </td>\n    </tr>\n"
                                 + "    </table>\n  </td></tr>\n</table>"
                                 + "\nMore blah.";
      REQUIRE( with_quotes == expected );
    }

    SECTION("nested quote: outer and inner quote with name")
    {
      const std::string text = "Yo dawg.\n[quote=Dawg]I heard you like quotes.\n[quote=Us]So we put a quote in your quote.[/quote][/quote]\nSo you can quote while you quote.";
      const auto with_quotes = handleQuotes(text, forum_url);
      const std::string expected = std::string("Yo dawg.\n")
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>Dawg</strong>\n"
                                 + "           </div>\n"
                                 + "           <div style=\"font-style:italic\">"
                                 + "I heard you like quotes.\n"
                                 + "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                                 + "  <tr><td>\n"
                                 + "    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
                                 + "    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
                                 + "    <tr>\n"
                                 + "        <td style=\"border:1px inset\">\n"
                                 + "            <div>\n"
                                 + "                Zitat von <strong>Us</strong>\n"
                                 + "           </div>\n"
                                 + "           <div style=\"font-style:italic\">"
                                 + "So we put a quote in your quote."
                                 + "</div>\n"
                                 + "        </td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "</div>\n"
                                 + "        </td>\n"
                                 + "    </tr>\n"
                                 + "    </table>\n"
                                 + "</td></tr>\n"
                                 + "</table>"
                                 + "\nSo you can quote while you quote.";
      REQUIRE( with_quotes == expected );
    }
  }
}
