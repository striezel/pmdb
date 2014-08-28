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
#include "MessageDatabase.hpp"

struct md_date
{
  SHA256::MessageDigest md;
  std::string date;

  md_date(const SHA256::MessageDigest& digest, const std::string& str)
  : md(digest), date(str)
  {}
}; //struct

bool operator<(const md_date& a, const md_date& b)
{
  const int cmp1 = a.date.compare(b.date);
  if (cmp1<0) return true;
  if (cmp1>0) return false;
  return (a.md<b.md);
}

std::map<md_date, std::vector<md_date> > getTextSubsets(const MessageDatabase& mdb)
{
  std::map<md_date, std::vector<md_date> > result;
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
          result[md_date(iter->first, iter->second.getDatestamp())].push_back(md_date(innerIter->first, innerIter->second.getDatestamp()));
        }
      }
      ++innerIter;
    }
    ++iter;
  }//while
  return result;
}

#endif // SUBSETS_H
