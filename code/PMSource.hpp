/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014  Dirk Stolle

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
#include "../libstriezel/common/BufferStream.hpp"
#include "../libstriezel/hash/sha256/MessageSource.hpp"

namespace SHA256
{

/** Class that uses a PrivateMessage class instance as message source
 *  for SHA-256 hash calculation.
 */
class PMSource: public MessageSource
{
  public:
    /** constructor */
    PMSource(const PrivateMessage& pm);


    /** \brief Puts the next message block from the source in mBlock.
     *
     * \param mBlock   reference to the message blocked that should be filled
     * \return Returns true, if there is at least one more message block.
     * Returns false and leaves mBlock unchanged, if there are no more
     * message blocks.
     */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    libstriezel::InBufferStream m_BufStream;
}; // class

} // namespace

#endif // PMSOURCE_HPP
