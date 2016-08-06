/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2015  Dirk Stolle

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
#include "../../code/Config.hpp"

int main(void)
{
  Config conf;

  // all stuff should be empty at the beginning
  if (!conf.getForumURL().empty())
  {
    std::cout << "Error: forum URL is not empty!\n";
    return 1;
  }
  if (!conf.getTPL().empty())
  {
    std::cout << "Error: TPL file name is not empty!\n";
    return 1;
  }
  if (!conf.getSmilies().empty())
  {
    std::cout << "Error: smilie list is not empty!\n";
    return 1;
  }
  //all checks successful
  return 0;
}
