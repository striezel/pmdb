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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "MessageDatabase.hpp"
#include "FolderMap.hpp"
#include "ConsoleColours.hpp"
#include "ColourMap.hpp"
#include "paths.hpp"
#include "Version.hpp"
#include "filters/FilterUser.hpp"
#include "functions.hpp"
#include "html_generation.hpp"
#include "HTMLOptions.hpp"
#include "open_file.hpp"
#include "ReturnCodes.hpp"
#include "../libstriezel/filesystem/directory.hpp"
#include "../libstriezel/filesystem/file.hpp"
#include "../libstriezel/common/DirectoryFileList.hpp"
#include "../libstriezel/common/StringUtils.hpp"

void showVersion()
{
  #ifdef NO_PM_COMPRESSION
  std::cout << "Private Message Database, version 0.25.0~no-compression, 2025-10-22\n";
  #else
  std::cout << "Private Message Database, version 0.25.0, 2015-10-22\n";
  #endif
  showLibraryVersions();
  std::cout << '\n'
            << "Copyright (C) 2012, 2013, 2014, 2015, 2025  Dirk Stolle\n"
            << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
            << "This is free software: you are free to change and redistribute it under the\n"
            << "terms of the GNU General Public License version 3 or any later version.\n"
            << "There is NO WARRANTY, to the extent permitted by law.\n";
}

void showHelp(const std::string& name)
{
  std::cout << "\n" << name << '\n'
            << '\n'
            << "Manages private messages exported from vBulletin in XML format.\n"
            << '\n'
            << "options:\n"
            << "  --help            - Displays this help message and quits.\n"
            << "  -?                - same as --help\n"
            << "  --version         - Displays the version of the program and quits.\n"
            << "  -v                - same as --version\n"
            << "  -xml FILENAME     - Sets the name of the XML file that contains the private\n"
            << "                      messages to FILENAME. Must not be omitted.\n"
            << "  --xml=FILENAME    - same as -xml\n"
            << "  --load-default    - Tries to load messages from the default directory.\n"
            << "                      This option is enabled by default.\n"
            << "  --no-load-default - Prevents the program from automatically loading messages\n"
            << "                      from the default directory.\n"
            << "                      Mutually exclusive with --load-default.\n"
            << "  --load=DIR        - Tries to load all messages saved in the directory DIR.\n"
            << "                      This option can be given more than once, however the\n"
            << "                      directory has to be different every time.\n"
            << "  --save            - All messages will be saved after the XML files were read\n"
            << "                      and the messages from the load directories have been\n"
            << "                      loaded. Enabled by default.\n"
            << "  --no-save         - Prevents the program from saving any read messages.\n"
            << "                      Mutually exclusive with --save.\n"
            #ifndef NO_PM_COMPRESSION
            << "  --compress        - Save and load operations (see --save and --load) will use\n"
            << "                      compression, i.e. messages are compressed using zlib\n"
            << "                      before they are saved to files, and they will be decom-\n"
            << "                      pressed when they are loaded from files. By default,\n"
            << "                      messages will NOT be compressed for backwards compatibi-\n"
            << "                      lity with earlier pmdb versions.\n"
            << "  --no-save-check   - This option prevents the program from checking the\n"
            << "                      compression status of messages when saving to an existing\n"
            << "                      directory. Note that this is not recommended, because it\n"
            << "                      could result in a mixup where a directory contains both\n"
            << "                      compressed and uncompressed messages, making some of the\n"
            << "                      messages unreadable by the program.\n"
            #endif // NO_PM_COMPRESSION
            << "  --html            - Creates HTML files for every message.\n"
            << "  --xhtml           - Like --html, but use XHTML instead of HTML.\n"
            << "  --no-br           - Do not convert new line characters to line breaks in\n"
            << "                      (X)HTML output.\n"
            << "  --no-list         - Do not parse [LIST] codes when creating HTML files.\n"
            << "  --table=CLASS     - Sets the class for grids in <table> to CLASS.\n"
            << "                      Must occur together with --row and --cell.\n"
            << "  --row=CLASS       - Sets the class for grids in <tr> to CLASS.\n"
            << "                      Must occur together with --table and --cell.\n"
            << "  --cell=CLASS      - Sets the class for grids in <td> to CLASS.\n"
            << "                      Must occur together with --table and --row.\n"
            << "  --std-classes     - Sets the 'standard' classes for the three class options.\n"
            << "                      This is equivalent to specifying all these parameters:\n"
            << "                          --table=" << TableClasses::DefaultTableClass << "\n"
            << "                          --row=" << TableClasses::DefaultRowClass << "\n"
            << "                          --cell=" << TableClasses::DefaultCellClass << "\n"
            << "  --no-open         - Usually the program tries to open the generated HTML\n"
            << "                      files in a web browser for viewing. If this option is\n"
            << "                      given, no such attempt is made.\n"
            << "  --subset-check    - Search for messages with texts that are completely\n"
            << "                      contained in other messages.\n"
            << "  --list-from X     - List all messages that were sent by user X, where X stands\n"
            << "                      for the name of the user (not the numeric user id).\n"
            << "                      Can occur multiple times for more than one user.\n"
            << "  --list-to X       - List all messages that were sent to user X, where X stands\n"
            << "                      for the name of the user (not the numeric user id).\n"
            << "                      Can occur multiple times for more than one user.\n";
}

