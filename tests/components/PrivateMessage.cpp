/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015, 2025  Dirk Stolle

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
#include "../../code/PrivateMessage.hpp"

TEST_CASE("PrivateMessage")
{
  SECTION("constructor")
  {
    PrivateMessage pm;
    REQUIRE( pm.getDatestamp().empty() );
    REQUIRE( pm.getTitle().empty() );
    REQUIRE( pm.getFromUser().empty() );
    REQUIRE( pm.getFromUserID() == 0 );
    REQUIRE( pm.getToUser().empty() );
    REQUIRE( pm.getMessage().empty() );
  }

  SECTION("normalise")
  {
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");

    SECTION("empty message")
    {
      pm.setMessage("");
      pm.normalise();
      REQUIRE( pm.getMessage() == "" );
    }

    SECTION("message without line breaks")
    {
      pm.setMessage("This is some random text.");
      pm.normalise();
      REQUIRE( pm.getMessage() == "This is some random text." );
    }

    SECTION("normal Unix line break - should not change")
    {
      pm.setMessage("\n");
      pm.normalise();
      REQUIRE( pm.getMessage() == "\n" );
    }

    SECTION("normal Windows line break - should be replaced by new line only")
    {
      pm.setMessage("\r\n");
      pm.normalise();
      REQUIRE( pm.getMessage() == "\n" );
    }

    SECTION("text with both kinds of line breaks")
    {
      pm.setMessage("Foo.\nBar.\r\nBoom!");
      pm.normalise();
      REQUIRE( pm.getMessage() == "Foo.\nBar.\nBoom!" );
    }

    SECTION("multiple Unix line breaks - should not change")
    {
      pm.setMessage("\n\n\n\n\n\n\n\n\n\n");
      pm.normalise();
      REQUIRE( pm.getMessage() == "\n\n\n\n\n\n\n\n\n\n" );
    }

    SECTION("multiple Windows-style line breaks - should get converted to Unix style")
    {
      pm.setMessage("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
      pm.normalise();
      REQUIRE( pm.getMessage() == "\n\n\n\n\n\n\n\n" );
    }

    SECTION("example text with Unix line breaks")
    {
      pm.setMessage(std::string("Lorem ipsum dolor sit amet,\n")
                              + "consetetur sadipscing elitr,\n"
                              + "sed diam nonumy eirmod tempor\n"
                              + "invidunt ut labore et dolore magna aliquyam erat,\n"
                              + "sed diam voluptua.\n"
                              + "\n"
                              + "At vero eos et accusam et justo duo dolores et ea rebum.\n"
                              + "Stet clita kasd gubergren,\n"
                              + "no sea takimata sanctus est Lorem ipsum dolor sit amet.");
      pm.normalise();
      const std::string expected = std::string("Lorem ipsum dolor sit amet,\n")
                                 + "consetetur sadipscing elitr,\n"
                                 + "sed diam nonumy eirmod tempor\n"
                                 + "invidunt ut labore et dolore magna aliquyam erat,\n"
                                 + "sed diam voluptua.\n"
                                 + "\n"
                                 + "At vero eos et accusam et justo duo dolores et ea rebum.\n"
                                 + "Stet clita kasd gubergren,\n"
                                 + "no sea takimata sanctus est Lorem ipsum dolor sit amet.";
      REQUIRE( pm.getMessage() == expected );
    }

    SECTION("example text with Windows line breaks")
    {
      pm.setMessage(std::string("Lorem ipsum dolor sit amet,\r\n")
                              + "consetetur sadipscing elitr,\r\n"
                              + "sed diam nonumy eirmod tempor\r\n"
                              + "invidunt ut labore et dolore magna aliquyam erat,\r\n"
                              + "sed diam voluptua.\r\n"
                              + "\r\n"
                              + "At vero eos et accusam et justo duo dolores et ea rebum.\r\n"
                              + "Stet clita kasd gubergren,\r\n"
                              + "no sea takimata sanctus est Lorem ipsum dolor sit amet.");
      pm.normalise();
      const std::string expected = std::string("Lorem ipsum dolor sit amet,\n")
                                 + "consetetur sadipscing elitr,\n"
                                 + "sed diam nonumy eirmod tempor\n"
                                 + "invidunt ut labore et dolore magna aliquyam erat,\n"
                                 + "sed diam voluptua.\n"
                                 + "\n"
                                 + "At vero eos et accusam et justo duo dolores et ea rebum.\n"
                                 + "Stet clita kasd gubergren,\n"
                                 + "no sea takimata sanctus est Lorem ipsum dolor sit amet.";
      REQUIRE( pm.getMessage() == expected );
    }
  }
}
