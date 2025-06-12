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

#ifndef XMLDOCUMENT_HPP
#define XMLDOCUMENT_HPP

#include <string>
#include <libxml/tree.h>
#include "XMLNode.hpp"

/** \brief This is a C++-style wrapper class for libxml(2)'s xmlDoc type.
*/
class XMLDocument
{
  public:
    /** \brief constructor
     *
     * \param fileName   path to the XML document that shall be wrapped inside this class
     */
    XMLDocument(const std::string& fileName);


    /** \brief Creates a document from a libxml2 document pointer.
     *
     * \param doc  a valid (non-null) xmlDocPtr
     * \remarks The created XMLDocument instance takes ownership of the passed
     * pointer and frees it in its destructor. Therefore, do NOT free that
     * pointer manually!
     */
    XMLDocument(xmlDocPtr doc);


    XMLDocument(const XMLDocument& op);
    XMLDocument& operator=(const XMLDocument& op);


    ~XMLDocument();


    /** Returns true, if the document is parsed (i.e. a valid document). */
    bool isParsed() const;

    /** \brief Checks whether the document is empty.
     *
     * \return Returns true, if the document is empty.
     */
    bool isEmpty() const;

    /** \brief Checks whether the document is well-formed.
     *
     * \return Returns true, if the document is well-formed.
     *         Returns false otherwise.
     */
    bool wellFormed() const;

    /** \brief Gets the document's root node.
     *
     * \return Returns the document's root node.
     * \throws std::invalid_argument Throws an exception, if the document is
     *         either empty or not parsed.
     */
    XMLNode getRootNode() const;
  private:
    xmlDocPtr m_Doc;
}; // class

#endif // XMLDOCUMENT_HPP
