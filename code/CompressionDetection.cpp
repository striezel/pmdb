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

#include "CompressionDetection.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include "../libstriezel/hash/sha256/sha256.hpp"

std::optional<Compression> detect_compression(const std::string& directory)
{
  namespace fs = std::filesystem;

  std::error_code error;

  auto iter = fs::directory_iterator(directory, fs::directory_options::skip_permission_denied, error);
  if (error)
  {
    return std::optional<Compression>();
  }

  for (const auto& entry: iter)
  {
    if (!SHA256::isValidHash(entry.path().filename().string()))
    {
      continue;
    }

    std::ifstream input(entry.path(), std::ios::in | std::ios::binary);
    if (!input.good())
    {
      return std::optional<Compression>();
    }

    uint32_t size = 0;
    uint32_t zlib_signature = 0;
    input.read(reinterpret_cast<char*>(&size), 4);
    input.read(reinterpret_cast<char*>(&zlib_signature), 4);
    if (!input.good())
    {
      input.close();
      return std::optional<Compression>();
    }
    input.close();
    // PM texts in vB are stored as MEDIUMTEXT, so they only have up to 24 bit
    // for the size. Furthermore, compression contains "78 DA" hex sequence.
    if ((size <= 0x00ffffffUL) && ((zlib_signature & 0x0000ffffUL) == 0xDA78))
    {
      return Compression::zlib;
    }
    else
    {
      return Compression::none;
    }
  }

  // End of directory content reached without matching file.
  return std::optional<Compression>();
}
