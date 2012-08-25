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

#ifndef XMLNODE_H
#define XMLNODE_H

#include <string>
#include <libxml/tree.h>

/* class XMLDocument:
         This is a C++-style wrapper class for libxml(2)'s xmlNodePtr type
*/
class XMLNode
{
  public:
    /* constructor

       parameters:
           node - the libxml node pointer we are wrapping in this class
    */
    XMLNode(const xmlNodePtr node);

    /* returns the node's name */
    const xmlChar* getName() const;
    std::string getNameAsString() const;

    /* returns true, if the node has at least one child node */
    inline bool hasChild() const
    {
      return (m_Node->children!=NULL);
    }

    /* returns true, if the node has at least one more sibling */
    inline bool hasNextSibling() const
    {
      return (m_Node->next!=NULL);
    }

    /* returns true, if the node has at least one previous sibling */
    inline bool hasPrevSibling() const
    {
      return (m_Node->prev!=NULL);
    }

    /* returns true, if the node has a parent node */
    inline bool hasParent() const
    {
      return (m_Node->parent!=NULL);
    }

    /* returns the first child of the node. Throws an exception, if there is no
       child node.
    */
    XMLNode getChild() const;

    /* returns the next sibling of the node. Throws an exception, if there is no
       next sibling.
    */
    XMLNode getNextSibling() const;

    /* returns the previous sibling of the node. Throws an exception, if there
       is no previous sibling.
    */
    XMLNode getPrevSibling() const;

    /* returns the parent of the node. Throws an exception, if there is no
       parent node.
    */
    XMLNode getParent() const;

    /* returns true, if the node is an element node */
    inline bool isElementNode() const
    {
      return (m_Node->type==XML_ELEMENT_NODE);
    }

    /* returns true, if the node is an attribute node */
    inline bool isAttributeNode() const
    {
      return (m_Node->type==XML_ATTRIBUTE_NODE);
    }

    /* returns true, if the node is a text node */
    bool isTextNode() const
    {
      return (m_Node->type==XML_TEXT_NODE);
    }

    /* returns the text contained in the node, if it's a text node */
    std::string getPlainTextContent() const;

    /* returns the text contained in the node's CDATA section */
    std::string getCDATAText() const;

    /* get content for both - either CDATA or text node */
    std::string getContentBoth() const;
  private:
    xmlNodePtr m_Node;
};//class

#endif // XMLNODE_H
