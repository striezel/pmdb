/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2013, 2014  Thoronador

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

#include "SortType.hpp"

SortType::SortType(const std::string& ds, const SHA256::MessageDigest& dig)
: datestamp(ds), md(dig)
{
}

SortType::SortType(const SHA256::MessageDigest& dig, const std::string& ds)
: datestamp(ds), md(dig)
{
}

bool operator<(const SortType& a, const SortType& b)
{
  const int cmp1 = a.datestamp.compare(b.datestamp);
  if (cmp1<0) return true;
  if (cmp1>0) return false;
  return (a.md<b.md);
}

bool ST_greater(const SortType& __x, const SortType& __y)
{
  if (__x.datestamp>__y.datestamp) return true;
  if (__x.datestamp<__y.datestamp) return false;
  return __x.md.toHexString() > __y.md.toHexString();
}
