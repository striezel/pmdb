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

#include <iostream>
#include "../../../code/PrivateMessage.hpp"
#include "../../../libthoro/filesystem/FileFunctions.hpp"

/* This program test whether a PrivateMessage can be saved to a file (with
   compressing the data) and be loaded from that file (with decompressing the
   data) and whether the loaded data is the same as the saved data. */

int main()
{
  PrivateMessage pmOne;
  pmOne.setDatestamp("2007-06-14 12:34");
  pmOne.setTitle("This is the title");
  pmOne.setFromUser("Hermes");
  pmOne.setFromUserID(234);
  pmOne.setToUser("Poseidon");
  pmOne.setMessage(std::string("Lorem ipsum dolor sit amet,\n")
                              +"consetetur sadipscing elitr,\n"
                              +"sed diam nonumy eirmod tempor\n"
                              +"invidunt ut labore et dolore magna aliquyam erat,\n"
                              +"sed diam voluptua.\n"
                              +"\n"
                              +"At vero eos et accusam et justo duo dolores et ea rebum.\n"
                              +"Stet clita kasd gubergren,\n"
                              +"no sea takimata sanctus est Lorem ipsum dolor sit amet.");

  //get temporary file name for saving message
  std::string tempName("");
  if (!libthoro::filesystem::File::createTemp(tempName))
  {
    std::cout << "Error: Failed to create temporary file!\n";
    return 1;
  }
  //save message (uncompressed)
  if (!pmOne.saveToFile(tempName, true))
  {
    std::cout << "Error: Could not save PM with compression to " << tempName << "!\n";
    if (!libthoro::filesystem::File::remove(tempName))
    {
      std::cout << "Additionally, the temporary file " << tempName
                << "could not be removed.\n";
    }
    return 1;
  }

  //second PM instance for loading the file (uncompressed)
  PrivateMessage pmTwo;
  if (!pmTwo.loadFromFile(tempName, true))
  {
    std::cout << "Error: Could not load compressed PM from " << tempName << "!\n";
    if (!libthoro::filesystem::File::remove(tempName))
    {
      std::cout << "Additionally, the temporary file " << tempName
                << "could not be removed.\n";
    }
    return 1;
  }

  //remove temporary file, no  longer needed
  if (!libthoro::filesystem::File::remove(tempName))
  {
    std::cout << "Hint: The temporary file " << tempName
              << "could not be removed.\n";
  }

  //check for inequalities
  if (pmTwo != pmOne)
  {
    std::cout << "Error: The loaded PM is not equal to the saved PM!\n";
    return 1;
  }
  // ...and use equality operator, too!
  if (pmTwo == pmOne)
  {
    std::cout << "Info: Loaded and saved PM are equal.\n";
  }
  else
  {
    std::cout << "Error: The loaded PM is not equal to the saved PM!\n";
    return 1;
  }
  //just to be on the safe side, check hashes, too
  try
  {
    if (pmTwo.getHash() != pmOne.getHash())
    {
      std::cout << "Error: The loaded PM's hash is not equal to the saved PM's hash!\n";
      return 1;
    }
  }
  catch (...)
  {
    std::cout << "Error: Caught an exception while calculating SHA-256 hashes!\n";
    return 1;
  }

  std::cout << "Info: Passed simple test for PrivateMessage::saveToFile() and PrivateMessage::loadFromFile() with compression!\n";
  return 0;
}