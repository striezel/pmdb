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

#include "../../locate_catch.hpp"
#include "../../../code/filters/FilterUser.hpp"

TEST_CASE("FilterUser")
{
  PrivateMessage msg;

  SECTION("recipient matching")
  {
    SECTION("full")
    {
      msg.setToUser("Uh Ser");

      // Exact name gives a match.
      FilterUser filter("Uh Ser", UserType::Recipient, Match::FullName);
      REQUIRE( filter.match(msg) );
      // Partial match does not count.
      filter = FilterUser("Uh", UserType::Recipient, Match::FullName);
      REQUIRE_FALSE( filter.match(msg) );
      filter = FilterUser("Uh Ser Name", UserType::Recipient, Match::FullName);
      REQUIRE_FALSE( filter.match(msg) );
    }

    SECTION("partial")
    {
      msg.setToUser("Uh Ser");

      // Exact name gives a match.
      FilterUser filter("Uh Ser", UserType::Recipient, Match::PartialName);
      REQUIRE( filter.match(msg) );
      // Partial match does count, too.
      filter = FilterUser("Uh", UserType::Recipient, Match::PartialName);
      REQUIRE( filter.match(msg) );
      // But not if name in filter criterion is longer.
      filter = FilterUser("Uh Ser Name", UserType::Recipient, Match::PartialName);
      REQUIRE_FALSE( filter.match(msg) );
    }
  }

  SECTION("sender matching")
  {
    SECTION("full")
    {
      msg.setFromUser("Uh Ser");

      // Exact name gives a match.
      FilterUser filter("Uh Ser", UserType::Sender, Match::FullName);
      REQUIRE( filter.match(msg) );
      // Partial match does not count.
      filter = FilterUser("Uh", UserType::Sender, Match::FullName);
      REQUIRE_FALSE( filter.match(msg) );
      filter = FilterUser("Uh Ser Name", UserType::Sender, Match::FullName);
      REQUIRE_FALSE( filter.match(msg) );
    }

    SECTION("partial")
    {
      msg.setFromUser("Uh Ser");

      // Exact name gives a match.
      FilterUser filter("Uh Ser", UserType::Sender, Match::PartialName);
      REQUIRE( filter.match(msg) );
      // Partial match does count, too.
      filter = FilterUser("Uh", UserType::Sender, Match::PartialName);
      REQUIRE( filter.match(msg) );
      // But not if name in filter criterion is longer.
      filter = FilterUser("Uh Ser Name", UserType::Sender, Match::PartialName);
      REQUIRE_FALSE( filter.match(msg) );
    }
  }
}
