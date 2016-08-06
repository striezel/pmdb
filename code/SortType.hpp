/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2013, 2014  Dirk Stolle

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

#ifndef SORTTYPE_HPP
#define SORTTYPE_HPP

#include <string>
#include "../libstriezel/hash/sha256/sha256.hpp"

/** aux. type for sorting entries by datestamp and hash */
struct SortType
{
  std::string datestamp;
  SHA256::MessageDigest md;


  /** \brief constructor
   *
   * \param ds datestamp value
   * \param dig SHA-256 message digest
   */
  SortType(const std::string& ds, const SHA256::MessageDigest& dig);


  /** \brief constructor with alternative argument order
   *
   * \param dig SHA-256 message digest
   * \param ds datestamp value
   */
  SortType(const SHA256::MessageDigest& dig, const std::string& ds);
};//struct


/** \brief comparison operator "less than" for SortType
 *
 * \param a first/left-hand SortType value
 * \param b second/right-hand SortType value
 * \return Returns true, if a < b.
 */
bool operator<(const SortType& a, const SortType& b);


/** \brief comparison function "greater than" for SortType
 *
 * \param __x first/left-hand SortType
 * \param __y second/right-hand SortType
 * \return Returns true, if __x > __y.
 */
bool ST_greater(const SortType& __x, const SortType& __y);


///alias for SortType
typedef SortType md_date;

#endif // SORTTYPE_HPP
