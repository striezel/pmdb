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
#include <algorithm>
#include "XMLDocument.h"
#include "XMLNode.h"
#include "libthoro/common/DirectoryFileList.h"
#include "libthoro/common/StringUtils.h"

bool isValidSHA256Hash(const std::string& hash)
{
  if (hash.length()!=64) return false;
  unsigned int i;
  for (i=0; i<64; ++i)
  {
    switch (hash[i])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
           break;
      default:
           return false;
           break;
    }//swi
  }//for
  return true;
}

MessageDatabase::MessageDatabase()
{
  m_Messages.clear();
}

bool MessageDatabase::addMessage(PrivateMessage& pm)
{
  if (m_Messages.find(pm.getHash())!=m_Messages.end())
    return false;
  m_Messages[pm.getHash()] = pm;
  return true;
}

unsigned int MessageDatabase::getNumberOfMessages() const
{
  return m_Messages.size();
}

bool MessageDatabase::hasMessage(PrivateMessage& pm) const
{
  return (m_Messages.find(pm.getHash())!=m_Messages.end());
}

const PrivateMessage& MessageDatabase::getMessage(const SHA256::MessageDigest& digest) const
{
  const Iterator iter = m_Messages.find(digest);
  if (iter!=m_Messages.end()) return iter->second;
  throw 42;
}

bool MessageDatabase::importFromFile(const std::string& fileName, uint32_t& readPMs, uint32_t& newPMs)
{
  readPMs = 0;
  newPMs = 0;
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

    if (!processFolderNode(node, readPMs, newPMs))
    {
      std::cout << "Error while processing folder!\n";
      return false;
    }
    node = node.getNextSibling();
  }//while (outer one)
  return true;
}

MessageDatabase::Iterator MessageDatabase::getBegin() const
{
  return m_Messages.begin();
}

MessageDatabase::Iterator MessageDatabase::getEnd() const
{
  return m_Messages.end();
}

bool MessageDatabase::processFolderNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs)
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
    if (!processPrivateMessageNode(current, readPMs, newPMs))
    {
      std::cout << "Error while processing <privatemessage> element!\n";
      return false;
    }

    current = current.getNextSibling();
  }
  return true;
}

bool MessageDatabase::processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs)
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
      if (!pm.getDatestamp().empty())
      {
        std::cout << "Error: More than one datestamp node in private message!\n";
        return false;
      }
      pm.setDatestamp(current.getPlainTextContent());
    }//if "datestamp"
    else if (curName=="title")
    {
      if (!pm.getTitle().empty())
      {
        std::cout << "Error: More than one title node in private message!\n";
        return false;
      }
      pm.setTitle(current.getContentBoth());
    }//if "title"
    else if (curName=="fromuser")
    {
      if (!pm.getFromUser().empty())
      {
        std::cout << "Error: More than one fromuser node in private message!\n";
        return false;
      }
      pm.setFromUser(current.getContentBoth());
    }//if "fromuser"
    else if (curName=="fromuserid")
    {
      if (pm.getFromUserID()!=0)
      {
        std::cout << "Error: More than one fromuserid node in private message!\n";
        return false;
      }

      const std::string temp = current.getPlainTextContent();
      uint32_t tempUint;
      if (!(std::stringstream (temp) >> tempUint))
      {
        std::cout << "Error: could not convert \""<<temp<<"\" to integer!\n";
        return false;
      }
      pm.setFromUserID(tempUint);
    }//if "fromuserid"
    else if (curName=="touser")
    {
      if (!pm.getToUser().empty())
      {
        std::cout << "Error: More than one touser node in private message!\n";
        return false;
      }
      pm.setToUser(current.getContentBoth());
    }//if "touser"
    else if (curName=="message")
    {
      if (!pm.getMessage().empty())
      {
        std::cout << "Error: More than one message node in private message!\n";
        return false;
      }
      pm.setMessage(current.getContentBoth());
    }//if "message"
    else
    {
      std::cout << "Error: encountered unknown node named \""<<curName<<"\".\n";
      return false;
    }
    current = current.getNextSibling();
  }//while (outer)

  //check data members
  if (pm.getMessage().empty())
  {
    std::cout << "Error: PM message is empty!\n";
    return false;
  }
  if (pm.getTitle().empty())
  {
    std::cout << "Error: PM title is empty!\n";
    return false;
  }
  if (pm.getDatestamp().empty())
  {
    std::cout << "Error: PM datestamp is empty!\n";
    return false;
  }
  if (pm.getFromUser().empty())
  {
    std::cout << "Error: PM fromuser is empty!\n";
    return false;
  }
  if (pm.getToUser().empty())
  {
    std::cout << "Error: PM touser is empty!\n";
    return false;
  }

  pm.normalise();

  ++readPMs;
  if (addMessage(pm))
  {
    ++newPMs;
  }
  return true;
}

