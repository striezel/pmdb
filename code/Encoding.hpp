/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2015  Thoronador

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

#ifndef ENCODING_HPP
#define ENCODING_HPP

#include <string>

class Encoding
{
  public:
    static bool UTF8_to_ISO_8859_1(const std::string& strUTF8, std::string& strISO);

    static bool ISO_8859_1_to_UTF8(const std::string& strISO, std::string& strUTF8);
}; //class

#endif // ENCODING_HPP
