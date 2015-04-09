/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Thoronador

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
#include <iostream>
#include "../../../code/MessageDatabase.hpp"

// tests for MessageDatabase::addMessage()

int main()
{
  //empty database
  {
    MessageDatabase mdb;

    if (mdb.getNumberOfMessages() != 0)
    {
      std::cout << "Error: Empty DB has more than zero messages!\n";
      return 1;
    }
  } //end of scope for empty DB

  {
    MessageDatabase mdb;

    //some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    try
    {
      //database should accept message
      if (!mdb.addMessage(pm))
      {
        std::cout << "Error: Could not add message to DB!\n";
        return 1;
      }
      //But database should not accept same message twice!
      if (mdb.addMessage(pm))
      {
        std::cout << "Error: DB accepted same message twice!\n";
        return 1;
      }
      //DB should say it has that message.
      if (!mdb.hasMessage(pm))
      {
        std::cout << "Error: Database can not keep track of messages!\n";
        return 1;
      }
      if (mdb.getNumberOfMessages() != 1)
      {
        std::cout << "Error: DB has more than one message!\n";
        return 1;
      }
    } //try
    catch (std::exception& except)
    {
      std::cout << "Error: Caught an exception, you managed to mess things up!\n"
                << "What: " << except.what() << "\n";
    }
  } //end of scope

  //scope for more than one message
  {
    MessageDatabase mdb;

    //some random content
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

    try
    {
      //database should accept message
      if (!mdb.addMessage(pm))
      {
        std::cout << "Error: Could not add message to DB!\n";
        return 1;
      }
      //database should accept 2nd message
      if (!mdb.addMessage(secondPM))
      {
        std::cout << "Error: Could not add 2nd message to DB!\n";
        return 1;
      }

      //DB should say it has the first message.
      if (!mdb.hasMessage(pm))
      {
        std::cout << "Error: Database can not keep track of messages!\n";
        return 1;
      }
      //DB should say it has the second message.
      if (!mdb.hasMessage(secondPM))
      {
        std::cout << "Error: Database can not keep track of messages!\n";
        return 1;
      }
      if (mdb.getNumberOfMessages() != 2)
      {
        std::cout << "Error: DB does not have two messages!\n";
        return 1;
      }
    } //try
    catch (std::exception& except)
    {
      std::cout << "Error: Caught an exception, you managed to mess things up!\n"
                << "What: " << except.what() << "\n";
    }
  } //end of scope for two messages

  // scope for adding multiple PMs
  {
    MessageDatabase mdb;

    //some random content
    PrivateMessage pm;
    pm.setDatestamp("2007-06-14 12:34");
    pm.setTitle("This is the title");
    pm.setFromUser("Hermes");
    pm.setFromUserID(234);
    pm.setToUser("Poseidon");
    pm.setMessage("This is a message.");

    const unsigned int limit = 200000;
    try
    {
      unsigned int i;
      for (i=1; i <= limit; ++i)
      {
        //change user ID to get a "different" message
        pm.setFromUserID(i);
        //database should accept message
        if (!mdb.addMessage(pm))
        {
          std::cout << "Error: Could not add message with user ID " << i << " to DB!\n";
          return 1;
        }
        //database should know it has that message
        if (!mdb.hasMessage(pm))
        {
          std::cout << "Error: Database can not keep track of " << i << " (or more) messages!\n";
          return 1;
        }
        //Database should NOT accept the message twice.
        if (mdb.addMessage(pm))
        {
          std::cout << "Error: Could add message with user ID " << i << " to DB - twice!\n";
          return 1;
        }
      } //for
      if (mdb.getNumberOfMessages() != limit)
      {
        std::cout << "Error: DB does not have " << limit << " messages as expected!\n";
        return 1;
      }
    } //try
    catch (std::exception& except)
    {
      std::cout << "Error: Caught an exception, you managed to mess things up!\n"
                << "What: " << except.what() << "\n";
    }

    std::cout << "Test info: Added " << limit << " messages successfully!\n";
  } //end of scope for multiple PMs

  std::cout << "Passed all MessageDatabase::addMessage() tests.\n";
  return 0;
}
