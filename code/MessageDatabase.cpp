/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014, 2015, 2016, 2025  Dirk Stolle

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
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "SortType.hpp"
#include "XMLDocument.hpp"
#include "XMLNode.hpp"
#include "../libstriezel/common/DirectoryFileList.hpp"
#include "../libstriezel/common/StringUtils.hpp"
#include "../libstriezel/filesystem/directory.hpp"
#include "../libstriezel/hash/sha256/BufferSourceUtility.hpp"

MessageDatabase::MessageDatabase()
:  m_Messages(std::map<SHA256::MessageDigest, PrivateMessage>())
{
}

bool MessageDatabase::addMessage(PrivateMessage& pm)
{
  if (m_Messages.find(pm.getHash()) != m_Messages.end())
  {
    return false;
  }
  m_Messages[pm.getHash()] = pm;
  return true;
}

unsigned int MessageDatabase::getNumberOfMessages() const
{
  return m_Messages.size();
}

bool MessageDatabase::hasMessage(PrivateMessage& pm) const
{
  return m_Messages.find(pm.getHash()) != m_Messages.end();
}

const PrivateMessage& MessageDatabase::getMessage(const SHA256::MessageDigest& digest) const
{
  const Iterator iter = m_Messages.find(digest);
  if (iter != m_Messages.end())
  {
        return iter->second;
  }
  throw std::runtime_error("The message database has no message with the given hash "
                          + digest.toHexString() + "!");
}

bool MessageDatabase::importFromFile(const std::string& fileName, uint32_t& readPMs, uint32_t& newPMs, FolderMap& fm)
{
  readPMs = 0;
  newPMs = 0;
  // parse XML file
  XMLDocument doc(fileName);
  if (!doc.isParsed())
  {
    std::cout << "Could not parse xml file!\n";
    return false;
  }
  // We don't want (and cannot use) empty files.
  if (doc.isEmpty())
  {
    std::cout << "Empty xml document!\n";
    return false;
  }
  XMLNode node = doc.getRootNode();
  if (node.getNameAsString() != "privatemessages")
  {
    std::cerr << "Root element's name is not \"privatemessages\" but \""
              << node.getNameAsString() << "\" instead.\n";
    return false;
  }
  if (!node.hasChild())
  {
    std::cerr << "No child nodes after root node.\n";
    return false;
  }
  node = node.getChild();

  while (node.hasNextSibling())
  {
    // folder node should be next, skip all text nodes
    while (node.hasNextSibling() && !node.isElementNode())
    {
      node = node.getNextSibling();
    }

    if (node.getNameAsString() != "folder")
    {
      std::cerr << "Element's name is not \"folder\" but \"" << node.getNameAsString() << "\".\n";
      return false;
    }

    if (!processFolderNode(node, readPMs, newPMs, fm))
    {
      std::cerr << "Error while processing folder!\n";
      return false;
    }
    node = node.getNextSibling();
  }
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
  if (!node.hasChild())
  {
    return false;
  }

  std::string folderName = "";

  if (node.hasAttribute())
  {
    if (node.getFirstAttributeName() == "name")
    {
      folderName = node.getFirstAttributeValue();
    }
  }

  // get child node - should be private messages
  XMLNode current = node.getChild();
  while (current.hasNextSibling())
  {
    // skip non-element nodes (i.e. text nodes, we want element nodes only)
    while (current.hasNextSibling() && !current.isElementNode())
    {
      current = current.getNextSibling();
    }

    if (current.getNameAsString() != "privatemessage")
    {
      std::cerr << "Current node should habe name \"privatemessage\", but it is \""
                << current.getNameAsString() << "\" instead!\n";
      return false;
    }
    if (!processPrivateMessageNode(current, readPMs, newPMs, folderName, fm))
    {
      std::cerr << "Error while processing <privatemessage> element!\n";
      return false;
    }

    current = current.getNextSibling();
  }
  return true;
}

