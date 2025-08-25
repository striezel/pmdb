/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2016, 2025  Dirk Stolle

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

#include "html_generation.hpp"
#include <fstream>
#include <iostream>
#include "browser_detection.hpp"
#include "Config.hpp"
#include "paths.hpp"
#include "ReturnCodes.hpp"
#include "bbcode/BBCodeParser.hpp"
#include "bbcode/DefaultCodes.hpp"
#include "bbcode/HorizontalRuleBBCode.hpp"
#include "bbcode/ListBBCode.hpp"
#include "bbcode/SpoilerBBCode.hpp"
#include "bbcode/TableBBCode.hpp"
#include "templates/functions.hpp"
#include "../libstriezel/filesystem/directory.hpp"
#include "../libstriezel/filesystem/file.hpp"
#include "../libstriezel/hash/sha256/BufferSourceUtility.hpp"

std::string getFirstFolderFileName(const FolderMap& fm)
{
  const auto folders = fm.getPresentFolders();
  if (folders.empty())
  {
    return "";
  }
  const std::string& name = *folders.begin();
  return "folder_"
    + SHA256::computeFromBuffer((uint8_t*) name.c_str(), name.length() * 8).toHexString()
    + ".html";
}

void openFirstIndexFile(const FolderMap& fm, const std::string& html_dir)
{
  auto fileName = getFirstFolderFileName(fm);
  if (fileName.empty())
  {
    return;
  }
  const auto fullFileName = html_dir + fileName;
  const auto browser = detect_browser();
  if (!browser.has_value())
  {
    std::cout << "Info: Could not find a browser to open generated files.\n"
              << "Info: Open " << fullFileName << " in a browser to see the generated HTML files.\n";
    return;
  }
  // TODO: Open file via Boost Process.
  std::cout << "Run\n\n\t" << browser.value().path.string();
  const auto params = additional_parameters(browser.value().type);
  for (const auto& param: params)
  {
    std::cout << " " << param;
  }
  std::cout << " " << fullFileName << "\n\n"
            << "to show the generated HTML output.\n";
}

