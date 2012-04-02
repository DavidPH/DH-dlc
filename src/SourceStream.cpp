//-----------------------------------------------------------------------------
//
// Copyright(C) 2009-2012 David Hill
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
//
// Low-level processing of source code.
//
//-----------------------------------------------------------------------------

#include "SourceStream.hpp"

#include "option.hpp"

#include <fstream>
#include <vector>


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

static option::option_data<std::vector<std::string> > option_include_dir
('i', "include-dir", "input",
 "Specifies a directory to search for includes in.", NULL,
 std::vector<std::string>(1));

static option::option_data<int> option_tab_columns
('\0', "tab-columns", "input",
 "How many columns a tab counts for in error reporting.", NULL, 1);


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// SourceStream::SourceStream
//
SourceStream::SourceStream(std::string const &_filename, unsigned type)
 : in(NULL), filename(_filename), delIn(true)
{
   initialize(type);

   if (!in)
   {
      std::vector<std::string>::iterator dir;
      for (dir  = option_include_dir.data.begin();
           dir != option_include_dir.data.end() && !in; ++dir)
      {
         in = new std::ifstream((*dir + filename).c_str());

         if (!*in)
         {
            delete in;
            in = NULL;
         }
      }
   }

   if (!in) throw std::exception();
}

//
// SourceStream::SourceStream
//
SourceStream::SourceStream
(std::istream &_in, std::string const &_filename, unsigned type)
 : in(&_in), filename(_filename), delIn(false)
{
   initialize(type);

   if (!in) throw std::exception();
}

//
// SourceStream::~SourceStream
//
SourceStream::~SourceStream()
{
   if (delIn) delete in;
}

//
// SourceStream::get
//
char SourceStream::get()
{
   if (!ungetStack.empty())
   {
      char c = ungetStack.top();
      ungetStack.pop();
      return c;
   }

   while (true)
   {
      oldC = curC;
      curC = newC != -2 ? newC : in->get();
      newC = in->get();


      // \t has special counting
      if (curC == '\t')
         countColumn += option_tab_columns.data;
      else
         ++countColumn;

      // \n end of line
      if (curC == '\n')
      {
         inComment = false;
         countColumn = 0;
         ++countLine;
      }

      // ; line comment start
      if (curC == ';' && doCommentASM && !isInComment() && !isInQuote())
      {
         inComment = true;
      }

      // // line comment start?
      if (curC == '/' && newC == '/' && doCommentCPP && !isInComment() && !isInQuote())
      {
         inComment = true;
      }

      // { brace open
      if (curC == '{' && !isInComment() && !isInQuote())
         ++depthBrace;

      // } brace close
      if (curC == '}' && !isInComment() && !isInQuote())
         --depthBrace;

      // /* comment start?
      if (curC == '/' && newC == '*' && doCommentC && !isInComment() && !isInQuote())
      {
         ++depthComment;
      }

      // */ commend end?
      if (curC == '*' && newC == '/' && doCommentC && !inComment && depthComment && !isInQuote())
      {
         --depthComment;
      }

      // " double quote
      if (curC == '"' && doQuoteDouble && !isInComment() && !inQuoteSingle)
      {
         inQuoteDouble = !inQuoteDouble;
      }

      // " single quote
      if (curC == '\'' && doQuoteSingle && !isInComment() && !inQuoteDouble)
      {
         inQuoteSingle = !inQuoteSingle;
      }

      // whitespace
      inWhitespace = isspace(curC);


      // If stripping is disabled, return now.
      if (!doStrip)
         return curC;

      // If in brace, don't strip.
      if (doStripAuto && depthBrace)
         return curC;


      // Comments are stripped.
      if (doStripComment && isInComment())
         continue;

      // End of multi-line comments are stripped, unless quoted.
      if (doStripComment && ((curC == '*' && newC == '/') || (oldC == '*' && curC == '/')) && !isInQuote())
         continue;

      // Unquoted whitespace is stripped.
      if (doStripWhitespace && inWhitespace && !isInQuote())
         continue;


      // Unescaped quotes are stripped.
      if (doStripQuote && (curC == '"' || curC == '\''))
         continue;


      // Escape sequence.
      if (isInQuote() && curC == '\\')
      {
         int _newC = newC;
         newC = -2;
         ++countColumn;

         switch (_newC)
         {
         case 'r': curC = '\r'; break;
         case 'n': curC = '\n'; break;
         case 't': curC = '\t'; break;

         case '\n':
            countColumn = 0;
            ++countLine;
            curC = _newC;
            break;

         case '\t':
            countColumn += option_tab_columns.data-1;
         case '\\':
         case '\'':
         case '"':
         case ' ':
            curC = _newC;
            break;

         case 'x':
         case 'X':
            curC = 0;

            #define IORDIGIT()             \
            switch (in->get())             \
            {                              \
            default:                       \
            case '0': curC |= 0x00; break; \
            case '1': curC |= 0x01; break; \
            case '2': curC |= 0x02; break; \
            case '3': curC |= 0x03; break; \
            case '4': curC |= 0x04; break; \
            case '5': curC |= 0x05; break; \
            case '6': curC |= 0x06; break; \
            case '7': curC |= 0x07; break; \
            case '8': curC |= 0x08; break; \
            case '9': curC |= 0x09; break; \
            case 'A': curC |= 0x0A; break; \
            case 'B': curC |= 0x0B; break; \
            case 'C': curC |= 0x0C; break; \
            case 'D': curC |= 0x0D; break; \
            case 'E': curC |= 0x0E; break; \
            case 'F': curC |= 0x0F; break; \
            case 'a': curC |= 0x0a; break; \
            case 'b': curC |= 0x0b; break; \
            case 'c': curC |= 0x0c; break; \
            case 'd': curC |= 0x0d; break; \
            case 'e': curC |= 0x0e; break; \
            case 'f': curC |= 0x0f; break; \
            }

            IORDIGIT();
            curC <<= 4;
            IORDIGIT();

            #undef IORDIGIT

         default:
            newC = _newC;
            break;
         }
      }


      if (!*in) throw EndOfStream();
      return static_cast<char>(curC);
   }
}

