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

#ifndef BBCODE_H
#define BBCODE_H

#include <string>
#include "../MsgTemplate.h"

struct BBCode
{
  public:
    /* constructor

       parameters:
           code - "name" of the code, i.e. "b" for [B]bold text[/B]
    */
    BBCode(const std::string& code)
    {
      m_Name = code;
    }

    /* destructor */
    virtual ~BBCode() {}

    inline const std::string& getName() const
    {
      return m_Name;
    }

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const = 0;
  private:
    std::string m_Name;
};//struct


struct SimpleBBCode: public BBCode
{
  public:
    /* constructor

       parameters:
           code - "name" of the code, i.e. "b" for [B]bold text[/B]
    */
    SimpleBBCode(const std::string& code);

    /* destructor */
    virtual ~SimpleBBCode() {}

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const;
};//struct SimpleBBCode


struct CustomizedSimpleBBCode: public BBCode
{
  public:
    /* constructor

       parameters:
           code   - "name" of the code, i.e. "b" for [B]bold text[/B]
           before - replacement for the opening code tag
           after  - replacements for the closing code tag
    */
    CustomizedSimpleBBCode(const std::string& code, const std::string& before, const std::string& after);

    /* destructor */
    virtual ~CustomizedSimpleBBCode() {}

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const;
  private:
    std::string m_Before, m_After;
};//struct


struct SimpleTemplateBBCode: public BBCode
{
  public:
    /* constructor

       parameters:
           code  - "name" of the code, i.e. "b" for [B]bold text[/B]
           tpl   - the template that shall be used
           inner - name of the template tag for the inner code
    */
    SimpleTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner");

    /* destructor */
    virtual ~SimpleTemplateBBCode() {}

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const;

    /* returns the current template */
    inline const MsgTemplate& getTemplate() const
    {
      return m_Template;
    }

    /* returns the name of the template tag for the inner code */
    inline const std::string& getInnerName() const
    {
      return m_InnerName;
    }
  private:
    MsgTemplate m_Template;
    std::string m_InnerName;
};//struct


struct AdvancedTemplateBBCode: public SimpleTemplateBBCode
{
  /* constructor

       parameters:
           code  - "name" of the code, i.e. "b" for [B]bold text[/B]
           tpl   - the template that shall be used
           inner - name of the template tag for the inner code
           attr  - name of the template tag for the attribute value
    */
    AdvancedTemplateBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner", const std::string& attr="attribute");

    /* destructor */
    virtual ~AdvancedTemplateBBCode() {}

    /* "applies" the BB code to the given text, i.e. transforms the BB code
       into its HTML representation

       parameters:
           text - the message text that (may) contain the BB code
    */
    virtual void applyToText(std::string& text) const;
  private:
    std::string m_AttrName;
};//struct

#endif // BBCODE_H
