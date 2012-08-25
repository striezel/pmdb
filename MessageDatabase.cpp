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

#include "MessageDatabase.h"
#include <iostream>
#include <sstream>
#include "XMLDocument.h"
#include "XMLNode.h"

MessageDatabase::MessageDatabase()
{
  m_Messages.clear();
}

void MessageDatabase::addMessage(const PrivateMessage& pm)
{
  m_Messages.push_back(pm);
}

unsigned int MessageDatabase::getNumberOfMessages() const
{
  return m_Messages.size();
}

bool MessageDatabase::importFromFile(const std::string& fileName, uint32_t& readPMs)
{
  readPMs = 0;
  //parse XML file
  XMLDocument doc(fileName);
  if (!doc.isParsed())
  {
    std::cout << "Could not parse xml file!\n";
    return false;
  }
  //we don't want (and cannot use) empty files
  if (doc.isEmpty())
  {
    std::cout << "Empty xml document!\n";
    return false;
  }
  XMLNode node = doc.getRootNode();
  if (node.getNameAsString()!="privatemessages")
  {
    std::cout << "Root element's name is not \"privatemessages\" but \""
              << node.getNameAsString()<<"\" instead.\n";
    return false;
  }
  if (!node.hasChild())
  {
    std::cout << "No child nodes after root node.\n";
    return false;
  }
  node = node.getChild();

  while (node.hasNextSibling())
  {
    //folder node should be next, skip all text nodes
    while (node.hasNextSibling() and !node.isElementNode())
    {
      node = node.getNextSibling();
    }

    if (node.getNameAsString()!="folder")
    {
      std::cout << "Element's name is not \"folder\" but \""<<node.getNameAsString() <<"\".\n";
      return false;
    }

    bool success = processFolderNode(node, readPMs);
    if (!success)
    {
      std::cout << "Error while processing folder!\n";
      return false;
    }
    node = node.getNextSibling();
  }//while (outer one)
  #ifdef DEBUG
  std::cout << "DEBUG: (importFromFile) Going to return now!\n";
  #endif
  return true;
}

bool MessageDatabase::processFolderNode(const XMLNode& node, uint32_t& readPMs)
{
  if (!node.hasChild()) return false;

  //get child node - should be private messages
  XMLNode current = node.getChild();
  while (current.hasNextSibling())
  {
    //skip non-element nodes (i.e. text nodes, we want element nodes only)
    while (current.hasNextSibling() and !current.isElementNode())
    {
      current = current.getNextSibling();
    }

    if (current.getNameAsString()!="privatemessage")
    {
      std::cout << "Current node should habe name \"privatemessage\", but it is \""
                << current.getNameAsString() << "\" instead!\n";
      return false;
    }
    if (!processPrivateMessageNode(current, readPMs))
    {
      std::cout << "Error while processing <privatemessage> element!\n";
      return false;
    }

    current = current.getNextSibling();
  }
  #ifdef DEBUG
  std::cout << "DEBUG: Another folder done!\n";
  #endif
  return true;
}

bool MessageDatabase::processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs)
{
  if (!node.hasChild()) return false;

  PrivateMessage pm;

  //get child node - should be private messages member data
  XMLNode current = node.getChild();
  while (current.hasNextSibling())
  {
    //skip non-element nodes (i.e. text nodes, we want element nodes only)
    while (current.hasNextSibling() and !current.isElementNode())
    {
      current = current.getNextSibling();
    }

    const std::string curName = current.getNameAsString();
    if (curName=="datestamp")
    {
      if (!pm.datestamp.empty())
      {
        std::cout << "Error: More than one datestamp node in private message!\n";
        return false;
      }
      pm.datestamp = current.getPlainTextContent();
      #ifdef DEBUG
      std::cout << "DEBUG: datestamp content is \""<<pm.datestamp<<"\".\n";
      #endif
    }//if "datestamp"
    else if (curName=="title")
    {
      if (!pm.title.empty())
      {
        std::cout << "Error: More than one title node in private message!\n";
        return false;
      }
      pm.title = current.getContentBoth();
      #ifdef DEBUG
      std::cout << "DEBUG: title content is \""<<pm.title<<"\".\n";
      #endif
    }//if "title"
    else if (curName=="fromuser")
    {
      if (!pm.fromUser.empty())
      {
        std::cout << "Error: More than one fromuser node in private message!\n";
        return false;
      }
      pm.fromUser = current.getContentBoth();
      #ifdef DEBUG
      std::cout << "DEBUG: fromUser content is \""<<pm.fromUser<<"\".\n";
      #endif
    }//if "fromuser"
    else if (curName=="fromuserid")
    {
      if (pm.fromUserID!=0)
      {
        std::cout << "Error: More than one fromuserid node in private message!\n";
        return false;
      }

      const std::string temp = current.getPlainTextContent();
      if (!(std::stringstream (temp) >> pm.fromUserID))
      {
        std::cout << "Error: could not convert \""<<temp<<"\" to integer!\n";
        return false;
      }
      #ifdef DEBUG
      std::cout << "DEBUG: fromUserID is \""<<pm.fromUserID<<"\".\n";
      #endif
    }//if "fromuserid"
    else if (curName=="touser")
    {
      if (!pm.toUser.empty())
      {
        std::cout << "Error: More than one touser node in private message!\n";
        return false;
      }
      pm.toUser = current.getContentBoth();
      #ifdef DEBUG
      std::cout << "DEBUG: toUser content is \""<<pm.toUser<<"\".\n";
      #endif
    }//if "touser"
    else if (curName=="message")
    {
      if (!pm.message.empty())
      {
        std::cout << "Error: More than one message node in private message!\n";
        return false;
      }
      pm.message = current.getContentBoth();
      #ifdef DEBUG
      std::cout << "DEBUG: message content is \""<<pm.message<<"\".\n";
      #endif
    }//if "message"
    else
    {
      std::cout << "Error: encountered unknown node named \""<<curName<<"\".\n";
      return false;
    }
    current = current.getNextSibling();
  }//while (outer)

  //check data members
  if (pm.message.empty())
  {
    std::cout << "Error: PM message is empty!\n";
    return false;
  }
  if (pm.title.empty())
  {
    std::cout << "Error: PM title is empty!\n";
    return false;
  }
  if (pm.datestamp.empty())
  {
    std::cout << "Error: PM datestamp is empty!\n";
    return false;
  }
  if (pm.fromUser.empty())
  {
    std::cout << "Error: PM fromuser is empty!\n";
    return false;
  }
  if (pm.toUser.empty())
  {
    std::cout << "Error: PM touser is empty!\n";
    return false;
  }

  ++readPMs;
  addMessage(pm);
  return true;
}
