/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014  Thoronador

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

#ifndef BBCODE_HPP
#define BBCODE_HPP

#include <string>
#include "../MsgTemplate.hpp"

/** \brief BBCode: basic interface for BB code structs/classes */
struct BBCode
{
  public:
    /** \brief constructor
     *
     * \param code   "name" of the code, i.e. "b" for [B]bold text[/B]
     */
    BBCode(const std::string& code)
    : m_Name(code)
    { }


    /** destructor */
    virtual ~BBCode() {}


    /** \brief returns the code's "name"
     */
    inline const std::string& getName() const
    {
      return m_Name;
    }

    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const = 0;

    #ifndef NO_BBCODE_NOTIFY
    template<typename notifier>
    void notify(const std::string& msg) const
    {
      notifier::put(msg);
    }
    #endif
  private:
    std::string m_Name;
};//struct


/* struct SimpleBBCode:
       handles "simple" BB codes, where the square brackets can be transformed
       to < or > to produce the proper HTML code, e.g. where
       "[TAG]content[/TAG]" becomes "<tag>content</tag>"
*/
struct SimpleBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code   "name" of the code, i.e. "b" for [B]bold text[/B]
     */
    SimpleBBCode(const std::string& code);

    /** destructor */
    virtual ~SimpleBBCode() {}

    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text - the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
};//struct SimpleBBCode


/** \brief CustomizedSimpleBBCode:
       like SimpleBBCode, but with a custom replacement for opening and closing
       tags.
*/
struct CustomizedSimpleBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code     "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param before   replacement for the opening code tag
     * \param after    replacements for the closing code tag
     */
    CustomizedSimpleBBCode(const std::string& code, const std::string& before, const std::string& after);


    /** destructor */
    virtual ~CustomizedSimpleBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
  private:
    std::string m_Before, m_After;
};//struct


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


/* struct SimpleTplAmpTransformBBCode:
       like SimpleTemplateBBCode, but "&" in content will be replaced with
       "&amp;"
*/
struct SimpleTplAmpTransformBBCode: public SimpleTemplateBBCode
{
  public:
    /** \brief constructor
     *
     * \param code    "name" of the code, i.e. "b" for [B]bold text[/B]
     * \param tpl     the template that shall be used
     * \param inner   name of the template tag for the inner code
     */
    SimpleTplAmpTransformBBCode(const std::string& code, const MsgTemplate& tpl, const std::string& inner="inner")
    : SimpleTemplateBBCode(code, tpl, inner)
    { }
  protected:
    /** \brief applies a transformation (if any) to the inner content of the BB code
     * during translation
     *
     * \param inner - the content
     */
    inline virtual std::string transformInner(const std::string& inner) const
    {
      std::string result(inner);
      std::string::size_type pos = result.find("&");
      while (pos!=std::string::npos)
      {
        result.replace(pos, 1, "&amp;");
        pos = result.find("&", pos+4);
      }//while
      return result;
    }
};//struct


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


/** \brief ListBBCode:
       struct for list BB code
*/
struct ListBBCode: public BBCode
{
  public:
    /** \brief constructor
     *
     * \param code        "name" of the code, i.e. "list"
                          for "[LIST][*]item 1[*]2nd item[/LIST]"
     * \param unordered   if set to true, this code will produce an unordered list.
                          Otherwise it will create an ordered list.
     */
    ListBBCode(const std::string& code, bool unordered=true);


    /** destructor */
    virtual ~ListBBCode() {}


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;


    /** returns true, if the code creates an unordered list */
    inline bool createsUnordered() const
    {
      return m_Unordered;
    }
  private:
    bool actualApply(std::string& text, const std::string::size_type offset) const;
    bool m_Unordered;
};//struct


/** \brief HorizontalRuleBBCode:
       struct for hr BB code
*/
struct HorizontalRuleBBCode: public BBCode
{
  public:
    /** constructor
     *
     * \param code   "name" of the code, i.e. "b" for [B]bold text[/B]
     */
    HorizontalRuleBBCode(const std::string& code, const bool isXHTML)
    : BBCode(code), m_isXHTML(isXHTML)
    { }


    /** \brief "applies" the BB code to the given text, i.e. transforms the BB code
     * into its HTML representation
     *
     * \param text   the message text that (may) contain the BB code
     */
    virtual void applyToText(std::string& text) const;
  private:
    bool m_isXHTML;
};//struct HorizontalRuleBBCode

#endif // BBCODE_HPP
