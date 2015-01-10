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

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "MessageDatabase.hpp"
#include "FolderMap.hpp"
#include "Config.hpp"
#include "ConsoleColours.h"
#include "ColourMap.hpp"
#include "bbcode/BBCodeParser.hpp"
#include "bbcode/BBCode_Table.hpp"
#include "bbcode/DefaultCodes.hpp"
#include "bbcode/HorizontalRuleBBCode.hpp"
#include "bbcode/ListBBCode.hpp"
#include "filters/FilterUser.hpp"
#include "libthoro/common/DirectoryFunctions.h"
#include "libthoro/common/DirectoryFileList.h"
#include "libthoro/common/StringUtils.h"

//return codes
const int rcInvalidParameter = 1;
const int rcFileError        = 2;
const int rcCaughtException  = 3;

void showGPLNotice()
{
  std::cout << "Private Message Database\n"
            << "  Copyright (C) 2012, 2013, 2014  Thoronador\n"
            << "\n"
            << "  This programme is free software: you can redistribute it and/or\n"
            << "  modify it under the terms of the GNU General Public License as published\n"
            << "  by the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This programme is distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this programme.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  showGPLNotice();
  std::cout << "Private Message Database, version 0.23, 2014-11-07\n";
}

void showHelp(const std::string& name)
{
  std::cout << "\n"<<name<<"\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -v               - same as --version\n"
            << "  -xml FILENAME    - sets the name of the XML file that contains the private\n"
            << "                     messages to FILENAME. Must not be omitted.\n"
            << "  --xml=FILENAME   - same as -xml\n"
            << "  --load           - tries to load messages from the default directory.\n"
            << "  --load=DIR       - tries to load all messages saved in the directory DIR.\n"
            << "                     This option can be given more than once, however the\n"
            << "                     directory has to be different every time.\n"
            << "  --save           - all messages will be saved after the XML files were read\n"
            << "                     and the messages from the load directories have been\n"
            << "                     loaded. Enabled by default.\n"
            << "  --no-save        - prevents the programme from saving any read meassages.\n"
            << "                     Mutually exclusive with --save.\n"
            << "  --html           - creates HTML files for every message.\n"
            << "  --xhtml          - like --html, but use XHTML instead of HTML\n"
            << "  --no-br          - do not convert new line characters to line breaks in\n"
            << "                     (X)HTML output.\n"
            << "  --no-list        - do not parse [LIST] codes when creating HTML files.\n"
            << "  --table=CLASS    - sets the class for grids in <table> to CLASS.\n"
            << "                     Must occur together with --row and --cell.\n"
            << "  --row=CLASS      - sets the class for grids in <tr> to CLASS.\n"
            << "                     Must occur together with --table and --cell.\n"
            << "  --cell=CLASS     - sets the class for grids in <td> to CLASS.\n"
            << "                     Must occur together with --table and --row.\n"
            << "  --std-classes    - sets the 'standard' classes for the three class options.\n"
            << "                     This is equivalent to specifying all these parameters:\n"
            << "                         --table="<<TableBBCode::DefaultTableClass<<"\n"
            << "                         --row="<<TableBBCode::DefaultRowClass<<"\n"
            << "                         --cell="<<TableBBCode::DefaultCellClass<<"\n"
            << "  --subset-check   - search for messages with texts that are completely\n"
            << "                     contained in other messages, too.\n"
            << "  --list-from X    - list all messages that were sent by user X.\n"
            << "                     Can occur multiple times for more than one user.\n"
            << "  --list-to X      - list all messages that were sent to user X.\n"
            << "                     Can occur multiple times for more than one user.\n";
}

