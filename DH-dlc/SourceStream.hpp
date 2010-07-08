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
		enum SourceType
		{
			ST_NORMAL,
			ST_DHLX,
		};

		SourceStream(std::istream & in, SourceType type = ST_NORMAL);

		int get();
		void unget(int c);
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
		int _lastData, _thisData, _nextData;
		int _ungetData;
		std::istream * _in;

		int _countLine;

		int _depthBrace;   // { }
		int _depthComment; // /* */

		unsigned _doStrip           : 1;
		unsigned _doStripAuto       : 1;
		unsigned _doStripComment    : 1;
		unsigned _doStripQuote      : 1;
		unsigned _doStripWhitespace : 1;

		unsigned _doCompressWhitespace : 1;

		unsigned _inComment        : 1; // //
		unsigned _inQuote          : 1; // "
		unsigned _inQuote2         : 1; // '
		unsigned _inWhitespace     : 1;
		unsigned _inWhitespaceLast : 1;
};



inline int SourceStream::getBraceDepth() const
{
	return _depthBrace;
}

inline int SourceStream::getCommentDepth() const
{
	return _depthComment;
}

inline int SourceStream::getLineCount() const
{
	return _countLine;
}

inline bool SourceStream::isInComment() const
{
	return _inComment || _depthComment != 0;
}

inline bool SourceStream::isInQuote() const
{
	return _inQuote || _inQuote2;
}

inline bool SourceStream::operator ! () const
{
	return !(*_in);
}

inline SourceStream::operator void* () const
{
	return (void*) *_in;
}



#endif /* SOURCESTREAM_H */



