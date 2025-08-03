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

#include "../../locate_catch.hpp"
#include "../../../code/bbcode/BBCodeParser.hpp"
#include "../../../code/bbcode/SimpleBBCode.hpp"

struct B_To_C: public TextProcessor
{
  public:
    virtual void applyToText(std::string& text) const
    {
      std::string::size_type pos = text.find("b]");
      while (pos != std::string::npos)
      {
        text.replace(pos, 1, "c");
        pos = text.find("b]", (pos == 0) ? 0 : pos - 1);
      }
    }
}; // struct

struct Exchange: public TextProcessor
{
  public:
    virtual void applyToText(std::string& text) const
    {
      std::string::size_type pos = text.find(">exchanging");
      while (pos != std::string::npos)
      {
        text.erase(pos + 1, 2);
        pos = text.find(">exchanging", pos + 5);
      }
    }
}; // struct

TEST_CASE("BBCodeParser")
{
  const std::string forum_url = "https://for.um/";

  SECTION("bb code with n2lbr in HTML 4.01")
  {
    const SimpleBBCode code("b");
    BBCodeParser parser;
    parser.addCode(&code);

    const std::string input = "[b]bold[/b]\ntext";
    const std::string expected = "<b>bold</b><br>\ntext";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code with n2lbr in XHTML")
  {
    const SimpleBBCode code("b");
    BBCodeParser parser;
    parser.addCode(&code);

    const std::string input = "[b]bold[/b]\ntext";
    const std::string expected = "<b>bold</b><br />\ntext";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::XHTML, true);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code without n2lbr")
  {
    const SimpleBBCode code("b");
    BBCodeParser parser;
    parser.addCode(&code);

    const std::string input = "[b]bold[/b]\ntext";
    const std::string expected = "<b>bold</b>\ntext";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, false);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code plus pre-processor")
  {
    const SimpleBBCode code("c");
    B_To_C pre_proc;

    BBCodeParser parser;
    parser.addCode(&code);
    parser.addPreProcessor(&pre_proc);

    const std::string input = "[b]exchanging[/b] text";
    const std::string expected = "<c>exchanging</c> text";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code plus smilie in HTML 4.01")
  {
    const SimpleBBCode code("b");
    Smilie smilie(":)", "img/smile.png", UrlType::Relative);
    BBCodeParser parser;
    parser.addCode(&code);
    parser.addSmilie(smilie);

    const std::string input = "[b]smiling[/b] face :)";
    const std::string expected = "<b>smiling</b> face <img src=\"https://for.um/img/smile.png\" alt=\":)\" border=\"0\">";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code plus smilie in XHTML")
  {
    const SimpleBBCode code("b");
    Smilie smilie(":)", "img/smile.png", UrlType::Relative);
    BBCodeParser parser;
    parser.addCode(&code);
    parser.addSmilie(smilie);

    const std::string input = "[b]smiling[/b] face :)";
    const std::string expected = "<b>smiling</b> face <img src=\"https://for.um/img/smile.png\" alt=\":)\" border=\"0\" />";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::XHTML, true);

    REQUIRE( parsed == expected );
  }

  SECTION("bb code plus post-processor")
  {
    const SimpleBBCode code("b");
    Exchange post_proc;

    BBCodeParser parser;
    parser.addCode(&code);
    parser.addPostProcessor(&post_proc);

    const std::string input = "[b]exchanging[/b] text";
    const std::string expected = "<b>changing</b> text";
    const auto parsed = parser.parse(input, forum_url, HTMLStandard::HTML4_01, true);

    REQUIRE( parsed == expected );
  }
}
