/*
    Copyright 2009, 2010 David Hill

    This file is part of DH-dlc.

    DH-dlc is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DH-dlc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DH-dlc.  If not, see <http://www.gnu.org/licenses/>.
*/

/*

*/

#ifndef SOURCESTREAM_H
#define SOURCESTREAM_H

#include <istream>

#include "types.hpp"



class SourceStream
{
	public:
		SourceStream(std::istream&);

		int get();
		std::string getbrace();

		int getBraceDepth() const;
		int getCommentDepth() const;
		int getLineCount() const;

		bool isInComment() const;
		bool isInQuote() const;

		// istream semantics
		bool operator !     () const;
		     operator void* () const;

	private:
		int lastData, thisData, nextData;
		std::istream* in;

		int countLine;

		int depthBrace;   // { }
		int depthComment; // /* */

		bool doStrip;
		bool doStripAuto;

		bool inComment; // //
		bool inQuote;   // "
		bool inQuote2;  // '
};



inline int SourceStream::getBraceDepth() const
{
	return depthBrace;
}

inline int SourceStream::getCommentDepth() const
{
	return depthComment;
}

inline int SourceStream::getLineCount() const
{
	return countLine;
}

inline bool SourceStream::isInComment() const
{
	return inComment || depthComment != 0;
}

inline bool SourceStream::isInQuote() const
{
	return inQuote || inQuote2;
}

inline bool SourceStream::operator ! () const
{
	return !(*in);
}

inline SourceStream::operator void* () const
{
	return (void*) *in;
}



#endif /* SOURCESTREAM_H */



