/*
    This file is part of the PM XML Interpreter.
    Copyright (C) 2008, 2011, 2012  Thoronador

    The PM XML Interpreter is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    The PM XML Interpreter is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quotes.h"

std::string::size_type getOpeningTagPosition(const std::string& text, const std::string::size_type offset, OpeningTagType &type, std::string::size_type &end)
{
  type = ottNone;
  end = std::string::npos;
  std::string::size_type start = find_ci(text, "[quote", offset);
  if (start==std::string::npos)
  {
    //nothing found
    return std::string::npos;
  }
  std::string::size_type len = text.length();
  //check for length
  if (len<start+7)
  {
    return std::string::npos;
  }
  switch (text[start+6])
  {
    case ']':
         type = ottNormal;
         end = start+6;
         return start;
         break;
    case '=':
         type = ottNamed;
         break;
    default:
         //No valid character, i.e. no real quote. Search for next one.
         return getOpeningTagPosition(text, start+7, type, end);
         break;
  }//swi

  std::string::size_type possible_end;

  //check for single quotes, but check length first
  if (len<start+8)
  {
    //too short
    type = ottNone;
    end = std::string::npos;
    return std::string::npos;
  }
  //quotes there?
  if (text[start+7]=='\'')
  {
    //yes, so go on
    type = (OpeningTagType) (ottNamed + ottSingleQuoted);
    possible_end = text.find("']", start+8);
    //no end found, invalid quote syntax
    if (possible_end==std::string::npos)
    {
      type = ottNone;
      end  = std::string::npos;
      return std::string::npos;
    }
    //end found, go on and return value
    end = possible_end+1;
    return start;
  }//if
  //no quotes found, so check for the next closing square bracket
  possible_end = text.find(']', start+8);
  //no end found, invalid quote syntax
  if (possible_end==std::string::npos)
  {
    type = ottNone;
    end  = std::string::npos;
    return std::string::npos;
  }
  //end found
  end = possible_end;
  return start;
}//getOpeningTagPosition

NameAndPost extractNameAndPost(const std::string& msg_content, const std::string::size_type start, const OpeningTagType type, const std::string::size_type end)
{
  if ((end<start+8) || (msg_content.length()<=end) ||
      ((end<start+10) && ((type & ottSingleQuoted)==ottSingleQuoted)))
  {
    //too short for any name or post, or too short for values given in end
    // -> return empty result
    return NameAndPost("", "");
  }
  //extract inner part
  std::string inner = msg_content.substr(start+7, end-start-7);
  if ((type & ottSingleQuoted)==ottSingleQuoted)
  {
    //get rid of those single quotes
    if (inner.length()<3) return NameAndPost("", "");
    inner = inner.substr(1, inner.length()-2);
  }
  std::string::size_type post = inner.rfind(';');
  if (post==std::string::npos)
  {
    //no ; found, whole string is the user's name
    return NameAndPost(inner);
  }
  // ; found, remove ; itself
  std::string post_string = inner.substr(post+1);
  std::string::size_type p_len = post_string.length()+1;
  std::string name = inner.substr(0, inner.length()-p_len);
  if (name.empty())
  {
    return NameAndPost();
  }
  if (p_len==1)
  {
    //post string is too short, return name only, if present
    if (!name.empty())
    {
      return NameAndPost(name);
    }
    return NameAndPost();
  }//if
  //return name and post, that's the way we want it
  return NameAndPost(name, post_string);
}//function extractNameAndPost

std::string handleQuotes(std::string msg_content, const std::string& forumURL)
{
  const int max_quotes = 15;
  const int processed_quotes = 0;
  while (processed_quotes<max_quotes)
  {
    //get start
    std::string::size_type offset = 0;
    OpeningTagType type;
    std::string::size_type end_pos;
    std::string::size_type first = getOpeningTagPosition(msg_content, offset, type, end_pos);
    if (first==std::string::npos)
    {
      return msg_content;
    }
    //get possible end
    std::string::size_type last = getClosingQuoteTagPosition(msg_content, first+1);
    if (last==std::string::npos)
    {
      return msg_content;
    }
    //now make sure we got the innermost quote tag
    std::string::size_type second;
    std::string::size_type second_end_pos;
    OpeningTagType second_type;

    while ((std::string::npos!=(second=getOpeningTagPosition(msg_content, first+1, second_type, second_end_pos)))
          && (second_end_pos<last))
    //while ((false!==($second=getOpeningTagPosition($msg_content, $first+1, $second_type, $second_end_pos)))
    //      && ($second_end_pos<$last))
    {
      //set stuff to newly found, inner quote tag's stuff
      first = second;
      type = second_type;
      end_pos = second_end_pos;
    }//inner while
    //if we get here, we should already have the quote's limits properly
    // --> handle it
    NameAndPost data;
    switch (type)
    {
      case ottNormal:
           //normal tags, no name or single quotes
           msg_content = msg_content.substr(0, first) //part before opening quote
                         +"<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
                         +"<tr><td><div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
+"    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
+"    <tr>\n"
+"        <td style=\"border:1px inset\">" //replacement for first part of quote
                         +msg_content.substr(end_pos+1, last-end_pos-1) //inner text of quote
                         +"</td>\n"
+"    </tr>\n"
+"    </table>\n"
+"</td></tr>\n"
+"</table>" //replacement for closing part of tag
                         +msg_content.substr(last+8, msg_content.length()-last-8); //rest of text
           break;
      case ottNamed:
      case ottNamed+ottSingleQuoted:
           data = extractNameAndPost(msg_content, first, type, end_pos);
           if (!data.post.empty())
           {
             //quote with name and post number
             msg_content = msg_content.substr(0, first) //part before opening quote
                           +"<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
+"  <tr><td>\n"
+"    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
+"    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
+"    <tr>\n"
+"      <td class=\"alt2\" style=\"border:1px inset\">\n"
+"        <div>\n"
+"            Zitat von <strong>"+data.name+"</strong>\n"
+"            <a href=\""+forumURL+"showthread.php?p="+data.post+"#post"+data.post
            +"\" rel=\"nofollow\"><img class=\"inlineimg\" src=\"img/buttons/viewpost.gif\" border=\"0\" alt=\"Beitrag anzeigen\"></a>\n"
+"        </div>\n"
+"        <div style=\"font-style:italic\">"
        +msg_content.substr(end_pos+1, last-end_pos-1) //inner text of quote
        +"</div>\n"
+"      </td>\n"
+"    </tr>\n"
+"    </table>\n"
+"  </td></tr>\n"
+"</table>" //replacement for closing part
                           +msg_content.substr(last+8, msg_content.length()-last-8); //rest of text
           }//if post number is set
           else
           {
             //quote with name only
             msg_content = msg_content.substr(0, first) //part before opening quote
                           +"<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"20\">\n"
+"  <tr><td>\n"
+"    <div class=\"smallfont\" style=\"margin-bottom:2px\">Zitat:</div>\n"
+"    <table cellpadding=\"6\" cellspacing=\"0\" border=\"0\" width=\"100%\">\n"
+"    <tr>\n"
+"        <td style=\"border:1px inset\">\n"
+"            <div>\n"
+"                Zitat von <strong>"+data.name+"</strong>\n"
+"           </div>\n"
+"           <div style=\"font-style:italic\">"
             +msg_content.substr(end_pos+1, last-end_pos-1) //inner text of quote
           +"</div>\n"
+"        </td>\n"
+"    </tr>\n"
+"    </table>\n"
+"</td></tr>\n"
+"</table>" //closing part
             +msg_content.substr(last+8, msg_content.length()-last-8); //rest of text
           }//if no post number is given
           break;
      default:
           //invalid or unimplemented type found, just return here
           return msg_content;
           break;
    }//swi
  }//while
  //We are done completely, return.
  return msg_content;
}//handleQuotes
