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

#include <cstdlib> //for mkstemp()
#include <fstream>
#include <iostream>
#include <unistd.h> //for close(), unlink(), write()
#include "../../code/MsgTemplate.hpp"

int main()
{
  const std::string cTemplateString = "foo {..bar..} stuff";

  MsgTemplate tpl_fromString;
  MsgTemplate tpl_fromFile;

  //load template from string
  if (!tpl_fromString.loadFromString(cTemplateString))
  {
    std::cout << "Error: could not 'load' template from string.\n";
    return 1;
  }

  //load other template from file
  // ---- get temporary file name
  std::string tempName = "";

  //mkstemp scope
  {
    char tempFileName[] = "/tmp/msgXXXXXXXXXX";
    const int fd = mkstemp(tempFileName);
    if (fd < 0) //-1 signals error
    {
      std::cout << "Could not get temporary file name!\n";
      unlink(tempFileName);
      return 1;
    }
    tempName = std::string(tempFileName);
    const ssize_t writeResult = write(fd, cTemplateString.c_str(), cTemplateString.size());
    close(fd);
    if (writeResult < 0) //-1 signals error, errno will be set
    {
      std::cout << "Could not write to temporary file \"" << tempName << "\"!\n";
      std::cout.flush();
      return 1;
    }
    if (writeResult != cTemplateString.size())
    {
      std::cout << "Could not write complete test template to temporary file \"" << tempName << "\"!\n";
      std::cout.flush();
      return 1;
    }
  } //end of mkstemp() scope

  const bool loadSuccess = tpl_fromFile.loadFromFile(tempName);
  unlink(tempName.c_str());
  if (!loadSuccess)
  {
    std::cout << "Could load template from temporary file \"" << tempName << "\"!\n";
    std::cout.flush();
    return 1;
  }

  //both templates should now produce the same output, when show() is called
  if (tpl_fromFile.show()!=tpl_fromString.show())
  {
    std::cout << "Templates do not match!\n";
    return 1;
  }
  //templates match :)
  return 0;
}
