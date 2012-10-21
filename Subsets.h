/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012  Thoronador

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

#ifndef SUBSETS_H
#define SUBSETS_H

#include <vector>
#include "MessageDatabase.h"

std::map<SHA256::MessageDigest, std::vector<SHA256::MessageDigest> > getTextSubsets(const MessageDatabase& mdb)
{
  std::map<SHA256::MessageDigest, std::vector<SHA256::MessageDigest> > result;
  MessageDatabase::Iterator iter = mdb.getBegin();
  while (iter!=mdb.getEnd())
  {
    MessageDatabase::Iterator innerIter = mdb.getBegin();
    while (innerIter!=mdb.getEnd())
    {
      if (iter!=innerIter)
      {
        if (iter->second.getMessage().find(innerIter->second.getMessage())!=std::string::npos)
        {
          result[iter->first].push_back(innerIter->first);
        }
      }
      ++innerIter;
    }
    ++iter;
  }//while
  return result;
}

#endif // SUBSETS_H
