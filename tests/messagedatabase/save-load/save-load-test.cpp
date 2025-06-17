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

#include <iostream>
#include "../../../code/MessageDatabase.hpp"

/* This program performs some tests for MessageDatabase::saveMessages() and
   MessageDatabase::loadMessages(), but with plain files without compression.

   Expected command line parameters: 1
     1st: path to the temporary directory that shall be used to save and load
          the private message database
*/

int main(int argc, char **argv)
{
  std::string tempDir;
  if ((argc == 2) && (argv != nullptr))
  {
    if (argv[1] != nullptr)
    {
      tempDir = std::string(argv[1]);
    }//parameter exists
    else
    {
      std::cout << "Parameter at index 1 is NULL.\n";
      return 1;
    }
  }//if arguments present
  else
  {
    std::cout << "Error: This program needs the path of a temporary directory as parameter!\n";
    return 1;
  }


  MessageDatabase mdb;

  // some random content
  PrivateMessage pm;
  pm.setDatestamp("2007-06-14 12:34");
  pm.setTitle("This is the title");
  pm.setFromUser("Hermes");
  pm.setFromUserID(234);
  pm.setToUser("Poseidon");
  pm.setMessage("This is a message.");

  // add some PMs to the database
  const unsigned int limit = 2000;
  try
  {
    for (unsigned int i = 1; i <= limit; ++i)
    {
      // change user ID to get a "different" message
      pm.setFromUserID(i);
      // database should accept message
      if (!mdb.addMessage(pm))
      {
        std::cerr << "Error: Could not add message with user ID " << i << " to DB!\n";
        return 1;
      }
    } // for
  }
  catch (...)
  {
    std::cerr << "Error: Caught exception while trying to add messages!\n";
    return 1;
  }
  std::cout << "Test info: Added " << limit << " messages successfully!\n";


  if (!mdb.saveMessages(tempDir, Compression::none))
  {
    std::cerr << "Error: Could not save messages to \"" << tempDir << "\"!\n";
    return 1;
  }
  uint32_t readMessages = 0, newMessages = 0;
  try
  {
    if (!mdb.loadMessages(tempDir, readMessages, newMessages, Compression::none))
    {
      std::cerr << "Error: Could not load messages from \"" << tempDir << "\"!\n";
      return 1;
    }
  }
  catch (...)
  {
    std::cerr << "Error: Caught exception while trying to load messages!\n";
    return 1;
  }
  if ((readMessages != limit) || (newMessages != 0))
  {
    std::cerr << "Error: Unexpected message count!\n"
              << "Expected " << limit << " messages in total (got "
              << readMessages << ") and zero new messages (got "
              << newMessages << ").\n";
    return 1;
  }
  // now clear and do the same again
  mdb.clear();
  try
  {
    if (!mdb.loadMessages(tempDir, readMessages, newMessages, Compression::none))
    {
      std::cerr << "Error: Could not load messages from \"" << tempDir << "\" a second time!\n";
      return 1;
    }
  }
  catch (...)
  {
    std::cerr << "Error: Caught exception while trying to load messages a second time!\n";
    return 1;
  }
  if ((readMessages != limit) || (newMessages != limit))
  {
    std::cout << "Error: Unexpected message count!\n"
              << "Expected " << limit << " messages in total (got "
              << readMessages << ") and " << limit << " new messages (got "
              << newMessages << ").\n";
    return 1;
  }

  std::cout << "Passed all MessageDatabase::saveMessages()/MessageDatabase::loadMessages() tests with plain/uncompressed data files.\n";
  return 0;
}
