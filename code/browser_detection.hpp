/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2025  Dirk Stolle

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

#ifndef PMDB_BROWSER_DETECTION_HPP
#define PMDB_BROWSER_DETECTION_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

/// enumeration for browser type
enum class Browser
{
  /// Firefox browser or compatible forks
  Firefox,

  /// Chromium browser
  Chromium
};

struct BrowserInformation
{
  std::filesystem::path path;  /**< path to executable */
  Browser type;                /**< general browser type */
};

/** \brief Detects an available web browser.
 *
 * \return Returns information about a web browser, if one was found.
           Returns an empty optional, if no browser was found.
 * \remark If the system has more than one browser installed, the function will
 *         just return information about one of them, usually the first that it
 *         can find.
 */
std::optional<BrowserInformation> detect_browser();

/** \brief Get additional parameters for browser executable.
 *
 * \param type   the type of browser
 * \return Returns a vector of parameters for the browser executable.
 */
std::vector<std::string> additional_parameters(const Browser type);

#endif // PMDB_BROWSER_DETECTION_HPP
