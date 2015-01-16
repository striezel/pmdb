/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2014  Thoronador

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

#ifndef CONSOLECOLOURS_HPP
#define CONSOLECOLOURS_HPP

#include <string>

const std::string colourDefault = "\033[0m";

//escape sequences for font colour codes
const std::string colourBlack   = "\033[30m";
const std::string colourRed     = "\033[31m";
const std::string colourGreen   = "\033[32m";
const std::string colourYellow  = "\033[33m";
const std::string colourBlue    = "\033[34m";
const std::string colourMagenta = "\033[35m";
const std::string colourCyan    = "\033[36m";
const std::string colourWhite   = "\033[37m";

//escape sequences for font background colour codes
const std::string backgroundBlack   = "\033[40m";
const std::string backgroundRed     = "\033[41m";
const std::string backgroundGreen   = "\033[42m";
const std::string backgroundYellow  = "\033[43m";
const std::string backgroundBlue    = "\033[44m";
const std::string backgroundMagenta = "\033[45m";
const std::string backgroundCyan    = "\033[46m";
const std::string backgroundWhite   = "\033[47m";

/** \brief tries to find a control sequence for the given colour
 *
 * \param name the name of the colour
 * \param background If true, the control sequence for the background colour is returned.
 *                   Otherwise the foreground/font colour is returned.
 * \return Returns control sequence in case of success. Returns empty string, if an error occurred.
 */
std::string nameToControlSequence(const std::string& name, const bool background = false);

#endif // CONSOLECOLOURS_HPP