bool MessageDatabase::saveMessages(const std::string& directory) const
{
  std::map<SHA256::MessageDigest, PrivateMessage>::const_iterator iter = m_Messages.begin();
  while (iter!=m_Messages.end())
  {
    if (!iter->second.saveToFile(directory+iter->first.toHexString()))
      return false;
    ++iter;
  }//while
  return true;
}

bool MessageDatabase::loadMessages(const std::string& directory, uint32_t& readPMs, uint32_t& newPMs)
{
  readPMs = 0;
  newPMs = 0;
  std::vector<FileEntry> files;
  if (!getDirectoryFileList(directory, files, std::string(""), false))
  {
    return false;
  }

  PrivateMessage tempPM;

  std::vector<FileEntry>::const_iterator iter = files.begin();
  while (iter!=files.end())
  {
    if (!(iter->IsDirectory))
    {
      if (isValidSHA256Hash(iter->FileName))
      {
        if (!tempPM.loadFromFile(directory+iter->FileName))
        {
          std::cout << "Error while loading message from file \""<< directory+iter->FileName <<"\"!\n";
          return false;
        }
        if (iter->FileName!=tempPM.getHash().toHexString())
        {
          std::cout << "Error: Content of message file has been altered!\n";
          return false;
        }
        ++readPMs;
        if (addMessage(tempPM))
        {
          ++newPMs;
        }
      }//if
    }//if
    ++iter;
  }//while
  return true;
}

//aux. type
struct SortType
{
  std::string datestamp;
  SHA256::MessageDigest md;

  SortType(const std::string& ds, const SHA256::MessageDigest& dig)
  : datestamp(ds), md(dig)
  {
  }
};//struct

bool ST_greater(const SortType& __x, const SortType& __y)
{
  if (__x.datestamp>__y.datestamp) return true;
  if (__x.datestamp<__y.datestamp) return false;
  return __x.md.toHexString() > __y.md.toHexString();
}

bool MessageDatabase::saveIndexFile(const std::string& fileName, MsgTemplate index, MsgTemplate entry) const
{
  std::vector<SortType> sortedList;
  Iterator pm_iter = m_Messages.begin();
  while (pm_iter!=m_Messages.end())
  {
    sortedList.push_back(SortType(pm_iter->second.getDatestamp(), pm_iter->first));
    ++pm_iter;
  }//while
  std::sort(sortedList.begin(), sortedList.end(), ST_greater);
  std::string theEntries = "";
  std::vector<SortType>::const_iterator vecIter = sortedList.begin();
  while (vecIter!=sortedList.end())
  {
    const PrivateMessage& currMessage = getMessage(vecIter->md);
    entry.addReplacement("date",       vecIter->datestamp, true);
    entry.addReplacement("pm_url",     vecIter->md.toHexString()+".html", false);
    entry.addReplacement("title",      currMessage.getTitle(), true);
    entry.addReplacement("fromuserid", intToString(currMessage.getFromUserID()), true);
    entry.addReplacement("fromuser",   currMessage.getFromUser(), true);
    theEntries += entry.show();
    ++vecIter;
  }//while
  index.addReplacement("entries", theEntries, false);
  const std::string indexText = index.show();
  std::ofstream indexFile;
  indexFile.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
  if (!indexFile) return false;
  indexFile.write(indexText.c_str(), indexText.length());
  const bool success = indexFile.good();
  indexFile.close();
  return success;
}
