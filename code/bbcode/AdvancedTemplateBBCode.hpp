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

#ifndef ADVANCEDTEMPLATEBBCODE_HPP
#define ADVANCEDTEMPLATEBBCODE_HPP

#include "SimpleTemplateBBCode.hpp"

/** \brief AdvancedTemplateBBCode:
       struct for BB codes that use templates (class MsgTemplate) for the
       proper replacement of BB code. However, this class will just handle
       "advanced" BB codes of the form "[TAG=value]content[/TAG]" and NOT the
       simpler codes of the form "[TAG]content[/TAG]". See SimpleTemplateBBCode
       for the later variant.
*/
struct AdvancedTemplateBBCode: public SimpleTemplateBBCode
{
  public:
    /** \brief constructor
     *
     * \param code    "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param tpl     the template that shall be used
     * \param inner   name of the template tag for the inner code
     * \param attr    name of the template tag for the attribute value
     */
    AdvancedTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner", const std::string& attr="attribute");


    /** destructor */
    virtual ~AdvancedTemplateBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
  protected:
    /** \brief applies a transformation (if any) to the attribute value of the BB code
     * during translation
     *
     * \param attr   the attribute value
     */
    inline virtual std::string transformAttribute(const std::string& attr) const
    {
      return attr;
    }
  private:
    std::string m_AttrName;
};//struct

#endif // ADVANCEDTEMPLATEBBCODE_HPP
