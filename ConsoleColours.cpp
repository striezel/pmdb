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

#include "ConsoleColours.h"

std::string nameToControlSequence(const std::string& name, const bool background)
{
  if (name=="default")
    return colourDefault;
  if (name=="black")
    return background ? backgroundBlack : colourBlack;
  if (name=="blue")
    return background ? backgroundBlue : colourBlue;
  if (name == "cyan")
    return background ? backgroundCyan : colourCyan;
  if (name=="green")
    return background ? backgroundGreen : colourGreen;
  if (name=="magenta")
    return background ? backgroundMagenta : colourMagenta;
  if (name=="red")
    return background ? backgroundRed : colourRed;
  if (name=="white")
    return background ? backgroundWhite : colourWhite;
  if (name=="yellow")
    return background ? backgroundYellow : colourYellow;
  return "";
}
