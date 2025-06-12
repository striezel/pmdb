/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2015, 2017  Dirk Stolle

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

#include "MsgTemplate.hpp"
#include <fstream>
#include <cstring>

MsgTemplate::MsgTemplate()
: m_Tags(std::map<std::string, std::string>()), m_Template("")
{ }

MsgTemplate::MsgTemplate(const std::string& tplText)
: m_Tags(std::map<std::string, std::string>()), m_Template(tplText)
{ }

bool MsgTemplate::loadFromFile(const std::string& fileName)
{
  std::ifstream inputFile;
  inputFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!inputFile.is_open())
  {
    return false;
  }
  const unsigned int buf_size = 4096;
  char buffer[buf_size];

  m_Template.clear();
  while (!inputFile.eof())
  {
    memset(buffer, '\0', buf_size);
    inputFile.read(buffer, buf_size - 1);
    if (!inputFile.good())
    {
      if (!inputFile.eof())
      {
        inputFile.close();
        return false;
      }
    }
    m_Template += std::string(buffer);
  }
  inputFile.close();
  return true;
}

bool MsgTemplate::loadFromString(const std::string& tplText)
{
  m_Template = tplText;
  return true;
}

void MsgTemplate::addReplacement(const std::string& tag, const std::string& replacement, const bool killHTML)
{
  m_Tags[tag] = prepareReplacement(replacement, killHTML);
}

void MsgTemplate::clearReplacements()
{
  m_Tags.clear();
}

std::string MsgTemplate::show() const
{
  std::string result = m_Template;
  for (const auto& tag : m_Tags)
  {
    const std::string fullTag = "{.." + tag.first + "..}";
    std::string::size_type pos = result.find(fullTag);
    while (pos != std::string::npos)
    {
      result.replace(pos, fullTag.length(), tag.second);
      pos = result.find(fullTag, pos);
    }
  }
  return result;
}

std::string MsgTemplate::prepareReplacement(std::string content, const bool killHTML)
{
  std::string::size_type pos = std::string::npos;
  if (killHTML)
  {
    pos = content.find("&");
    while (pos != std::string::npos)
    {
      content.replace(pos, 1, "&amp;");
      pos = content.find("&", pos + 1);
    }

    pos = content.find("<");
    while (pos != std::string::npos)
    {
      content.replace(pos, 1, "&lt;");
      pos = content.find("<", pos + 1);
    }

    pos = content.find(">");
    while (pos != std::string::npos)
    {
      content.replace(pos, 1, "&gt;");
      pos = content.find(">", pos + 1);
    }
  } // if escape HTML tags

  pos = content.find("{..");
  while (pos != std::string::npos)
  {
    content.replace(pos, 3, "&#x7B;..");
    pos = content.find("{..", pos + 1);
  }

  pos = content.find("..}");
  while (pos != std::string::npos)
  {
    content.replace(pos, 3, "..&#x7D;");
    pos = content.find("..}", pos + 1);
  }

  return content;
}
