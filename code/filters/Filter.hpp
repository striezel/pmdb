/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014, 2025  Dirk Stolle

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

#ifndef FILTER_HPP
#define FILTER_HPP

#include "../PrivateMessage.hpp"

/// abstract base class for filters
class Filter
{
  public:
      virtual ~Filter() = default;


      /** \brief Checks, if a given PM matches the filter criterion.
       *
       * \param pm  reference to the private message
       * \return Returns true, if criterion is fulfilled. Returns false otherwise.
       */
      virtual bool match(const PrivateMessage& pm) const = 0;
}; // class

#endif // FILTER_HPP
