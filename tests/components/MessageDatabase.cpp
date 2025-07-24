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

#include <exception>
#include "../locate_catch.hpp"
#include "../../code/MessageDatabase.hpp"

// tests for MessageDatabase::addMessage()

TEST_CASE("MessageDatabase")
{
  SECTION("empty database")
  {
    MessageDatabase mdb;

    REQUIRE( mdb.getNumberOfMessages() == 0 );
  }

  SECTION("single message")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    // database should accept message
    REQUIRE( mdb.addMessage(pm) );
    // But database should not accept same message twice!
    REQUIRE_FALSE( mdb.addMessage(pm) );
    // DB should say it has that message.
    REQUIRE( mdb.hasMessage(pm) );
    REQUIRE( mdb.getNumberOfMessages() == 1 );
  }

  SECTION("more than one message")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    PrivateMessage secondPM;
    secondPM.setDatestamp("2007-06-14 12:34");
    secondPM.setTitle("This is the title");
    secondPM.setFromUser("Mr. A");
    secondPM.setFromUserID(567890);
    secondPM.setToUser("Mrs. B");
    secondPM.setMessage("This is another message.");

    // database should accept message
    REQUIRE( mdb.addMessage(pm) );
    // database should accept 2nd message
    REQUIRE( mdb.addMessage(secondPM) );

    // DB should say it has the first message.
    REQUIRE( mdb.hasMessage(pm) );
    // DB should say it has the second message.
    REQUIRE( mdb.hasMessage(secondPM) );

    REQUIRE( mdb.getNumberOfMessages() == 2 );
  }

  SECTION("adding multiple PMs")
  {
    MessageDatabase mdb;

    // some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    constexpr uint32_t limit = 200000;

    for (uint32_t i = 1; i <= limit; ++i)
    {
      // change user ID to get a "different" message
      pm.setFromUserID(i);
      // database should accept message
      REQUIRE( mdb.addMessage(pm) );
      // database should know it has that message
      REQUIRE( mdb.hasMessage(pm) );
      // Database should NOT accept the message twice.
      REQUIRE_FALSE( mdb.addMessage(pm) );
    } //for
    REQUIRE( mdb.getNumberOfMessages() == limit );
  } //end of scope for multiple PMs
}
