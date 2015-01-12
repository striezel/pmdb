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

#include <cstdio> //for tempnam()
#include <cstdlib> //for free()
#include <fstream>
#include <iostream>
#include <unistd.h> //for unlink()
#include "../../MsgTemplate.hpp"

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
  {
    char* tempFileName = tempnam("/tmp", "msg");
    if (tempFileName == NULL)
    {
      std::cout << "Could not get temporary file name!\n";
      return 1;
    } //if
    tempName = std::string(tempFileName);
    free(tempFileName);
  }

  // ---- scope for output file stream
  {
    std::ofstream outFile;
    outFile.open(tempName.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!outFile.is_open())
    {
      std::cout << "Could not create temporary file \"" << tempName << "\"!\n";
      std::cout.flush();
      return 1;
    }
    outFile.write(cTemplateString.c_str(), cTemplateString.length());
    if (!outFile.good())
    {
      std::cout << "Could not write to temporary file \"" << tempName << "\"!\n";
      std::cout.flush();
      outFile.close();
      return 1;
    }
    outFile.close();
  } //end of outFile scope

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