bool MessageDatabase::processPrivateMessageNode(const XMLNode& node, uint32_t& readPMs, uint32_t& newPMs, const std::string& folder, FolderMap& fm)
{
  if (!node.hasChild())
  {
    return false;
  }

  PrivateMessage pm;

  // get child node - should be private messages member data
  XMLNode current = node.getChild();
  while (current.hasNextSibling())
  {
    // skip non-element nodes (i.e. text nodes, we want element nodes only)
    while (current.hasNextSibling() && !current.isElementNode())
    {
      current = current.getNextSibling();
    }

    const std::string curName = current.getNameAsString();
    if (curName == "datestamp")
    {
      if (!pm.getDatestamp().empty())
      {
        std::cerr << "Error: More than one datestamp node in private message!\n";
        return false;
      }
      pm.setDatestamp(current.getPlainTextContent());
    } // if "datestamp"
    else if (curName == "title")
    {
      if (!pm.getTitle().empty())
      {
        std::cerr << "Error: More than one title node in private message!\n";
        return false;
      }
      pm.setTitle(current.getContentBoth());
    } // if "title"
    else if (curName == "fromuser")
    {
      if (!pm.getFromUser().empty())
      {
        std::cerr << "Error: More than one fromuser node in private message!\n";
        return false;
      }
      pm.setFromUser(current.getContentBoth());
    } // if "fromuser"
    else if (curName == "fromuserid")
    {
      if (pm.getFromUserID() != 0)
      {
        std::cerr << "Error: More than one fromuserid node in private message!\n";
        return false;
      }

      const std::string temp = current.getPlainTextContent();
      uint32_t tempUint;
      if (!(std::stringstream (temp) >> tempUint))
      {
        std::cerr << "Error: Could not convert \"" << temp << "\" to integer!\n";
        return false;
      }
      pm.setFromUserID(tempUint);
    } // if "fromuserid"
    else if (curName == "touser")
    {
      if (!pm.getToUser().empty())
      {
        std::cerr << "Error: More than one touser node in private message!\n";
        return false;
      }
      pm.setToUser(current.getContentBoth());
    } // if "touser"
    else if (curName == "message")
    {
      if (!pm.getMessage().empty())
      {
        std::cerr << "Error: More than one message node in private message!\n";
        return false;
      }
      pm.setMessage(current.getContentBoth());
    } // if "message"
    else
    {
      std::cerr << "Error: Encountered unknown node named \"" << curName << "\".\n";
      return false;
    }
    current = current.getNextSibling();
  }

  // check data members
  if (pm.getMessage().empty())
  {
    std::cerr << "Error: PM message is empty!\n";
    return false;
  }
  if (pm.getTitle().empty())
  {
    std::cerr << "Error: PM title is empty!\n";
    return false;
  }
  if (pm.getDatestamp().empty())
  {
    std::cerr << "Error: PM datestamp is empty!\n";
    return false;
  }
  if (pm.getFromUser().empty())
  {
    std::cerr << "Error: PM fromuser is empty!\n";
    return false;
  }
  if (pm.getToUser().empty())
  {
    std::cerr << "Error: PM touser is empty!\n";
    return false;
  }

  pm.normalise();

  ++readPMs;
  // add message to DB
  if (addMessage(pm))
  {
    ++newPMs;
  }
  // add entry to folder map
  if (!folder.empty())
    fm.add(pm.getHash(), folder);
  return true;
}

bool MessageDatabase::saveMessages(const std::string& directory, const Compression compression) const
{
  const std::string realDirectory(libstriezel::filesystem::slashify(directory));
  for (const auto& [hash, message]: m_Messages)
  {
    if (!message.saveToFile(realDirectory + hash.toHexString(), compression))
    {
      return false;
    }
  }
  return true;
}

bool MessageDatabase::loadMessages(const std::string& directory, uint32_t& readPMs, uint32_t& newPMs, const Compression compression)
{
  readPMs = 0;
  newPMs = 0;
  std::vector<FileEntry> files;
  if (!getDirectoryFileList(directory, files, std::string(""), false))
  {
    return false;
  }

  PrivateMessage tempPM;

  const std::string realDirectory(libstriezel::filesystem::slashify(directory));

  for (const auto& entry: files)
  {
    if (entry.IsDirectory)
    {
      continue;
    }
    if (!SHA256::isValidHash(entry.FileName))
    {
      continue;
    }
    if (!tempPM.loadFromFile(realDirectory + entry.FileName, compression))
    {
      std::cerr << "Error while loading message from file \"" << realDirectory + entry.FileName << "\"!\n";
      return false;
    }
    if (entry.FileName != tempPM.getHash().toHexString())
    {
      std::cerr << "Error: Content of message file " << realDirectory + entry.FileName << " has been altered!\n";
      return false;
    }
    ++readPMs;
    if (addMessage(tempPM))
    {
      ++newPMs;
    }
  }
  return true;
}


