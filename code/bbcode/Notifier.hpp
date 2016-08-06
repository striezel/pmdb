/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012  Dirk Stolle

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

#ifndef BBCODE_NOTIFIER_HPP
#define BBCODE_NOTIFIER_HPP

#include <iostream>

/** \brief notifier class for printing and/or logging messages */
struct DefaultNotifier
{
  /** \brief puts the passed message string to standard output
   *
   * \param str the message
   */
  static void put(const std::string& str)
  {
    std::cout << str;
  }
}; //struct

#endif // BBCODE_NOTIFIER_HPP