int main(int argc, char **argv)
{
  std::set<std::string> pathXML;
  std::set<std::string> loadDirs;
  std::optional<bool> loadDefault {};
  bool doSave = true;
  bool saveModeSpecified = false;
  Compression compression = Compression::none;
  CompressionCheck compressionCheck = CompressionCheck::Perform;

  bool doHTML = false;
  HTMLOptions htmlOptions;
  bool doNotOpen = false;

  bool searchForSubsets = false;
  std::vector<FilterUser> filters = std::vector<FilterUser>();

  if ((argc > 1) && (argv != nullptr))
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if ((param == "--help") || (param == "-?") || (param == "/?"))
        {
          showHelp(argv[0]);
          return 0;
        }
        // version information requested?
        else if ((param == "--version") || (param == "-v"))
        {
          showVersion();
          return 0;
        }
        else if ((param == "-xml") || (param == "--xml"))
        {
          if ((i + 1 < argc) && (argv[i+1] != nullptr))
          {
            const std::string pathToXML = std::string(argv[i+1]);
            if (pathXML.find(pathToXML) != pathXML.end())
            {
              std::cerr << "Parameter " << param << " must not occur more than once for the same file!\n";
              return rcInvalidParameter;
            }
            if (!libstriezel::filesystem::file::exists(pathToXML))
            {
              std::cerr << "Error: XML file " << pathToXML << " does not exist!\n";
              return rcInvalidParameter;
            }
            pathXML.insert(pathToXML);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "XML file \"" << pathToXML << "\" was queued for loading.\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param << "\".\n";
            return rcInvalidParameter;
          }
        }
        else if ((param.substr(0,6) == "--xml=") && (param.length() > 6))
        {
          const std::string pathToXML = param.substr(6);
          if (pathXML.find(pathToXML) != pathXML.end())
          {
            std::cerr << "Parameter --xml must not occur more than once for the same XML file!\n";
            return rcInvalidParameter;
          }
          if (!libstriezel::filesystem::file::exists(pathToXML))
          {
            std::cerr << "Error: XML file " << pathToXML << " does not exist!\n";
            return rcInvalidParameter;
          }
          pathXML.insert(pathToXML);
          std::cout << "XML file \"" << pathToXML << "\" was queued for loading.\n";
        } // param == xml (single parameter version)
        else if (param == "--save")
        {
          if (saveModeSpecified)
          {
            std::cerr << "Parameter --save must not occur more than once and "
                      << "is mutually exclusive with --no-save!\n";
            return rcInvalidParameter;
          }
          doSave = true;
          saveModeSpecified = true;
          std::cout << "Files will be saved as requested via " << param << ".\n";
        }
        else if ((param == "--no-save") || (param == "--nosave"))
        {
          if (saveModeSpecified)
          {
            std::cerr << "Parameter " << param << " must not occur more than once "
                      << "and is mutually exclusive with --save!\n";
            return rcInvalidParameter;
          }
          doSave = false;
          saveModeSpecified = true;
          std::cout << "Files will NOT be saved as requested via " << param << ".\n";
        }//param == no-save
        else if (param == "--load-default")
        {
          const std::string defaultMessageDirectory = pmdb::paths::messages() + libstriezel::filesystem::pathDelimiter;
          if (loadDirs.find(defaultMessageDirectory) != loadDirs.end())
          {
            std::cerr << "Parameter --load-default must not occur more than once!\n";
            return rcInvalidParameter;
          }
          if (loadDefault.has_value())
          {
            std::cerr << "Error: Parameter " << param << " cannot be used "
                      << "together with --no-load-default.\n";
            return rcInvalidParameter;
          }
          loadDefault = true;
          if (!libstriezel::filesystem::directory::exists(defaultMessageDirectory))
          {
            std::cout << "Default message directory " << defaultMessageDirectory
                      << " does not exist, so it will not be loaded.\n";
          }
          else
          {
            loadDirs.insert(defaultMessageDirectory);
            std::cout << "Directory \"" << defaultMessageDirectory << "\" was queued for loading.\n";
          }
        }
        else if (param == "--no-load-default")
        {
          if (loadDefault.has_value())
          {
            if (loadDefault.value())
            {
              std::cerr << "Error: Parameter " << param << " cannot be used "
                      << "together with --load-default.\n";
              return rcInvalidParameter;
            }
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          loadDefault = false;
          std::cout << "Info: Attempt to load from default directory will be skipped.\n";
        }
        else if ((param.substr(0,7) == "--load=") && (param.length() > 7))
        {
          std::string pathToDir = param.substr(7);
          // Does it have a trailing (back)slash?
          pathToDir = libstriezel::filesystem::slashify(pathToDir);
          if (loadDirs.find(pathToDir) != loadDirs.end())
          {
            std::cerr << "Parameter --load must not occur more than once for the same directory!\n";
            return rcInvalidParameter;
          }
          if (!libstriezel::filesystem::directory::exists(libstriezel::filesystem::unslashify(pathToDir)))
          {
            std::cerr << "Error: Directory " << pathToDir << " does not exist!\n";
            return rcInvalidParameter;
          }
          loadDirs.insert(pathToDir);
          std::cout << "Directory \"" << pathToDir << "\" was queued for loading.\n";
        }//param == 'load=...'
        else if ((param == "--html") || (param == "--HTML"))
        {
          if (doHTML)
          {
            std::cerr << "Parameter " << param << " must not occur more than once "
                      << "or in combination with --xhtml!\n";
            return rcInvalidParameter;
          }
          doHTML = true;
        }//param == html
        else if ((param == "--xhtml") || (param == "--XHTML"))
        {
          if (doHTML || htmlOptions.standard == HTMLStandard::XHTML)
          {
            std::cerr << "Parameter " << param << " must not occur more than once "
                      << "or in combination with --html!\n";
            return rcInvalidParameter;
          }
          doHTML = true;
          htmlOptions.standard = HTMLStandard::XHTML;
        }//param == xhtml
        else if ((param == "--no-br") || (param == "--no-breaks"))
        {
          if (!htmlOptions.nl2br)
          {
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          htmlOptions.nl2br = false;
        }//param == no-br
        else if (param == "--no-list")
        {
          if (htmlOptions.noList)
          {
            std::cerr << "Parameter --no-list must not occur more than once!\n";
            return rcInvalidParameter;
          }
          htmlOptions.noList = true;
        }//param == no-list
        else if ((param == "--compress") || (param == "--compression") || (param == "--zlib"))
        {
          #ifndef NO_PM_COMPRESSION
          if (compression != Compression::none)
          {
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          compression = Compression::zlib;
          std::cout << "Files loading/saving will use compression as requested via " << param << ".\n";
          #else
          std::cerr << "Error: Compression is not available in this build of pmdb!\n";
          return rcInvalidParameter;
          #endif // NO_PM_COMPRESSION
        }//param == compression
        else if ((param == "--no-save-check") || (param == "--skip-save-check"))
        {
          if (compressionCheck == CompressionCheck::Skip)
          {
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          compressionCheck = CompressionCheck::Skip;
        }
        else if ((param.substr(0,8) == "--table=") && (param.length() > 8))
        {
          htmlOptions.tableClasses.table = param.substr(8);
          htmlOptions.tableClasses.useClasses = true;
        }//param == 'table=...'
        else if ((param.substr(0,6) == "--row=") && (param.length() > 6))
        {
          htmlOptions.tableClasses.row = param.substr(6);
          htmlOptions.tableClasses.useClasses = true;
        }//param == 'row=...'
        else if ((param.substr(0,7) == "--cell=") && (param.length() > 7))
        {
          htmlOptions.tableClasses.cell = param.substr(7);
          htmlOptions.tableClasses.useClasses = true;
        }//param == 'cell=...'
        else if ((param == "--std-classes") || (param == "--classes") || (param == "--default-classes"))
        {
          htmlOptions.tableClasses.table      = TableClasses::DefaultTableClass;
          htmlOptions.tableClasses.row        = TableClasses::DefaultRowClass;
          htmlOptions.tableClasses.cell       = TableClasses::DefaultCellClass;
          htmlOptions.tableClasses.useClasses = true;
        }//param == std-classes
        else if (param == "--no-open")
        {
          if (doNotOpen)
          {
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          doNotOpen = true;
        }
        else if ((param == "--subset-check") || (param == "--redundant-check"))
        {
          if (searchForSubsets)
          {
            std::cerr << "Parameter " << param << " must not occur more than once!\n";
            return rcInvalidParameter;
          }
          searchForSubsets = true;
        }//param == subset-check
        else if (param == "--list-from")
        {
          if ((i + 1 < argc) && (argv[i+1] != nullptr))
          {
            const std::string user = std::string(argv[i+1]);
            const FilterUser f(user, UserType::Sender, Match::FullName);
            filters.push_back(f);
            ++i; // skip next parameter, because it's used as user name already
            std::cout << "Added PMs from user \"" << user << "\" to filter criteria.\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a user name after \""
                      << param << "\".\n";
            return rcInvalidParameter;
          }
        }//param == list-from-user
        else if (param == "--list-to")
        {
          if ((i + 1 < argc) && (argv[i+1] != nullptr))
          {
            const std::string user = std::string(argv[i+1]);
            const FilterUser f(user, UserType::Recipient, Match::FullName);
            filters.push_back(f);
            ++i; // skip next parameter, because it's used as user name already
            std::cout << "Added PMs to user \"" << user << "\" to filter criteria.\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a user name after \""
                      << param << "\".\n";
            return rcInvalidParameter;
          }
        }//param == list-to-user
        else
        {
          // unknown or wrong parameter
          std::cerr << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return rcInvalidParameter;
      }
      ++i; // on to next parameter
    } // while
  } // if arguments present

  // Load default message directory, if it exists.
  if (loadDefault.value_or(true))
  {
    const auto defaultMessageDirectory = pmdb::paths::messages();
    if ((loadDirs.find(defaultMessageDirectory + libstriezel::filesystem::pathDelimiter) == loadDirs.end())
      && libstriezel::filesystem::directory::exists(defaultMessageDirectory))
    {
      loadDirs.insert(defaultMessageDirectory + libstriezel::filesystem::pathDelimiter);
    }
  }

  // No files to load?
  if (pathXML.empty() && loadDirs.empty())
  {
    std::cerr << "Error: The program needs some input data to work with, for "
              << "example an XML file\ncontaining saved messages or a directory "
              << "which contains previously saved\nmessages. To load messages "
              << "from an XML file use the --xml command-line option.\n"
              << "For example, to load messages from the file ";
    #if defined(_WIN32)
    constexpr auto path = "C:\\path\\to\\messages.xml";
    #else
    constexpr auto path = "/path/to/messages.xml";
    #endif
    std::cerr << path << " type\n\n    pmdb --xml " << path << "\n\n"
              << "For more information use --help to get a list of all valid parameters.\n";
    return rcInvalidParameter;
  }

  if (htmlOptions.tableClasses.useClasses)
  {
    if (htmlOptions.tableClasses.table.empty() || htmlOptions.tableClasses.row.empty()
        || htmlOptions.tableClasses.cell.empty())
    {
      std::cerr << "If at least one of the parameters --table, --row or --cell"
                << " is given, the other two have to be specified, too!\n";
      return rcInvalidParameter;
    }
  }

  MessageDatabase mdb;
  FolderMap fm;
  uint32_t PMs_done, PMs_new;

  // try to load data from directories
  for (const auto& directory: loadDirs)
  {
    std::cout << "Loading messages from " << directory << " ...\n";
    if (!mdb.loadMessages(directory, PMs_done, PMs_new, compression))
    {
      std::cerr << "Could not load all messages from \"" << directory
                << "\"!\nRead so far: " << PMs_done << "; new: " << PMs_new
                << "\n";
      return rcFileError;
    }
    std::cout << "All messages from " << directory << " loaded. Read: "
              << PMs_done << "; new: " << PMs_new << "\n";
    // try to load folder map, too, but don't return, if it failed
    if (fm.load(directory))
    {
      std::cout << "Loaded folder map from " << directory << ", too.\n";
    }
  }

  // try to load XML files
  for (const auto& path: pathXML)
  {
    if (mdb.importFromFile(path, PMs_done, PMs_new, fm))
    {
      std::cout << "Import of private messages from " << path << " was successful.\n  "
                << PMs_done << " PMs read, new PMs: " << PMs_new << "\n";
    }
    else
    {
      std::cerr << "Error: Import of private messages from " << path << " failed!\n"
                << "  PMs read from file so far: " << PMs_done << "\nNew PMs: " << PMs_new << "\n";
      return rcFileError;
    }
  }

  std::cout << "PMs in the database: " << mdb.getNumberOfMessages() << "\n";

  if (doSave)
  {
    const int rc = saveMessages(mdb, fm, compression, compressionCheck);
    if (rc != 0)
    {
      return rc;
    }
  } // if save requested

  if (doHTML)
  {
    const int rc = generateHtmlFiles(mdb, fm, htmlOptions);
    if (rc != 0)
    {
      return rc;
    }
    if (!doNotOpen)
    {
      // Open first HTML file in browser.
      openFirstIndexFile(fm, pmdb::paths::html());
    }
  } // if HTML file generation was requested

  if (searchForSubsets)
  {
    ColourMap cMap;
    const std::string pathToColourMap = pmdb::paths::colourmap();
    if (libstriezel::filesystem::file::exists(pathToColourMap))
    {
      if (!cMap.loadFromFile(pathToColourMap))
      {
        std::cerr << "Error: Failed to load colour map from " << pathToColourMap << "!\n";
        return rcFileError;
      }
    }

    std::cout << "Searching for message texts that are contained in others. This may take a while...\n";
    std::map<md_date, std::vector<md_date> > subsets = mdb.getTextSubsets();
    std::map<md_date, std::vector<md_date> >::iterator subIter = subsets.begin();
    std::set<SHA256::MessageDigest> redundantMessages;
    while (subIter != subsets.end())
    {
      try
      {
        const PrivateMessage & pm = mdb.getMessage(subIter->first.md);
        std::cout << "Message \"" << pm.getTitle() << "\" of " << pm.getDatestamp();
        if (fm.hasEntry(subIter->first.md))
        {
          std::cout << " in \"" << cMap.colouredFolder(fm.getFolderName(subIter->first.md)) << "\"";
        }
      }
      catch (std::exception& ex)
      {
        std::cerr << "\nCaught exception: " << ex.what() << "\n.";
        return rcCaughtException;
      }
      std::cout << " contains the following " << subIter->second.size() << " message(s):\n";
      std::sort(subIter->second.begin(), subIter->second.end());
      std::vector<md_date>::const_iterator secondIter = subIter->second.begin();
      while (secondIter != subIter->second.end())
      {
        try
        {
          const PrivateMessage & contained = mdb.getMessage(secondIter->md);
          std::cout << "    \"" << contained.getTitle() << "\" of " << contained.getDatestamp();
          if (fm.hasEntry(secondIter->md))
            std::cout << " (in \"" << cMap.colouredFolder(fm.getFolderName(secondIter->md)) << "\")";
          std::cout << "\n";
        }
        catch (std::exception& except)
        {
          std::cerr << "Caught exception: " << except.what() << "\n.";
          return rcCaughtException;
        }
        redundantMessages.insert(secondIter->md);
        ++secondIter;
      } // while (inner)
      ++subIter;
    } // while (outer, subIter)

    if (!redundantMessages.empty())
    {
      const unsigned int current = mdb.getNumberOfMessages();
      const unsigned int redundant = static_cast<unsigned int>(redundantMessages.size());
      std::cout << "Information: " << redundant
                << " message(s) can be deleted without information loss, "
                << "reducing the message count from currently " << current
                << " message(s) to " << (current-redundant) << " message(s)."
                << std::endl;
    } // if redundant message(s)
  } // if search for duplicates/subsets

  // list messages by given filter conditions
  if (!filters.empty())
  {
    showFilteredMessages(mdb, fm, filters);
  }

  return 0;
}
