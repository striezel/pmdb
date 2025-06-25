/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014, 2015, 2025  Dirk Stolle

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

#include "functions.hpp"
#include <algorithm>
#include <iostream>

void showFilteredMessages(const MessageDatabase& mdb, const FolderMap& fm, const std::vector<FilterUser>& filters)
{
  std::vector<SortType> matches;
  MessageDatabase::Iterator msgIter = mdb.getBegin();
  while (msgIter != mdb.getEnd())
  {
    for (const auto& filter: filters)
    {
      if (filter.match(msgIter->second))
      {
        matches.push_back( md_date(msgIter->second.getDatestamp(), msgIter->first));
        break;
      }
    }
    ++msgIter;
  }

  std::sort(matches.begin(), matches.end());
  std::cout << "Filtered messages:\n";
  for (const auto& match: matches)
  {
    const PrivateMessage & pm = mdb.getMessage(match.md);
    std::cout << "Message \"" << pm.getTitle() << "\" of " << pm.getDatestamp();
    if (fm.hasEntry(match.md))
    {
      std::cout << " in \"" << fm.getFolderName(match.md) << "\"";
    }
    std::cout << std::endl;
  }
  if (matches.empty())
    std::cout << "  no matches\n";
  else
    std::cout << "Total: " << matches.size() << "\n";
}