//
// SourceStream::getbrace
//
std::string SourceStream::getbrace()
{
   std::string body;

   if (!depthBrace)
      return body;

   int targetDepthBrace = depthBrace - 1;

   // Need to stop stripping in braces for this function.
   bool saveDoStripAuto = doStripAuto;
   doStripAuto = true;

   while (true)
   {
      char nextChar = get();

      if (depthBrace == targetDepthBrace)
         break;

      body += nextChar;
   }

   // Restore configuration.
   doStripAuto = saveDoStripAuto;

   return body;
}

//
// SourceStream::initialize
//
void SourceStream::initialize(unsigned type)
{
   oldC = -2;
   curC = -2;
   newC = -2;

   countColumn = 0;
   countLine   = 1;

   depthBrace   = 0;
   depthComment = 0;

   inComment = false;

   inQuoteDouble = false;
   inQuoteSingle = false;

   switch (type)
   {
   case ST_C:
      doCommentASM = false;
      doCommentC   = true;
      doCommentCPP = true;

      doQuoteDouble = true;
      doQuoteSingle = true;

      doStrip           = true;
      doStripAuto       = true;
      doStripComment    = true;
      doStripQuote      = true;
      doStripWhitespace = true;

      doStrip           = true;
      doStripAuto       = false;
      doStripComment    = true;
      doStripQuote      = false;
      doStripWhitespace = false;
      break;

   case ST_DDL:
      doCommentASM = false;
      doCommentC   = true;
      doCommentCPP = true;

      doQuoteDouble = true;
      doQuoteSingle = true;

      doStrip           = true;
      doStripAuto       = true;
      doStripComment    = true;
      doStripQuote      = true;
      doStripWhitespace = true;
      break;

   case ST_DHLX:
      doCommentASM = false;
      doCommentC   = true;
      doCommentCPP = true;

      doQuoteDouble = true;
      doQuoteSingle = true;

      doStrip           = true;
      doStripAuto       = false;
      doStripComment    = true;
      doStripQuote      = false;
      doStripWhitespace = false;
   break;
   }
}

//
// SourceStream::unget
//
void SourceStream::unget(char c)
{
   ungetStack.push(c);
}

// EOF

