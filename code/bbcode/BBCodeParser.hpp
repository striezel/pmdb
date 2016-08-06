/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2012, 2013, 2014  Dirk Stolle

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

#ifndef BBCODEPARSER_HPP
#define BBCODEPARSER_HPP


/* Note on C-preprocessor constants:
     The BBCodeParser code recognizes certain preprocessor constants.
     - NO_PREPROCESSORS_IN_PARSER
           If defined, the BBCodeParser won't contains text preprocessors.
     - NO_POSTPROCESSORS_IN_PARSER
           If defined, the BBCodeParser won't contains text postprocessors.
     - NO_SMILIES_IN_PARSER
           If defined, the BBCodeParser won't have the capabilities to handle
           smilie structs. (I guess one could still substitute smilies via a
           custom BBCode class in that case.)
*/
#include <string>
#include <vector>
#include "BBCode.hpp"
#ifndef NO_SMILIES_IN_PARSER
  #include "Smilie.hpp"
#endif
#if !defined(NO_PREPROCESSORS_IN_PARSER) && !defined(NO_POSTPROCESSORS_IN_PARSER)
#include "TextProcessor.hpp"
#endif

/** \brief BBCodeParser:
       transforms a given text containing BB codes to HTML code via the
       parse() function. All codes that should be replaced have to be added
       before parsing. Use addCode() for that. Smilies added via addSmilie()
       will be replaced, too.
*/
class BBCodeParser
{
  public:
    /** constructor */
    BBCodeParser();


    /** \brief transforms BB codes in text to HTML codes (still incomplete)
     *
     * \param text       the original text
     * \param forumURL   the base URL of the forum (some BB codes might require
                         this URL for proper transformation of code to HTML)
     * \param isXHTML    if set to true, smilie transformations will produce XHTML
                         image tags
     * \param nl2br      if set to true, new line characters will be converted to
                         the corresponding (X)HTML code for line breaks
     * \return Returns the transformed/parsed text.
     */
    std::string parse(std::string text, const std::string& forumURL, const bool isXHTML, const bool nl2br) const;


    #ifndef NO_PREPROCESSORS_IN_PARSER
    /** \brief adds a new text preprocessor to the parser
     *
     * \param preProc  pointer to the text processor object that should be added
     * \remarks The passed pointers must live for the whole lifetime of the
           BBCodeParser instance they are passed to. Otherwise parse() will
           fail. Alternatively call clearPreProcessors() before you delete/free
           those TextProcessor objects.

           The parsing process during parse() calls will handle preprocessors
           in the order they are passed to addPreProcessor().

           Passing identical TextProcessor objects to this function without
           clearing old preprocessor in between will not do any harm, it just
           causes this TextProcessors to be applied twice.

           Preprocessors are applied before the BB codes are handled.
     */
    void addPreProcessor(TextProcessor* preProc);
    #endif


    /** \brief adds a new bb code to the parser
     *
     * \param code   pointer to the BB code object that should be added
     * \remarks
           The passed pointers must live for the whole lifetime of the
           BBCodeParser instance they are passed to. Otherwise parse() will
           fail. Alternatively call clearCodes() before you delete/free those
           BBCode objects.

           The parsing process during parse() calls will handle BBCodes in the
           order they are passed to addCode().

           Passing identical BBCode objects to this function without clearing
           old codes in between will not do any harm, it just causes this codes
           to be applied twice - although the second pass should not find any
           codes to replace, because the first one already took care of that.
    */
    void addCode(BBCode* code);


    #ifndef NO_SMILIES_IN_PARSER
    /** \brief adds a new smilie to the parser
     *
     * \param sm   the smilie
     */
    void addSmilie(const Smilie& sm);
    #endif


    #ifndef NO_POSTPROCESSORS_IN_PARSER
    /** \brief adds a new text postprocessor to the parser
     *
     * \param postProc   pointer to the text processor object that should be added
     *
     * \remarks
     * The passed pointers must live for the whole lifetime of the
     * BBCodeParser instance they are passed to. Otherwise parse() will
     * fail. Alternatively call clearPostProcessors() before you delete/free
     * those TextProcessor objects.
     *
     * The parsing process during parse() calls will handle postprocessors
     * in the order they are passed to addPostProcessor().
     *
     * Passing identical TextProcessor objects to this function without
     * clearing old postprocessor in between will not do any harm, it just
     * causes this TextProcessor to be applied twice.
     *
     * Postprocessors are applied after the BB codes are handled.
     */
    void addPostProcessor(TextProcessor* postProc);
    #endif

    #ifndef NO_PREPROCESSORS_IN_PARSER
    /** clears all added preprocessors */
    inline void clearPreProcessors()
    {
      m_PreProcs.clear();
    }
    #endif


    /** \brief clears all added BB codes
     *
     * \remarks
     *  Clearing codes right before calling parse() without any addCode()
     *  calls in between will result in no BB codes being parsed. However,
     *  smilies might still get parsed.
     */
    void clearCodes();


    #ifndef NO_SMILIES_IN_PARSER
    /** \brief clears all added smilies
     *
     * \remarks
     * Clearing smilies right before calling parse() without any addSmile()
     * calls in between will result in no smilies being parsed. However,
     * BB codes might still get parsed, if they are present.
     */
    void clearSmilies();
    #endif

    #ifndef NO_POSTPROCESSORS_IN_PARSER
    /** clears all added postprocessors */
    inline void clearPostProcessors()
    {
      m_PostProcs.clear();
    }
    #endif
  private:
    #ifndef NO_PREPROCESSORS_IN_PARSER
    std::vector<TextProcessor*> m_PreProcs;
    #endif
    #ifndef NO_SMILIES_IN_PARSER
    std::vector<Smilie>  m_Smilies;
    #endif
    #ifndef NO_POSTPROCESSORS_IN_PARSER
    std::vector<TextProcessor*> m_PostProcs;
    #endif
    std::vector<BBCode*> m_Codes;
};//class

#endif // BBCODEPARSER_HPP
