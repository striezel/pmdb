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

#ifndef SIMPLETEMPLATEBBCODE_HPP
#define SIMPLETEMPLATEBBCODE_HPP

#include <string>
#include "BBCode.hpp"
#include "../MsgTemplate.hpp"


/** \brief SimpleTemplateBBCode:
       struct for BB codes that use templates (class MsgTemplate) for the
       proper replacement of BB code. However, this class will just handle
       "simple" BB codes of the form "[TAG]content[/TAG]" and NOT codes of the
       form "[TAG=value]content[/TAG]". See AdvancedTemplateBBCode for the
       later variant.
*/
struct SimpleTemplateBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code    "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param tpl     the template that shall be used
     * \param inner   name of the template tag for the inner code
     */
    SimpleTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner");


    /** destructor */
    virtual ~SimpleTemplateBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text - the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;


    /** returns the current template */
    inline const MsgTemplate& getTemplate() const
    {
      return m_Template;
    }

    /** returns the name of the template tag for the inner code */
    inline const std::string& getInnerName() const
    {
      return m_InnerName;
    }
  protected:
    /** \brief applies a transformation (if any) to the inner content of the BB code
     * during translation
     *
     * \param inner   the content
     */
    inline virtual std::string transformInner(const std::string& inner) const
    {
      return inner;
    }
  private:
    MsgTemplate m_Template;
    std::string m_InnerName;
};//struct

#endif // SIMPLETEMPLATEBBCODE_HPP
