/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014  Thoronador

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

#ifndef PMSOURCE_HPP
#define PMSOURCE_HPP

#include "PrivateMessage.hpp"
#include "../libthoro/common/BufferStream.hpp"
#include "../libthoro/hash/sha-256_sources.h"

namespace SHA256
{

/** class that uses a PrivateMessage class instance as message source
 *  for SHA-256 hash calculation
 */
class PMSource: public MessageSource
{
  public:
    /** constructor */
    PMSource(const PrivateMessage& pm);


    /** destructor */
    virtual ~PMSource();


    /** \brief puts the next message block from the source in mBlock
     *
     * \param mBlock   reference to the message blocked that should be filled
     * \return Returns true, if there is at least one more message block.
     * Returns false and leaves mBlock unchanged, if there are no more
     * message blocks.
     */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    libthoro::BufferStream m_BufStream;
}; //class

} //namespace

#endif // PMSOURCE_HPP
