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

#include "SourceStream.hpp"

#include <istream>

#include "options.hpp"
#include "types.hpp"
#include "exceptions/SyntaxException.hpp"

SourceStream::SourceStream(std::istream& is) :
	lastData(-2), thisData(-2), nextData(-2),
	in(&is),

	countLine(1),

	depthBrace(0),
	depthComment(0),

	doStrip(true),
	doStripAuto(true),

	inComment(false),
	inQuote(false),
	inQuote2(false)
{}

int SourceStream::get()
{
	while (true)
	{
		thisData = nextData != -2 ? nextData : in->get();
		nextData = in->get();

		// \n end of line
		if (thisData == '\n')
		{
			inComment = false;
			++countLine;
		}

		// { brace open
		if (thisData == '{' && !isInComment() && !isInQuote())
			++depthBrace;

		// } brace close
		if (thisData == '}' && !isInComment() && !isInQuote())
			--depthBrace;

		// " quote open/close
		if (thisData == '"' && !isInComment() && !inQuote2)
		{
			if (inQuote && lastData != '\\')
				inQuote = false;
			else
				inQuote = true;
		}

		// ' quote open/close
		if (thisData == '\'' && !isInComment() && !inQuote)
		{
			if (inQuote2 && lastData != '\\')
				inQuote2 = false;
			else
				inQuote2 = true;
		}

		// /? comment start
		if (thisData == '/' && lastData != '*' && !isInQuote())
		{
			if (nextData == '/' && depthComment == 0)
				inComment = true;

			else if (nextData == '*' && !inComment)
				++depthComment;
		}

		// */ multi-line comment end
		if (thisData == '*' && nextData == '/' && lastData != '/' && !isInQuote() && !inComment)
			--depthComment;

		int lastData = this->lastData;
		this->lastData = thisData;

		if (!doStrip)
			return thisData;

		// StripAuto stops stripping inside braces.
		if (doStripAuto && depthBrace != 0)
			return thisData;

		// Comments are stripped.
		if (isInComment())
			continue;

		// End of multi-line comments are stripped, unless quoted.
		if (((thisData == '*' && nextData == '/') || (lastData == '*' && thisData == '/')) && !isInQuote())
			continue;

		// Unquoted whitespace is stripped.
		if (!isInQuote() && isspace(thisData))
			continue;

		// Quotes are stripped (if escaped, they won't reach here)
		if (thisData == '"' || thisData == '\'')
			continue;

		// Escape sequence (only if quoted)
		if (isInQuote() && thisData == '\\')
		{
			int nextData = this->nextData;
			this->lastData = -2;
			this->nextData = -2;

			switch (nextData)
			{
				case 'r': return '\r';
				case 'n': return '\n';
				case 't': return '\t';
				case '\n': ++countLine;
				case '\\':
				case '\'':
				case '"': return nextData;

				default:
					throw SyntaxException(std::string("unknown escape sequence:\\") + (char)nextData);
			}
		}

		// For compiling TEXTMAP lumps and for people who like it.
		if (!option_case_sensitive && !isInQuote())
			return option_case_upper ? toupper(thisData) : tolower(thisData);

		return thisData;
	}
}

std::string SourceStream::getbrace()
{
	std::string newString;

	if (depthBrace == 0)
		return newString;

	int targetDepthBrace = depthBrace-1;

	while (true)
	{
		int nextChar = get();

		if (nextChar == -1) throw SyntaxException("unexpected end of data");

		if (depthBrace == targetDepthBrace)
			break;

		newString += (char) nextChar;
	}

	return newString;
}



