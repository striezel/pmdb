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

#ifndef PMSOURCE_H
#define PMSOURCE_H

#include "PrivateMessage.hpp"
#include "libthoro/common/BufferStream.h"
#include "libthoro/hashfunctions/sha-256_sources.h"

namespace SHA256
{

class PMSource: public MessageSource
{
  public:
    /* constructor */
    PMSource(const PrivateMessage& pm);

    /* destructor */
    virtual ~PMSource();

    /* puts the next message block from the source in mBlock and returns true,
       if there is at least one more message block. Returns false and leaves
       mBlock unchanged, if there are no more message blocks.

       parameters:
           mBlock - reference to the message blocked that should be filled
    */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    Thoro::BufferStream m_BufStream;
}; //class

} //namespace

#endif // PMSOURCE_H
