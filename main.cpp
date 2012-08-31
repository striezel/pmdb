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

#include <iostream>
#include <set>
#include <string>
#include "MessageDatabase.h"
#include "libthoro/common/DirectoryFunctions.h"

//return codes
const int rcInvalidParameter = 1;

#if defined(_WIN32)
  const char cDirSeparator = '\\';
#elif defined(__linux__) || defined(linux)
  const char cDirSeparator = '/';
#else
  #error Unknown operating system!
#endif

const std::string defaultSaveDirectory = std::string(".pmdb")+cDirSeparator;

void showGPLNotice()
{
  std::cout << "Private Message Database\n"
            << "  Copyright (C) 2012 Thoronador\n"
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
  std::cout << "Private Message Database, version 0.06, 2012-08-29\n";
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
            << "                     Mutually exclusive with --save.\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::set<std::string> pathXML;
  std::set<std::string> loadDirs;
  bool doSave = true;
  bool saveModeSpecified = false;
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
          //Does it have a trailing (back)slash?
          if (pathToDir[pathToDir.length()-1]!=cDirSeparator)
          {
            pathToDir = pathToDir + cDirSeparator;
          }
          if (loadDirs.find(pathToDir)!=loadDirs.end())
          {
            std::cout << "Parameter --load must not occur more than once for the same directory!\n";
            return rcInvalidParameter;
          }
          loadDirs.insert(pathToDir);
          std::cout << "Directory \""<<pathToDir<<"\" was chained for loading.\n";
        }//param == 'load=...'
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

  if (pathXML.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  MessageDatabase mdb;
  uint32_t PMs_done, PMs_new;
  std::set<std::string>::const_iterator set_iter = pathXML.begin();
  while (set_iter!=pathXML.end())
  {
    if (mdb.importFromFile(*set_iter, PMs_done, PMs_new))
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
    ++set_iter;
  }//while

  std::cout << "PMs in the database: "<<mdb.getNumberOfMessages()<<"\n";

  if (doSave)
  {
    //directory creation - only necessary, if there are any messages
    if (mdb.getNumberOfMessages()!=0)
    {
      const std::string realDir = (defaultSaveDirectory[defaultSaveDirectory.length()-1]==cDirSeparator)
                                   ?  defaultSaveDirectory.substr(0, defaultSaveDirectory.length()-1)
                                   : defaultSaveDirectory;
      if (!directoryExists(realDir))
      {
        std::cout << "Trying to create directory \""<<realDir<<"\"...";
        if (!createDirectoryRecursive(realDir))
        {
          std::cout <<"failed!\nAborting.\n";
          return 0;
        }
        std::cout << "success!\n";
      }
    }//if more than zero messages

    if (!mdb.saveMessages(defaultSaveDirectory))
    {
      std::cout << "Could not save messages!\n";
      return 0;
    }
    std::cout << "Messages saved successfully!\n";
  }//if save requested

  return 0;
}
