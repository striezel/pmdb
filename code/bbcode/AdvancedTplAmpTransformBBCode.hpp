/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015  Dirk Stolle

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

#ifndef ADVANCEDTPLAMPTRANSFORMBBCODE_HPP
#define ADVANCEDTPLAMPTRANSFORMBBCODE_HPP

#include "AdvancedTemplateBBCode.hpp"

/* struct AdvancedTplAmpTransformBBCode:
       like AdvancedTemplateBBCode, but "&" in attribute value will be replaced
       with "&amp;"
*/
struct AdvancedTplAmpTransformBBCode: public AdvancedTemplateBBCode
{
  public:
    /** \brief constructor
     *
     * \param code    "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param tpl     the template that shall be used
     * \param inner   name of the template tag for the inner code
     * \param attr    name of the template tag for the attribute value
     */
    AdvancedTplAmpTransformBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner", const std::string& attr="attribute")
    : AdvancedTemplateBBCode(code, tpl, inner, attr)
    { }
  protected:
    /** \brief applies a transformation (if any) to the attribute value of the BB code
     * during translation
     *
     * \param attr   the attribute value
     */
    inline virtual std::string transformAttribute(const std::string& attr) const
    {
      std::string result(attr);
      std::string::size_type pos = result.find("&");
      while (pos!=std::string::npos)
      {
        result.replace(pos, 1, "&amp;");
        pos = result.find("&", pos+4);
      }//while
      return result;
    }
};//struct

#endif // ADVANCEDTPLAMPTRANSFORMBBCODE_HPP
