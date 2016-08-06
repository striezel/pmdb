/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database test suite.
    Copyright (C) 2015  Dirk Stolle

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

#include <iostream>
#include <string>
#include <vector>
#include "../../code/MsgTemplate.hpp"

struct ReplData
{
  std::string tag;
  std::string replacement;
  bool killHTML;

  ReplData(const std::string& _tag, const std::string& repl, const bool kill_HTML)
  : tag(_tag), replacement(repl), killHTML(kill_HTML)
  { }
}; //struct

struct TplData
{
  std::string tpl_text;
  std::vector<ReplData> replacements;
  std::string expectedResultText;

  TplData(const std::string& tpl, const std::vector<ReplData>& repl, const std::string& result)
  : tpl_text(tpl), replacements(repl), expectedResultText(result)
  {}
}; //struct

int main()
{
  std::vector<TplData> template_data;

  // string without any tags and template without tags -> should show unchanged text
  template_data.push_back(TplData("This is a text that does not contain any replacement tags.",
                                  std::vector<ReplData>(),
                                  "This is a text that does not contain any replacement tags."
                                  ));

  // template with tag, but no replacement data -> should show unchanged text
  template_data.push_back(TplData("This is a text with {..replacement..} tags.",
                                  std::vector<ReplData>(),
                                  "This is a text with {..replacement..} tags."
                                  ));

  // template with replacement tags
  template_data.push_back(TplData("The {..person..} is {..attribute..}.",
                                  std::vector<ReplData>(),
                                  "The hobbit is small."
                                  ));
  template_data.back().replacements.push_back(ReplData("person", "hobbit", false));
  template_data.back().replacements.push_back(ReplData("attribute", "small", false));

  // template with replacement tags that match words in the text -> should not change text
  template_data.push_back(TplData("The person has an object.",
                                  std::vector<ReplData>(),
                                  "The person has an object."
                                  ));
  template_data.back().replacements.push_back(ReplData("person", "hobbit", false));
  template_data.back().replacements.push_back(ReplData("object", "teapot", false));

  // tag with HTML code replacement and disabled "kill HTML" option
  template_data.push_back(TplData("This is {..what..}.",
                                  std::vector<ReplData>(),
                                  "This is <html>."
                                  ));
  template_data.back().replacements.push_back(ReplData("what", "<html>", false));

  // another tag with HTML code replacement and disabled "kill HTML" option
  template_data.push_back(TplData("The information can be found at {..url..}.",
                                  std::vector<ReplData>(),
                                  "The information can be found at http://www.example.com/this/is_not/a.html?page=you&should=request."
                                  ));
  template_data.back().replacements.push_back(ReplData("url", "http://www.example.com/this/is_not/a.html?page=you&should=request", false));

  // tag with HTML code replacement and enabled "kill HTML" option
  template_data.push_back(TplData("This is {..what..}.",
                                  std::vector<ReplData>(),
                                  "This is &lt;html&gt;."
                                  ));
  template_data.back().replacements.push_back(ReplData("what", "<html>", true));

  // another tag with HTML code replacement and enabled "kill HTML" option
  template_data.push_back(TplData("The information can be found at {..url..}.",
                                  std::vector<ReplData>(),
                                  "The information can be found at http://www.example.com/this/is_not/a.html?page=you&amp;should=request."
                                  ));
  template_data.back().replacements.push_back(ReplData("url", "http://www.example.com/this/is_not/a.html?page=you&should=request", true));

  // tag with HTML code replacement and enabled and disabled "kill HTML" option
  template_data.push_back(TplData("This is {..what..} and {..what_else..}.",
                                  std::vector<ReplData>(),
                                  "This is <html> and &lt;html&gt;."
                                  ));
  template_data.back().replacements.push_back(ReplData("what", "<html>", false));
  template_data.back().replacements.push_back(ReplData("what_else", "<html>", true));


  // Run through all the test data and check, whether it does match the expected result.
  unsigned int i;
  for (i=0; i<template_data.size(); ++i)
  {
    MsgTemplate tpl(template_data[i].tpl_text);
    std::vector<ReplData>::const_iterator iter = template_data[i].replacements.begin();
    while (iter != template_data[i].replacements.end())
    {
      tpl.addReplacement(iter->tag, iter->replacement, iter->killHTML);
      ++iter;
    } //while

    const std::string shownText = tpl.show();
    if (shownText != template_data[i].expectedResultText)
    {
       std::cout << "Output of template " << i << " does not match the expected text!\n";
       std::cout << "Output:   \"" << shownText << "\".\n"
                 << "Expected: \"" << template_data[i].expectedResultText << "\".\n";
       return 1;
    }
  } //for i

  //success
  return 0;
}
