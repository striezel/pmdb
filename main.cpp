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
#include "MessageDatabase.h"

//return codes
const int rcInvalidParameter = 1;

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
  std::cout << "Private Message Database, version 0.02, 2012-08-25\n";
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
            << "                     messages to FILENAME. Must not be omitted.\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  std::string pathToXML = "";
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
            if (!pathToXML.empty())
            {
              std::cout << "Parameter "<<param<<" must not occur more than once!\n";
              return rcInvalidParameter;
            }
            pathToXML = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "XML file was set to \""<<pathToXML<<"\".\n";
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
          if (!pathToXML.empty())
          {
            std::cout << "Parameter --xml must not occur more than once!\n";
            return rcInvalidParameter;
          }
          pathToXML = param.substr(6);
          std::cout << "XML file was set to \""<<pathToXML<<"\".\n";
        }//param == xml (single parameter version)
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

  if (pathToXML.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  MessageDatabase mdb;
  uint32_t PMs_done;
  if (mdb.importFromFile(pathToXML, PMs_done))
  {
    std::cout << "import success!\n";
  }
  else
  {
    std::cout << "import failed!\n";
  }
  std::cout << "PMs read so far: "<<PMs_done<<"\n";

  std::cout << "Nothing more to be done here yet. That's what you get for trying a very early version of that programme.\n";

  return 0;
}
