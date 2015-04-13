/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014, 2015  Thoronador

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

#include "MessageDatabase.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "SortType.hpp"
#include "XMLDocument.hpp"
#include "XMLNode.hpp"
#include "../libthoro/common/DirectoryFileList.h"
#include "../libthoro/common/StringUtils.h"
#include "../libthoro/filesystem/DirectoryFunctions.hpp"

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
:  m_Messages(std::map<SHA256::MessageDigest, PrivateMessage>())
{
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
  throw std::runtime_error("The message database has no message with the given hash "+digest.toHexString()+"!");
}

bool MessageDatabase::importFromFile(const std::string& fileName, uint32_t& readPMs, uint32_t& newPMs, FolderMap& fm)
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

    if (!processFolderNode(node, readPMs, newPMs, fm))
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

bool MessageDatabase::processFolderNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, FolderMap& fm)
{
  if (!node.hasChild()) return false;

  std::string folderName = "";

  if (node.hasAttribute())
  {
    if (node.getFirstAttributeName()=="name")
    {
      folderName = node.getFirstAttributeValue();
    }
  }

  //get child node - should be private messages
  XMLNode current = node.getChild();
  while (current.hasNextSibling())
  {
    //skip non-element nodes (i.e. text nodes, we want element nodes only)
    while (current.hasNextSibling() and !current.isElementNode())
    {
      current = current.getNextSibling();
    }//while (inner)

    if (current.getNameAsString()!="privatemessage")
    {
      std::cout << "Current node should habe name \"privatemessage\", but it is \""
                << current.getNameAsString() << "\" instead!\n";
      return false;
    }
    if (!processPrivateMessageNode(current, readPMs, newPMs, folderName, fm))
    {
      std::cout << "Error while processing <privatemessage> element!\n";
      return false;
    }

    current = current.getNextSibling();
  } //while
  return true;
}

bool MessageDatabase::processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, const std::string& folder, FolderMap& fm)
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
  //add message to DB
  if (addMessage(pm))
  {
    ++newPMs;
  }
  //add entry to folder map
  if (!folder.empty())
    fm.add(pm.getHash(), folder);
  return true;
}

bool MessageDatabase::saveMessages(const std::string& directory, const bool compressed) const
{
  std::map<SHA256::MessageDigest, PrivateMessage>::const_iterator iter = m_Messages.begin();
  const std::string realDirectory(libthoro::filesystem::slashify(directory));
  while (iter!=m_Messages.end())
  {
    if (!iter->second.saveToFile(realDirectory+iter->first.toHexString(), compressed))
      return false;
    ++iter;
  }//while
  return true;
}

bool MessageDatabase::loadMessages(const std::string& directory, uint32_t& readPMs, uint32_t& newPMs, const bool isCompressed)
{
  readPMs = 0;
  newPMs = 0;
  std::vector<FileEntry> files;
  if (!getDirectoryFileList(directory, files, std::string(""), false))
  {
    return false;
  }

  PrivateMessage tempPM;

  const std::string realDirectory(libthoro::filesystem::slashify(directory));

  std::vector<FileEntry>::const_iterator iter = files.begin();
  while (iter!=files.end())
  {
    if (!(iter->IsDirectory))
    {
      if (isValidSHA256Hash(iter->FileName))
      {
        if (!tempPM.loadFromFile(realDirectory+iter->FileName, isCompressed))
        {
          std::cout << "Error while loading message from file \""<< realDirectory+iter->FileName <<"\"!\n";
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


bool MessageDatabase::saveIndexFiles(const std::string& directory, MsgTemplate index, MsgTemplate entry, MsgTemplate folderList, MsgTemplate folderEntry, const FolderMap& fm) const
{
  std::vector<SortType> sortedList;
  Iterator pm_iter = m_Messages.begin();
  while (pm_iter!=m_Messages.end())
  {
    sortedList.push_back(SortType(pm_iter->second.getDatestamp(), pm_iter->first));
    ++pm_iter;
  }//while
  //sort PM list by datestamps - newest first
  std::sort(sortedList.begin(), sortedList.end(), ST_greater);

  //create folder maps for later use
  std::map<std::string, std::vector<SortType> > folderContents;
  while (!sortedList.empty())
  {
    if (fm.hasEntry(sortedList[0].md))
    {
      //add to folder
      folderContents[fm.getFolderName(sortedList[0].md)].push_back(SortType(sortedList[0].datestamp, sortedList[0].md));
    }
    else
    {
      //no folder, so add it with "" a folder name
      folderContents[""].push_back(SortType(sortedList[0].datestamp, sortedList[0].md));
    }
    sortedList.erase(sortedList.begin());
  }//while

  //folder hashes
  std::map<std::string, std::string> folderHashes;
  std::map<std::string, std::vector<SortType> >::const_iterator fcIter = folderContents.begin();
  while (fcIter!=folderContents.end())
  {
    folderHashes[fcIter->first] = SHA256::computeFromBuffer((uint8_t*) fcIter->first.c_str(), fcIter->first.length()*8).toHexString();
    ++fcIter;
  }//while

  //generate folder entries
  std::string theEntries = "";
  fcIter = folderContents.begin();
  while (fcIter!=folderContents.end())
  {
    if (fcIter->first!="")
    {
      folderEntry.addReplacement("folder_link", "folder_"+folderHashes[fcIter->first]+".html", false);
      folderEntry.addReplacement("folder_name", fcIter->first, true);
      theEntries += folderEntry.show();
    }
    ++fcIter;
  }//while

  //put entries into folder list
  folderList.addReplacement("folder_entries", theEntries, false);
  const std::string cFolderList = folderList.show();

  fcIter = folderContents.begin();
  while (fcIter!=folderContents.end())
  {
    theEntries.clear();
    std::vector<SortType>::const_iterator vecIter = fcIter->second.begin();
    while (vecIter!=fcIter->second.end())
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
    index.addReplacement("folders", cFolderList, false);
    const std::string indexText = index.show();
    std::ofstream indexFile;
    std::string fileName = directory;
    if (fcIter->first!="")
      fileName = fileName + "folder_"+folderHashes[fcIter->first]+".html";
    else
      fileName = fileName + "index.html";

    indexFile.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!indexFile) return false;
    indexFile.write(indexText.c_str(), indexText.length());
    const bool success = indexFile.good();
    indexFile.close();
    if (!success) return false;
    ++fcIter;
  }//while
  return true;
}


std::map<md_date, std::vector<md_date> > MessageDatabase::getTextSubsets() const
{
  std::map<md_date, std::vector<md_date> > result;
  Iterator iter = m_Messages.begin();
  while (iter!=m_Messages.end())
  {
    Iterator innerIter = m_Messages.begin();
    while (innerIter!=m_Messages.end())
    {
      if (iter!=innerIter)
      {
        if (iter->second.getMessage().find(innerIter->second.getMessage())!=std::string::npos)
        {
          result[md_date(iter->first, iter->second.getDatestamp())].push_back(md_date(innerIter->first, innerIter->second.getDatestamp()));
        }
      }
      ++innerIter;
    }
    ++iter;
  }//while
  return result;
}

void MessageDatabase::clear()
{
  m_Messages.clear();
}