bool MessageDatabase::saveIndexFiles(const std::string& directory, MsgTemplate index, MsgTemplate entry, MsgTemplate folderList, MsgTemplate folderEntry, const FolderMap& fm) const
{
  std::vector<SortType> sortedList;
  for (const auto& [digest, pm]: m_Messages)
  {
    sortedList.push_back(SortType(pm.getDatestamp(), digest));
  }
  // sort PM list by datestamps - newest first
  std::sort(sortedList.begin(), sortedList.end(), ST_greater);

  // create folder maps for later use
  std::map<std::string, std::vector<SortType> > folderContents;
  while (!sortedList.empty())
  {
    if (fm.hasEntry(sortedList[0].md))
    {
      // add to folder
      folderContents[fm.getFolderName(sortedList[0].md)].push_back(SortType(sortedList[0].datestamp, sortedList[0].md));
    }
    else
    {
      // no folder, so add it with "" as folder name
      folderContents[""].push_back(SortType(sortedList[0].datestamp, sortedList[0].md));
    }
    sortedList.erase(sortedList.begin());
  }

  // folder hashes
  std::map<std::string, std::string> folderHashes;
  std::map<std::string, std::vector<SortType> >::const_iterator fcIter = folderContents.begin();
  while (fcIter != folderContents.end())
  {
    folderHashes[fcIter->first] = SHA256::computeFromBuffer((uint8_t*) fcIter->first.c_str(), fcIter->first.length()*8).toHexString();
    ++fcIter;
  }

  // generate folder entries
  std::string theEntries = "";
  fcIter = folderContents.begin();
  while (fcIter != folderContents.end())
  {
    if (fcIter->first != "")
    {
      folderEntry.addReplacement("folder_link", "folder_" + folderHashes[fcIter->first] + ".html", false);
      folderEntry.addReplacement("folder_name", fcIter->first, true);
      theEntries += folderEntry.show();
    }
    ++fcIter;
  }

  // put entries into folder list
  folderList.addReplacement("folder_entries", theEntries, false);
  const std::string cFolderList = folderList.show();

  fcIter = folderContents.begin();
  while (fcIter != folderContents.end())
  {
    theEntries.clear();
    std::vector<SortType>::const_iterator vecIter = fcIter->second.begin();
    while (vecIter != fcIter->second.end())
    {
      const PrivateMessage& currMessage = getMessage(vecIter->md);
      entry.addReplacement("date",       vecIter->datestamp, true);
      entry.addReplacement("pm_url",     vecIter->md.toHexString() + ".html", false);
      entry.addReplacement("title",      currMessage.getTitle(), true);
      entry.addReplacement("fromuserid", intToString(currMessage.getFromUserID()), true);
      entry.addReplacement("fromuser",   currMessage.getFromUser(), true);
      theEntries += entry.show();
      ++vecIter;
    }
    index.addReplacement("entries", theEntries, false);
    index.addReplacement("folders", cFolderList, false);
    const std::string indexText = index.show();
    std::ofstream indexFile;
    std::string fileName = directory;
    if (fcIter->first != "")
      fileName = fileName + "folder_" + folderHashes[fcIter->first] + ".html";
    else
      fileName = fileName + "index.html";

    indexFile.open(fileName, std::ios_base::out | std::ios_base::binary);
    if (!indexFile)
    {
      return false;
    }
    indexFile.write(indexText.c_str(), indexText.length());
    const bool success = indexFile.good();
    indexFile.close();
    if (!success)
    {
      return false;
    }
    ++fcIter;
  }
  return true;
}


std::map<md_date, std::vector<md_date> > MessageDatabase::getTextSubsets() const
{
  std::map<md_date, std::vector<md_date> > result;
  Iterator iter = m_Messages.begin();
  while (iter != m_Messages.end())
  {
    Iterator innerIter = m_Messages.begin();
    while (innerIter != m_Messages.end())
    {
      if (iter != innerIter)
      {
        if (iter->second.getMessage().find(innerIter->second.getMessage())!=std::string::npos)
        {
          result[md_date(iter->first, iter->second.getDatestamp())].push_back(md_date(innerIter->first, innerIter->second.getDatestamp()));
        }
      }
      ++innerIter;
    }
    ++iter;
  }
  return result;
}

void MessageDatabase::clear()
{
  m_Messages.clear();
}
