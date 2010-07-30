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

SourceStream::SourceStream(std::istream & in, SourceType type) :
	_lastData(-2), _thisData(-2), _nextData(-2),
	_ungetStack(),
	_in(&in),

	_countLine(1),

	_depthBrace(0),
	_depthComment(0),

	_doStrip(true),
	_doStripAuto(true),
	_doStripComment(true),
	_doStripQuote(true),
	_doStripWhitespace(true),

	_doCompressWhitespace(false),

	_inComment(false),
	_inQuote(false),
	_inQuote2(false),
	_inWhitespace(false),
	_inWhitespaceLast(false)
{
	switch (type)
	{
	case ST_NORMAL:
		break;

	case ST_DHLX:
		_doStripAuto       = false;
		_doStripQuote      = false;
		_doStripWhitespace = false;

		_doCompressWhitespace = true;
		break;
	}
}

int SourceStream::get()
{
	if (!_ungetStack.empty())
	{
		int c = _ungetStack.top();
		_ungetStack.pop();
		return c;
	}

	while (true)
	{
		_thisData = _nextData != -2 ? _nextData : _in->get();
		_nextData = _in->get();

		// \n end of line
		if (_thisData == '\n')
		{
			_inComment = false;
			++_countLine;
		}

		// { brace open
		if (_thisData == '{' && !isInComment() && !isInQuote())
			++_depthBrace;

		// } brace close
		if (_thisData == '}' && !isInComment() && !isInQuote())
			--_depthBrace;

		// " quote open/close
		if (_thisData == '"' && !isInComment() && !_inQuote2)
		{
			if (_inQuote && _lastData != '\\')
				_inQuote = false;
			else
				_inQuote = true;
		}

		// ' quote open/close
		if (_thisData == '\'' && !isInComment() && !_inQuote)
		{
			if (_inQuote2 && _lastData != '\\')
				_inQuote2 = false;
			else
				_inQuote2 = true;
		}

		// /? comment start
		if (_thisData == '/' && _lastData != '*' && !isInQuote())
		{
			if (_nextData == '/' && _depthComment == 0)
				_inComment = true;

			else if (_nextData == '*' && !_inComment)
				++_depthComment;
		}

		// */ multi-line comment end
		if (_thisData == '*' && _nextData == '/' && _lastData != '/' && !isInQuote() && !_inComment)
			--_depthComment;

		// whitespace
		_inWhitespaceLast = _inWhitespace;
		_inWhitespace     = isspace(_thisData) != 0;

		int lastData = _lastData;
		   _lastData = _thisData;

		if (!_doStrip)
			return _thisData;

		// StripAuto stops stripping inside braces.
		if (_doStripAuto && _depthBrace != 0)
			return _thisData;

		// Comments are stripped.
		if (_doStripComment && isInComment())
			continue;

		// End of multi-line comments are stripped, unless quoted.
		if (_doStripComment && ((_thisData == '*' && _nextData == '/') || (lastData == '*' && _thisData == '/')) && !isInQuote())
			continue;

		// Unquoted whitespace is stripped.
		if (_doStripWhitespace && _inWhitespace && !isInQuote())
			continue;

		if (_doCompressWhitespace && _inWhitespace)
		{
			// Compressing only returns the first whitespace...
			if (_inWhitespaceLast)
				continue;
			// ...as a space.
			return ' ';
		}

		// Quotes are stripped (if escaped, they won't reach here)
		if (_doStripQuote && (_thisData == '"' || _thisData == '\''))
			continue;

		// Escape sequence (only if quoted)
		if (isInQuote() && _thisData == '\\')
		{
			int nextData = _nextData;
			_lastData = -2;
			_nextData = -2;

			switch (nextData)
			{
			case 'r': return '\r';
			case 'n': return '\n';
			case 't': return '\t';
			case '\n': ++_countLine;
			case '\t':
			case '\\':
			case '\'':
			case '"':
			case ' ':
				return nextData;

			case 'x':
			case 'X':
			{
				int digit1 = _in->get();
				int digit2 = _in->get();

				switch (digit1)
				{
				case '0': nextData = 0x00; break;
				case '1': nextData = 0x10; break;
				case '2': nextData = 0x20; break;
				case '3': nextData = 0x30; break;
				case '4': nextData = 0x40; break;
				case '5': nextData = 0x50; break;
				case '6': nextData = 0x60; break;
				case '7': nextData = 0x70; break;
				case '8': nextData = 0x80; break;
				case '9': nextData = 0x90; break;
				case 'A': nextData = 0xA0; break;
				case 'B': nextData = 0xB0; break;
				case 'C': nextData = 0xC0; break;
				case 'D': nextData = 0xD0; break;
				case 'E': nextData = 0xE0; break;
				case 'F': nextData = 0xF0; break;
				case 'a': nextData = 0xa0; break;
				case 'b': nextData = 0xb0; break;
				case 'c': nextData = 0xc0; break;
				case 'd': nextData = 0xd0; break;
				case 'e': nextData = 0xe0; break;
				case 'f': nextData = 0xf0; break;

				default:
					throw SyntaxException(((std::string("invalid escape sequence:\\") + (char)nextData) + (char)digit1) + (char)digit2);
				}

				switch (digit2)
				{
				case '0': nextData += 0x00; break;
				case '1': nextData += 0x01; break;
				case '2': nextData += 0x02; break;
				case '3': nextData += 0x03; break;
				case '4': nextData += 0x04; break;
				case '5': nextData += 0x05; break;
				case '6': nextData += 0x06; break;
				case '7': nextData += 0x07; break;
				case '8': nextData += 0x08; break;
				case '9': nextData += 0x09; break;
				case 'A': nextData += 0x0A; break;
				case 'B': nextData += 0x0B; break;
				case 'C': nextData += 0x0C; break;
				case 'D': nextData += 0x0D; break;
				case 'E': nextData += 0x0E; break;
				case 'F': nextData += 0x0F; break;
				case 'a': nextData += 0x0a; break;
				case 'b': nextData += 0x0b; break;
				case 'c': nextData += 0x0c; break;
				case 'd': nextData += 0x0d; break;
				case 'e': nextData += 0x0e; break;
				case 'f': nextData += 0x0f; break;

				default:
					throw SyntaxException(((std::string("invalid escape sequence:\\") + (char)nextData) + (char)digit1) + (char)digit2);
				}

				return nextData;
			}

			default:
				throw SyntaxException(std::string("unknown escape sequence:\\") + (char)nextData);
			}
		}

		// For compiling TEXTMAP lumps and for people who like it.
		if (!option_case_sensitive && !isInQuote())
			return option_case_upper ? toupper(_thisData) : tolower(_thisData);

		return _thisData;
	}
}

void SourceStream::unget(int c)
{
	_ungetStack.push(c);
}

std::string SourceStream::getbrace()
{
	std::string newString;

	if (_depthBrace == 0)
		return newString;

	int targetDepthBrace = _depthBrace-1;

	// Need to stop stripping in braces for this function.
	bool doStripAuto = _doStripAuto;
	_doStripAuto = true;

	while (true)
	{
		int nextChar = get();

		if (nextChar == -1) throw SyntaxException("unexpected end of data");

		if (_depthBrace == targetDepthBrace)
			break;

		newString += (char) nextChar;
	}

	// Restore configuration.
	_doStripAuto = doStripAuto;

	return newString;
}



