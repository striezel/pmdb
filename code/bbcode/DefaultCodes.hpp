/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2015, 2025  Dirk Stolle

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

#ifndef BBCODE_DEFAULTCODES_HPP
#define BBCODE_DEFAULTCODES_HPP

#include "BBCode.hpp"
#include "BBCodeParser.hpp"
#include "SimpleBBCode.hpp"
#include "CustomizedSimpleBBCode.hpp"
#include "SimpleTplAmpTransformBBCode.hpp"
#include "AdvancedTplAmpTransformBBCode.hpp"

namespace bbcode_default
{
  // [b], [u], [i], [s] codes
  const SimpleBBCode b("b");
  const SimpleBBCode u("u");
  const SimpleBBCode i("i");
  const CustomizedSimpleBBCode s("s",
                               "<span style=\"text-decoration:line-through;\">",
                               "</span>");

  // [sup] and [sub] tags
  const SimpleBBCode sup("sup");
  const SimpleBBCode sub("sub");

  // indent
  const CustomizedSimpleBBCode indent("indent", "<blockquote>", "</blockquote>");

  //alignment stuff
  const SimpleBBCode center("center");
  const CustomizedSimpleBBCode left("left", "<div align=\"left\">", "</div>");
  const CustomizedSimpleBBCode right("right", "<div align=\"right\">", "</div>");

  // code tag
  const CustomizedSimpleBBCode code("code",
                               std::string("<div style=\"margin:20px; margin-top:5px\">\n")
                               + "<div class=\"smallfont\" style=\"margin-bottom:2px; font: 10px verdana,"
                               + " geneva, lucida, 'lucida grande', arial, helvetica, sans-serif;\n"
                               + "font-size:7pt;\">Code:</div>\n"
                               + "<pre dir=\"ltr\" style=\"margin: 0px; padding: 6px; border: 1px inset;"
                               + " width: 620px; text-align: left; overflow: auto\">",
                               "</pre></div>");

  // tt tag
  const CustomizedSimpleBBCode tt("tt", "<tt style=\"font-size: medium\">", "</tt>");

  // simple url tag
  const SimpleTplAmpTransformBBCode url_simple("url", MsgTemplate("<a href=\"{..inner..}\" target=\"_blank\">{..inner..}</a>"), "inner");
  // advanced url tag
  const AdvancedTplAmpTransformBBCode url_advanced("url", MsgTemplate("<a href=\"{..attribute..}\" target=\"_blank\">{..inner..}</a>"), "inner", "attribute");
  // color tag
  const AdvancedTemplateBBCode color("color", MsgTemplate("<font color=\"{..attr..}\">{..inner..}</font>"), "inner", "attr");
  // size tag
  const AdvancedTemplateBBCode size("size", MsgTemplate("<font size=\"{..attr..}\">{..inner..}</font>"), "inner", "attr");
  // font tag
  const AdvancedTemplateBBCode font("font", MsgTemplate("<span style=\"font-family: {..attr..}\">{..inner..}</span>"), "inner", "attr");

  // highlight code
  const CustomizedSimpleBBCode highlight("highlight", "<span style=\"color: red; font-weight: bold;\">", "</span>");

  /* this function adds the "default" BB codes to the given parser

     parameters:
         parser - the parser to which those codes should be added
  */
  inline void addDefaultCodes(BBCodeParser& parser)
  {
    parser.addCode(&bbcode_default::b);
    parser.addCode(&bbcode_default::u);
    parser.addCode(&bbcode_default::i);
    parser.addCode(&bbcode_default::s);
    parser.addCode(&bbcode_default::sup);
    parser.addCode(&bbcode_default::sub);
    parser.addCode(&bbcode_default::indent);
    parser.addCode(&bbcode_default::center);
    parser.addCode(&bbcode_default::left);
    parser.addCode(&bbcode_default::right);
    parser.addCode(&bbcode_default::url_simple);
    parser.addCode(&bbcode_default::url_advanced);
    parser.addCode(&bbcode_default::color);
    parser.addCode(&bbcode_default::size);
    parser.addCode(&bbcode_default::font);
    parser.addCode(&bbcode_default::code);
    parser.addCode(&bbcode_default::tt);
    parser.addCode(&bbcode_default::highlight);
  }

} //namespace

#endif // BBCODE_DEFAULTCODES_HPP
