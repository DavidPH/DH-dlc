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

#ifndef HPP_SourceStream_
#define HPP_SourceStream_

#include <istream>
#include <stack>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// SourceStream
//
class SourceStream
{
public:
   //
   // ::EndOfStream
   //
   // Thrown when attempting to read past the end of the stream.
   //
   class EndOfStream : public std::exception
   {
   };

   enum SourceType
   {
      ST_C,
      ST_DDL,
      ST_DHLX,
   };


   SourceStream(std::string const &filename, unsigned type);
   SourceStream(std::istream &in, std::string const &filename, unsigned type);
   ~SourceStream();

   char get();
   void unget(char c);
   std::string getbrace();

   long getBraceDepth() const {return depthBrace;}
   long getColumn() const {return countColumn;}
   long getCommentDepth() const {return depthComment;}
   std::string const &getFilename() const {return filename;}
   long getLineCount() const {return countLine;}

   bool isInComment() const {return inComment || depthComment;}
   bool isInQuote() const {return inQuoteDouble || inQuoteSingle;}

   // istream semantics
   bool operator !     () const {return !*in;}
        operator void* () const {return (void *)*in;}

private:
   void initialize(unsigned type);

   int oldC, curC, newC;
   std::istream *in;
   std::string filename;
   std::stack<char> ungetStack;

   long countColumn;
   long countLine;

   long depthBrace;   // { }
   long depthComment; // /* */

   bool delIn : 1;

   bool doCommentASM : 1; // ;
   bool doCommentC   : 1; // /* */
   bool doCommentCPP : 1; // //

   bool doQuoteDouble : 1; // "
   bool doQuoteSingle : 1; // '

   bool doStrip           : 1;
   bool doStripAuto       : 1;
   bool doStripComment    : 1;
   bool doStripQuote      : 1;
   bool doStripWhitespace : 1;

   bool inComment     : 1; // //
   bool inQuoteDouble : 1; // "
   bool inQuoteSingle : 1; // '
   bool inWhitespace  : 1;
};

#endif//HPP_SourceStream_