int generateHtmlFiles(const MessageDatabase& mdb, const FolderMap& fm, const HTMLOptions htmlOptions)
{
  MessageDatabase::Iterator msgIter = mdb.getBegin();
  if (msgIter == mdb.getEnd())
  {
    std::cout << "There are no messages, thus no HTML files were created.\n";
    return 0;
  }

  // directory creation
  std::string htmlDir = pmdb::paths::html();
  if (!libstriezel::filesystem::directory::exists(htmlDir))
  {
    std::cout << "Trying to create HTML directory \"" << htmlDir << "\" ...";
    if (!libstriezel::filesystem::directory::createRecursive(htmlDir))
    {
      std::cout << " failed!\nAborting.\n";
      return rcFileError;
    }
    std::cout << " success!\n";
  } // if html directory does not exist
  htmlDir = libstriezel::filesystem::slashify(htmlDir);

  BBCodeParser parser;
  Config conf;
  conf.setForumURL("http://www.example.com/forum/");
  // try to load configuration file
  const std::string conf_path = pmdb::paths::conf();
  if (libstriezel::filesystem::file::exists(conf_path))
  {
    if (!conf.loadFromFile(conf_path))
    {
      std::cout << "Could not load pmdb.conf, using default/incomplete values instead.\n";
    }
    else
      std::cout << "Loading pmdb.conf was successful.\n";
  }

  #ifndef NO_SMILIES_IN_PARSER
  const std::vector<Smilie>& smilies_from_config = conf.getSmilies();
  for (const Smilie& smilie: smilies_from_config)
  {
    parser.addSmilie(smilie);
  }
  #endif

  // Ensure that template files exist.
  const int tpl_exit_code = pmdb::tpl::ensureFilesExist();
  if (tpl_exit_code != 0)
  {
    return tpl_exit_code;
  }

  // load template for HTML files
  MsgTemplate theTemplate;
  const auto template_directory = libstriezel::filesystem::slashify(pmdb::paths::templates());
  if (!theTemplate.loadFromFile(template_directory + "message.tpl"))
  {
    std::cerr << "Error: Could not load template file \"" << template_directory
              << "message.tpl\" for messages!\n";
    return rcFileError;
  }


  /* prepare BB code parser with BB codes */
  // image tags
  CustomizedSimpleBBCode img_simple("img", "<img border=\"0\" src=\"",
                                    htmlOptions.standard == HTMLStandard::XHTML ? "\" alt=\"\" />" : "\" alt=\"\">");

  MsgTemplate tpl;
  // thread tag - simple variant
  tpl.loadFromString("<a target=\"_blank\" href=\"" + conf.getForumURL() + "showthread.php?t={..inner..}\">"
                    + conf.getForumURL() + "showthread.php?t={..inner..}</a>");
  SimpleTemplateBBCode thread_simple("thread", tpl, "inner");
  // thread tag - advanced variant
  tpl.loadFromString("<a target=\"_blank\" href=\"" + conf.getForumURL() + "showthread.php?t={..attr..}\">{..inner..}</a>");
  AdvancedTemplateBBCode thread_advanced("thread", tpl, "inner", "attr");
  // wiki tag
  tpl.loadFromString("<a href=\"http://de.wikipedia.org/wiki/{..inner..}\" target=\"_blank\" title=\"Wikipediareferenz zu '{..inner..}'\">{..inner..}</a>");
  SimpleTemplateBBCode wiki("wiki", tpl, "inner");
  // tag for unordered lists
  ListBBCode list_unordered("list", true);
  // tag for tables
  TableBBCode table("table", htmlOptions.tableClasses);
  // hr code
  HorizontalRuleBBCode hr("hr", htmlOptions.standard);
  // spoiler code (old style without JavaScript)
  SpoilerBBCode spoiler("spoiler");

  bbcode_default::addDefaultCodes(parser);
  parser.addCode(&img_simple);
  parser.addCode(&thread_simple);
  parser.addCode(&thread_advanced);
  parser.addCode(&wiki);
  if (!htmlOptions.noList)
  {
    parser.addCode(&list_unordered);
  }
  parser.addCode(&table);
  parser.addCode(&hr);
  parser.addCode(&spoiler);

  KillSpacesBeforeNewline eatRedundantSpaces;
  ListNewlinePreProcessor preProc_List;
  TablePreProcessor table_killLF("tr", "td");
  parser.addPreProcessor(&eatRedundantSpaces);
  if (htmlOptions.nl2br && !htmlOptions.noList)
  {
    parser.addPreProcessor(&preProc_List);
  }
  if (htmlOptions.nl2br)
  {
    parser.addPreProcessor(&table_killLF);
  }

  // create HTML files
  std::cout << "Creating HTML files for message texts. This may take a while...\n";
  theTemplate.addReplacement("forum_url", conf.getForumURL(), false);
  while (msgIter != mdb.getEnd())
  {
    theTemplate.addReplacement("date", msgIter->second.getDatestamp(), true);
    theTemplate.addReplacement("title", msgIter->second.getTitle(), true);
    theTemplate.addReplacement("fromuser", msgIter->second.getFromUser(), true);
    theTemplate.addReplacement("fromuserid", intToString(msgIter->second.getFromUserID()), true);
    theTemplate.addReplacement("touser", msgIter->second.getToUser(), true);
    theTemplate.addReplacement("message", parser.parse(msgIter->second.getMessage(), conf.getForumURL(), htmlOptions.standard, htmlOptions.nl2br), false);
    const std::string output = theTemplate.show();
    std::ofstream htmlFile;
    htmlFile.open(htmlDir + msgIter->first.toHexString() + ".html",
                  std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!htmlFile.is_open())
    {
      std::cout << "Failed to open file " << htmlDir << msgIter->first.toHexString() << ".html!\n";
      return rcFileError;
    }
    htmlFile.write(output.c_str(), output.length());
    if (!htmlFile.good())
    {
      std::cerr << "Error while writing to file " << htmlDir << msgIter->first.toHexString() << ".html!\n";
      htmlFile.close();
      return rcFileError;
    }
    htmlFile.close();
    ++msgIter;
  } // while
  // create index file
  MsgTemplate tplIndex, tplEntry, tplFolderList, tplFolderEntry;
  if (!tplIndex.loadFromFile(template_directory + "folder.tpl"))
  {
    std::cerr << "Could not load folder.tpl!\n";
    return rcFileError;
  }
  if (!tplEntry.loadFromFile(template_directory + "index_entry.tpl"))
  {
    std::cerr << "Could not load index_entry.tpl!\n";
    return rcFileError;
  }
  if (!tplFolderList.loadFromFile(template_directory + "folder_list.tpl"))
  {
    std::cerr << "Could not load folder_list.tpl!\n";
    return rcFileError;
  }
  if (!tplFolderEntry.loadFromFile(template_directory + "folder_entry.tpl"))
  {
    std::cerr << "Could not load folder_entry.tpl!\n";
    return rcFileError;
  }
  tplIndex.addReplacement("forum_url", conf.getForumURL(), true);
  tplEntry.addReplacement("forum_url", conf.getForumURL(), true);
  if (!mdb.saveIndexFiles(htmlDir, tplIndex, tplEntry, tplFolderList, tplFolderEntry, fm))
  {
    std::cerr << "Could not write index.html!\n";
    return rcFileError;
  }
  std::cout << "All HTML files were created successfully!\n";
  // Open first HTML file in browser.
  openFirstIndexFile(fm, htmlDir);
  return 0;
}
