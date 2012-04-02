//-----------------------------------------------------------------------------
//
// Copyright(C) 2011, 2012 David Hill
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
// Source file position handling.
//
//-----------------------------------------------------------------------------

#ifndef HPP_SourcePosition_
#define HPP_SourcePosition_

#include <string>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// SourcePosition
//
class SourcePosition
{
public:
   //
   // ::SourcePosition
   //
   SourcePosition()
   {
   }

   //
   // ::SourcePosition
   //
   SourcePosition(std::string const &_file, long _line, long _column)
    : file(_file), line(_line), column(_column)
   {
   }

   std::string file;
   long line;
   long column;


   //
   // ::builtin
   //
   static SourcePosition const &builtin()
   {
      static SourcePosition const position("__builtin__", 0, 0);
      return position;
   }
};

#endif /* HPP_SourcePosition_ */

