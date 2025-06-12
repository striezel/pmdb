/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2014, 2015, 2025  Dirk Stolle

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

#include "XMLDocument.hpp"
#include <stdexcept>

XMLDocument::XMLDocument(const std::string& fileName)
: // parse the XML file
  m_Doc(xmlParseFile(fileName.c_str()))
{}

XMLDocument::XMLDocument(xmlDocPtr doc)
: // use existing pointer
  m_Doc(doc)
{}

XMLDocument::XMLDocument(const XMLDocument& op)
: m_Doc(op.m_Doc == nullptr ? nullptr : xmlCopyDoc(op.m_Doc, 1))
{
}

XMLDocument& XMLDocument::operator=(const XMLDocument& op)
{
  if (this == &op)
  {
    return *this;
  }
  xmlFreeDoc(m_Doc);
  m_Doc = nullptr;
  if (op.m_Doc != nullptr)
  {
    m_Doc = xmlCopyDoc(op.m_Doc, 1);
  }
  return *this;
}

XMLDocument::~XMLDocument()
{
  xmlFreeDoc(m_Doc);
}

bool XMLDocument::isParsed() const
{
  return m_Doc != nullptr;
}

bool XMLDocument::isEmpty() const
{
  return xmlDocGetRootElement(m_Doc) == nullptr;
}

bool XMLDocument::wellFormed() const
{
  if (nullptr != m_Doc)
    return ((m_Doc->properties & XML_DOC_WELLFORMED) != 0);
  return false;
}

XMLNode XMLDocument::getRootNode() const
{
  if (!isParsed() || isEmpty())
    throw std::invalid_argument("XMLDocument::getRootNode(): XML document must be parsed and not empty, if you want to get the root node!");
  return XMLNode(xmlDocGetRootElement(m_Doc));
}