int main(int argc, char **argv)
{
  std::set<std::string> pathXML;
  std::set<std::string> loadDirs;
  bool doSave = true;
  bool saveModeSpecified = false;
  std::string homeDirectory;
  std::string defaultSaveDirectory;

  if (getHomeDirectory(homeDirectory))
  {
    homeDirectory = slashify(homeDirectory);
    defaultSaveDirectory = homeDirectory + std::string(".pmdb") + Thoro::pathDelimiter;
  }
  else
  {
    homeDirectory = std::string(".") + Thoro::pathDelimiter;
    defaultSaveDirectory = std::string(".pmdb") + Thoro::pathDelimiter;
  }
  bool doHTML = false;
  bool forceXHTML = false;
  bool nl2br = true;
  bool noList = false;

  bool useTableClasses = false;
  std::string classTable;
  std::string classRow;
  std::string classCell;

  bool searchForSubsets = false;
  std::vector<FilterUser> filters = std::vector<FilterUser>();

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-?") or (param=="/?"))
        {
          showHelp(argv[0]);
          return 0;
        }//param == help
        //version information requested?
        else if ((param=="--version") or (param=="-v"))
        {
          showVersion();
          return 0;
        }//param == version
        else if ((param=="-xml") or (param=="--xml"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            const std::string pathToXML = std::string(argv[i+1]);
            if (pathXML.find(pathToXML)!=pathXML.end())
            {
              std::cout << "Parameter "<<param<<" must not occur more than once for the same file!\n";
              return rcInvalidParameter;
            }
            pathXML.insert(pathToXML);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "XML file \""<<pathToXML<<"\" was chained for loading.\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }//param == xml
        else if ((param.substr(0,6)=="--xml=") and (param.length()>6))
        {
          const std::string pathToXML = param.substr(6);
          if (pathXML.find(pathToXML)!=pathXML.end())
          {
            std::cout << "Parameter --xml must not occur more than once for the same XML file!\n";
            return rcInvalidParameter;
          }
          pathXML.insert(pathToXML);
          std::cout << "XML file \""<<pathToXML<<"\" was chained for loading.\n";
        }//param == xml (single parameter version)
        else if (param=="--save")
        {
          if (saveModeSpecified)
          {
            std::cout << "Parameter --save must not occur more than once and "
                      << "is mutually exclusive with --no-save!\n";
            return rcInvalidParameter;
          }
          doSave = true;
          saveModeSpecified = true;
          std::cout << "Files will be saved as requested via "<<param<<".\n";
        }//param == save
        else if ((param=="--no-save") or (param=="--nosave"))
        {
          if (saveModeSpecified)
          {
            std::cout << "Parameter "<<param<<" must not occur more than once "
                      << "and is mutually exclusive with --save!\n";
            return rcInvalidParameter;
          }
          doSave = false;
          saveModeSpecified = true;
          std::cout << "Files will NOT be saved as requested via "<<param<<".\n";
        }//param == no-save
        else if (param=="--load")
        {
          if (loadDirs.find(defaultSaveDirectory)!=loadDirs.end())
          {
            std::cout << "Parameter --load must not occur more than once!\n";
            return rcInvalidParameter;
          }
          loadDirs.insert(defaultSaveDirectory);
          std::cout << "Directory \""<<defaultSaveDirectory<<"\" was chained for loading.\n";
        }//param == load
        else if ((param.substr(0,7)=="--load=") and (param.length()>7))
        {
          std::string pathToDir = param.substr(7);
          pathToDir = slashify(pathToDir); //Does it have a trailing (back)slash?
          if (loadDirs.find(pathToDir)!=loadDirs.end())
          {
            std::cout << "Parameter --load must not occur more than once for the same directory!\n";
            return rcInvalidParameter;
          }
          loadDirs.insert(pathToDir);
          std::cout << "Directory \""<<pathToDir<<"\" was chained for loading.\n";
        }//param == 'load=...'
        else if (param=="--html")
        {
          if (doHTML)
          {
            std::cout << "Parameter --html must not occur more than once!\n";
            return rcInvalidParameter;
          }
          doHTML = true;
        }//param == html
        else if ((param=="--xhtml") or (param=="--XHTML"))
        {
          if (doHTML or forceXHTML)
          {
            std::cout << "Parameter "<<param<<" must not occur more than once "
                      << "or in combination with --html!\n";
            return rcInvalidParameter;
          }
          doHTML = true;
          forceXHTML = true;
        }//param == xhtml
        else if ((param=="--no-br") or (param=="--no-breaks"))
        {
          if (!nl2br)
          {
            std::cout << "Parameter "<<param<<" must not occur more than once!\n";
            return rcInvalidParameter;
          }
          nl2br = false;
        }//param == no-br
        else if (param=="--no-list")
        {
          if (noList)
          {
            std::cout << "Parameter --no-list must not occur more than once!\n";
            return rcInvalidParameter;
          }
          noList = true;
        }//param == no-list
        else if ((param.substr(0,8)=="--table=") and (param.length()>8))
        {
          classTable = param.substr(8);
          useTableClasses = true;
        }//param == 'table=...'
        else if ((param.substr(0,6)=="--row=") and (param.length()>6))
        {
          classRow = param.substr(6);
          useTableClasses = true;
        }//param == 'row=...'
        else if ((param.substr(0,7)=="--cell=") and (param.length()>7))
        {
          classCell = param.substr(7);
          useTableClasses = true;
        }//param == 'cell=...'
        else if ((param=="--std-classes") or (param=="--classes") or (param=="--default-classes"))
        {
          classTable = TableBBCode::DefaultTableClass;
          classRow   = TableBBCode::DefaultRowClass;
          classCell  = TableBBCode::DefaultCellClass;
          useTableClasses = true;
        }//param == std-classes
        else if ((param=="--subset-check") or (param=="--redundant-check"))
        {
          if (searchForSubsets)
          {
            std::cout << "Parameter "<<param<<" must not occur more than once!\n";
            return rcInvalidParameter;
          }
          searchForSubsets = true;
        }//param == subset-check
        else if ((param=="--list-from"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            const std::string user = std::string(argv[i+1]);
            const FilterUser f(user, true, true);
            filters.push_back(f);
            ++i; //skip next parameter, because it's used as user name already
            std::cout << "Added PMs from user \""<<user<<"\" to filter criteria.\n";
          }
          else
          {
            std::cout << "Error: You have to specify a user name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }//param == list-from-user
        else if ((param=="--list-to"))
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            const std::string user = std::string(argv[i+1]);
            const FilterUser f(user, false, true);
            filters.push_back(f);
            ++i; //skip next parameter, because it's used as user name already
            std::cout << "Added PMs to user \""<<user<<"\" to filter criteria.\n";
          }
          else
          {
            std::cout << "Error: You have to specify a user name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }//param == list-to-user
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present

  //no files to load?
  if (pathXML.empty() and loadDirs.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  if (useTableClasses)
  {
    if (classTable.empty() or classRow.empty() or classCell.empty())
    {
      std::cout << "If at least one of the parameters --table, --row or --cell"
                << " is given, the other two have to be specified, too!\n";
      return rcInvalidParameter;
    }
  }//if

  MessageDatabase mdb;
  FolderMap fm;
  uint32_t PMs_done, PMs_new;
  std::set<std::string>::const_iterator set_iter = pathXML.begin();
  while (set_iter!=pathXML.end())
  {
    if (mdb.importFromFile(*set_iter, PMs_done, PMs_new, fm))
    {
      std::cout << "Import of private messages from \""<< *set_iter <<"\" was successful!\n  "
                << PMs_done<<" PMs read, new PMs: "<<PMs_new<<"\n";
    }
    else
    {
      std::cout << "Import of private messages from \""<< *set_iter <<"\" failed!\n"
                << "  PMs read from file so far: "<<PMs_done<<"\nNew PMs: "<<PMs_new<<"\n";
    }
    ++set_iter;
  }//while

  //try to load data from directories
  set_iter = loadDirs.begin();
  while (set_iter!=loadDirs.end())
  {
    if (!mdb.loadMessages(*set_iter, PMs_done, PMs_new))
    {
      std::cout << "Could not load all messages from \""<< *set_iter <<"\"!\nRead so far: "<<PMs_done<<"; new: "<<PMs_new<<"\n";
      return 0;
    }
    std::cout << "All messages from \""<< *set_iter <<"\" loaded! Read: "<<PMs_done<<"; new: "<<PMs_new<<"\n";
    //try to load folder map, too, but don't return, if it failed
    if (fm.load(*set_iter))
    {
      std::cout << "Loaded folder map from \""<< *set_iter <<"\".\n";
    }
    ++set_iter;
  }//while

  std::cout << "PMs in the database: "<<mdb.getNumberOfMessages()<<"\n";

  if (doSave)
  {
    //directory creation - only necessary, if there are any messages
    if (mdb.getNumberOfMessages()!=0)
    {
      const std::string realDir = unslashify(defaultSaveDirectory);
      if (!directoryExists(realDir))
      {
        std::cout << "Trying to create save directory \""<<realDir<<"\"...";
        if (!createDirectoryRecursive(realDir))
        {
          std::cout <<"failed!\nAborting.\n";
          return 0;
        }
        std::cout << "success!\n";
      }
    }//if more than zero messages

    if (!mdb.saveMessages(slashify(defaultSaveDirectory)))
    {
      std::cout << "Could not save messages!\n";
      return 0;
    }
    std::cout << "Messages saved successfully!\n";
    if (!fm.save(slashify(defaultSaveDirectory)))
    {
      std::cout << "Could not save folder map!\n";
      return 0;
    }
    std::cout << "Folder map saved successfully!\n";
  }//if save requested

  if (doHTML)
  {
    MessageDatabase::Iterator msgIter = mdb.getBegin();
    if (msgIter!=mdb.getEnd())
    {
      //directory creation
      std::string htmlDir = slashify(defaultSaveDirectory)+"html";
      if (!directoryExists(htmlDir))
      {
        std::cout << "Trying to create HTML directory \""<<htmlDir<<"\"...";
        if (!createDirectoryRecursive(htmlDir))
        {
          std::cout <<"failed!\nAborting.\n";
          return 0;
        }
        std::cout << "success!\n";
      }//if html directory does not exist
      htmlDir = slashify(htmlDir);

      BBCodeParser parser;
      Config conf;
      conf.setForumURL("http://www.example.com/forum/");
      conf.setTPLFile("message.tpl");
      //try to load configuration file
      if (FileExists(defaultSaveDirectory+"pmdb.conf"))
      {
        if (!conf.loadFromFile(defaultSaveDirectory+"pmdb.conf"))
        {
          std::cout << "Could not load pmdb.conf, using default/incomplete values instead.\n";
          conf.setTPLFile("message.tpl");
        }
        else std::cout << "Loading pmdb.conf was successful.\n";
      }

      #ifndef NO_SMILIES_IN_PARSER
      const std::vector<Smilie>& smilies_from_config = conf.getSmilies();
      std::vector<Smilie>::const_iterator iter = smilies_from_config.begin();
      for (; iter!= smilies_from_config.end(); ++iter)
      {
        parser.addSmilie(*iter);
      } //for
      #endif

      //load template for HTML files
      MsgTemplate theTemplate;
      if (!theTemplate.loadFromFile(conf.getTPL()))
      {
        std::cout << "Error: could not load template file \""<<conf.getTPL()<<"\" for messages!\n";
        return rcFileError;
      }


      /* prepare BB code parser with BB codes */
      //image tags
      CustomizedSimpleBBCode img_simple("img", "<img border=\"0\" src=\"",
                                        forceXHTML ? "\" alt=\"\" />" : "\" alt=\"\">");

      MsgTemplate tpl;
      //thread tag - simple variant
      tpl.loadFromString("<a target=\"_blank\" href=\""+conf.getForumURL()+"showthread.php?t={..inner..}\">"
                        +conf.getForumURL()+"showthread.php?t={..inner..}</a>");
      SimpleTemplateBBCode thread_simple("thread", tpl, "inner");
      //thread tag - advanced variant
      tpl.loadFromString("<a target=\"_blank\" href=\""+conf.getForumURL()+"showthread.php?t={..attr..}\">{..inner..}</a>");
      AdvancedTemplateBBCode thread_advanced("thread", tpl, "inner", "attr");
      //wiki tag
      tpl.loadFromString("<a href=\"http://de.wikipedia.org/wiki/{..inner..}\" target=\"_blank\" title=\"Wikipediareferenz zu '{..inner..}'\">{..inner..}</a>");
      SimpleTemplateBBCode wiki("wiki", tpl, "inner");
      //tag for unordered lists
      ListBBCode list_unordered("list", true);
      //tag for tables
      TableBBCode table("table", useTableClasses, classTable, classRow, classCell);
      //hr code
      HorizontalRuleBBCode hr("hr", forceXHTML);

      bbcode_default::addDefaultCodes(parser);
      parser.addCode(&img_simple);
      parser.addCode(&thread_simple);
      parser.addCode(&thread_advanced);
      parser.addCode(&wiki);
      if (!noList) parser.addCode(&list_unordered);
      parser.addCode(&table);
      parser.addCode(&hr);

      KillSpacesBeforeNewline eatRedundantSpaces;
      ListNewlinePreProcessor preProc_List;
      TablePreprocessor table_killLF("tr", "td");
      parser.addPreProcessor(&eatRedundantSpaces);
      if (nl2br and !noList) parser.addPreProcessor(&preProc_List);
      if (nl2br) parser.addPreProcessor(&table_killLF);

      //create HTML files
      std::cout << "Creating HTML files for message texts. This may take a while...\n";
      theTemplate.addReplacement("forum_url", conf.getForumURL(), false);
      while (msgIter!=mdb.getEnd())
      {
        theTemplate.addReplacement("date", msgIter->second.getDatestamp(), true);
        theTemplate.addReplacement("title", msgIter->second.getTitle(), true);
        theTemplate.addReplacement("fromuser", msgIter->second.getFromUser(), true);
        theTemplate.addReplacement("fromuserid", intToString(msgIter->second.getFromUserID()), true);
        theTemplate.addReplacement("touser", msgIter->second.getToUser(), true);
        theTemplate.addReplacement("message", parser.parse(msgIter->second.getMessage(), conf.getForumURL(), forceXHTML, nl2br), false);
        const std::string output = theTemplate.show();
        std::ofstream htmlFile;
        htmlFile.open((htmlDir+msgIter->first.toHexString()+".html").c_str(),
                      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if (!htmlFile.is_open())
        {
          std::cout << "Failed to open file "<<htmlDir<<msgIter->first.toHexString()<<".html!\n";
          return rcFileError;
        }
        htmlFile.write(output.c_str(), output.length());
        if (!htmlFile.good())
        {
          std::cout << "Error while writing to file "<<htmlDir<<msgIter->first.toHexString()<<".html!\n";
          htmlFile.close();
          return rcFileError;
        }
        htmlFile.close();
        ++msgIter;
      }//while
      //create index file
      MsgTemplate tplIndex, tplEntry, tplFolderList, tplFolderEntry;
      if (!tplIndex.loadFromFile("folder.tpl"))
      {
        std::cout << "Could not load folder.tpl!\n";
        return rcFileError;
      }
      if (!tplEntry.loadFromFile("index_entry.tpl"))
      {
        std::cout << "Could not load index_entry.tpl!\n";
        return rcFileError;
      }
      if (!tplFolderList.loadFromFile("folder_list.tpl"))
      {
        std::cout << "Could not load folder_list.tpl!\n";
        return rcFileError;
      }
      if (!tplFolderEntry.loadFromFile("folder_entry.tpl"))
      {
        std::cout << "Could not load folder_entry.tpl!\n";
        return rcFileError;
      }
      tplIndex.addReplacement("forum_url", conf.getForumURL(), true);
      tplEntry.addReplacement("forum_url", conf.getForumURL(), true);
      if (!mdb.saveIndexFiles(htmlDir, tplIndex, tplEntry, tplFolderList, tplFolderEntry, fm))
      {
        std::cout << "Could not write index.html!\n";
        return rcFileError;
      }
      std::cout << "All HTML files were created successfully!\n";
    }//if
    else
    {
      std::cout << "There are no messages, thus no HTML files were created.\n";
    }
  }//if doHTML

  if (searchForSubsets)
  {
    ColourMap cMap;
    const std::string pathToColourMap = defaultSaveDirectory + "pmdb.colourmap";
    if (FileExists(pathToColourMap))
    {
      if (!cMap.loadFromFile(pathToColourMap))
      {
        std::cout << "Error: failed to load colour map from " << pathToColourMap << "!\n";
        return rcFileError;
      }
    }//if colour map file exists

    std::cout << "Searching for message texts that are contained in others. This may take a while...\n";
    std::map<md_date, std::vector<md_date> > subsets = mdb.getTextSubsets();
    std::map<md_date, std::vector<md_date> >::iterator subIter = subsets.begin();
    while (subIter!=subsets.end())
    {
      try
      {
        const PrivateMessage & pm = mdb.getMessage(subIter->first.md);
        std::cout << "Message \""<<pm.getTitle()<<"\" of "<<pm.getDatestamp();
        if (fm.hasEntry(subIter->first.md))
        {
          std::cout << " in \"" << cMap.colouredFolder(fm.getFolderName(subIter->first.md)) << "\"";
        }//if
      }
      catch (std::exception& ex)
      {
        std::cout << "\nCaught exception: " << ex.what() << "\n.";
        return rcCaughtException;
      }//try-catch
      std::cout << " contains the following "<<subIter->second.size() <<" message(s):\n";
      std::sort(subIter->second.begin(), subIter->second.end());
      std::vector<md_date>::const_iterator secondIter = subIter->second.begin();
      while (secondIter!=subIter->second.end())
      {
        try
        {
          const PrivateMessage & contained = mdb.getMessage(secondIter->md);
          std::cout << "    \""<< contained.getTitle()<<"\" of "<<contained.getDatestamp();
          if (fm.hasEntry(secondIter->md))
            std::cout << " (in \"" << cMap.colouredFolder(fm.getFolderName(secondIter->md)) << "\")";
          std::cout <<"\n";
        }
        catch (std::exception& except)
        {
          std::cout << "Caught exception: " << except.what() << "\n.";
          return rcCaughtException;
        }
        ++secondIter;
      }//while (inner)
      ++subIter;
    }//while (outer, subIter)
  }//if search for duplicates/subsets

  //list messages by given filter conditions
  if (!filters.empty())
  {
    std::vector<SortType> matches;
    MessageDatabase::Iterator msgIter = mdb.getBegin();
    while (msgIter != mdb.getEnd())
    {
      std::vector<FilterUser>::const_iterator filterIter = filters.begin();
      while (filterIter != filters.end())
      {
        if (filterIter->match(msgIter->second))
        {
          matches.push_back( md_date(msgIter->second.getDatestamp(), msgIter->first));
          filterIter = filters.end();
        }
        else
        {
          ++filterIter;
        }
      }//while (inner)
      ++msgIter;
    }//while

    std::sort(matches.begin(), matches.end());
    std::cout << "Filtered messages:\n";
    std::vector<SortType>::const_iterator mdIter = matches.begin();
    while (mdIter != matches.end())
    {
      const PrivateMessage & pm = mdb.getMessage(mdIter->md);
      std::cout << "Message \""<<pm.getTitle()<<"\" of "<<pm.getDatestamp();
      if (fm.hasEntry(mdIter->md))
      {
        std::cout << " in \"" << fm.getFolderName(mdIter->md) << "\"";
      }//if
      std::cout << std::endl;
      ++mdIter;
    }//while
    if (matches.size()==0)
      std::cout << "  no matches\n";
    else
      std::cout << "Total: "<<matches.size()<<"\n";
  }//if filters are set

  return 0;
}
