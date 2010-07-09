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
	2010/02/06 - SourceStream extraction function no longer inline because
		of new --debug-token option handling.
	2010/02/28 - Update for lo_type enum.
*/

#include "SourceToken.hpp"

#include <iostream>

#include "lo_types.hpp"
#include "math.hpp"
#include "options.hpp"
#include "SourceStream.hpp"
#include "exceptions/SyntaxException.hpp"



SourceToken::SourceToken() : type(), name(), value(), data(), base() {}

SourceToken::SourceToken(SourceStream& in) : type(), name(), value(), data(), base()
{
	int nextChar = in.get();
	if (nextChar == -1)
	{
		if (in.getBraceDepth() != 0)
			throw SyntaxException("unbalanced braces");

		return;
	}

	int termChar = -2;

	bool setType = false;

	// [type]  if present
	if (nextChar == '[')
	{
		while (true)
		{
			nextChar = in.get();

			if (nextChar == -1) throw SyntaxException("unexpected end of data");

			if (nextChar == ']' && !in.isInQuote())
				break;

			type += nextChar;
		}

		setType = true;

		type = get_lo_type_redirect(type);

		nextChar = -2;
	}

	if (nextChar == -2)
		nextChar = in.get();

	// name
	while (true)
	{
		switch (nextChar)
		{
			case -1: throw SyntaxException("unexpected end of data");

			case ':':
			case ';':
			case '=':
			case '{':
				if (!in.isInQuote())
				{
					termChar = nextChar;
					break;
				}
			default:
				name += nextChar;
				break;
		}

		if (termChar != -2)
			break;

		nextChar = in.get();
	}

	// base
	if (termChar == ':')
	{
		termChar = -2;

		std::string nextBase;

		while (true)
		{
			nextChar = in.get();

			switch (nextChar)
			{
				case -1: throw SyntaxException("unexpected end of data");

				case ':':
				case ';':
				case '=':
				case '{':
					if (!in.isInQuote())
					{
						termChar = nextChar;
						break;
					}
				default:
					nextBase += nextChar;
					break;
			}

			if (termChar == ':')
			{
				// Cannot parse_name the base, because it might be a command!
				// #script:"FILE":"int arg[1]"; would have unexpected results.
				base.push_back(nextBase);
				nextBase.clear();
				termChar = -2;
			}

			if (termChar != -2)
				break;
		}

		base.push_back(nextBase);
	}

	// type {...} || type;
	if ((termChar == '{' || termChar == ';') && !setType)
	{
		type = get_lo_type_redirect(name);

		if (get_lo_type(type))
		{
			name.clear();
			setType = true;
		}
		else
		{
			type.clear();
		}
	}

	if (termChar == ';')
		return;

	// = value
	if (termChar == '=')
	{
		termChar = -2;

		nextChar = in.get();

		// = [type] value
		if (nextChar == '[' && !setType)
		{
			while (true)
			{
				nextChar = in.get();

				if (nextChar == -1) throw SyntaxException("unexpected end of data");

				if (nextChar == ']' && !in.isInQuote())
					break;

				type += nextChar;
			}

			type = get_lo_type_redirect(type);

			nextChar = -2;

			if (!type.empty() && !get_lo_type(type))
			{
				value = "[" + type + "]";
				type.clear();
			}
			else
				setType = true;
		}
		// = (type) value
		else if (nextChar == '(' && !setType)
		{
			while (true)
			{
				nextChar = in.get();

				if (nextChar == -1) throw SyntaxException("unexpected end of data");

				if (nextChar == ')')
					break;

				type += nextChar;
			}

			type = get_lo_type_redirect(type);

			nextChar = -2;

			if (!get_lo_type(type))
			{
				value = "(" + type + ")";
				type.clear();
			}
			else
				setType = true;
		}

		if (nextChar == -2)
			nextChar = in.get();

		while (true)
		{
			switch (nextChar)
			{
				case -1: throw SyntaxException("unexpected end of data");

				case ';':
				case '{':
					if (!in.isInQuote())
					{
						termChar = nextChar;
						break;
					}
				default:
					value += nextChar;
					break;
			}

			if (termChar != -2)
				break;

			nextChar = in.get();
		}
	}

	// op=
	if (!name.empty() && !value.empty())
	{
		char opChar = *name.rbegin();

		if (isoperator(opChar))
		{
			name = name.substr(0, name.size()-1);
			value = name + opChar + "(" + value + ")";
		}
	}

	// = type {...}
	if (!setType)
	{
		type = get_lo_type_redirect(value);

		if (get_lo_type(type))
		{
			value.clear();
			setType = true;
		}
		else
		{
			type.clear();
		}
	}

	if (termChar == ';')
		return;

	// data
	if (termChar == '{')
		data = in.getbrace();
}



SourceTokenDHLX::SourceTokenDHLX() : _data(), _type(TT_NONE) {}
SourceTokenDHLX::SourceTokenDHLX(SourceStream & in) : _data(), _type(TT_NONE)
{
	int nextChar = in.get();

	// Discard any whitespace before token.
	if (nextChar == ' ') nextChar = in.get();

	switch (nextChar)
	{
	case -1: _type = TT_EOF; return;

	case '}': _type = TT_OP_BRACE_C;       return;
	case '{': _type = TT_OP_BRACE_O;       return;
	case ']': _type = TT_OP_BRACKET_C;     return;
	case '[': _type = TT_OP_BRACKET_O;     return;
	case ',': _type = TT_OP_COMMA;         return;
	case '#': _type = TT_OP_HASH;          return;
	case ')': _type = TT_OP_PARENTHESIS_C; return;
	case '(': _type = TT_OP_PARENTHESIS_O; return;

	case '&':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_AND_EQUALS; return;
		if (nextChar == '&')
		{
			nextChar = in.get();

			if (nextChar == '=') _type = TT_OP_AND2_EQUALS; return;

			in.unget(nextChar);

			_type = TT_OP_AND2; return;
		}

		in.unget(nextChar);

		_type = TT_OP_AND; return;

	case '*':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_ASTERIX_EQUALS; return;

		in.unget(nextChar);

		_type = TT_OP_ASTERIX; return;

	case '=':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_CMP_EQ; return;

		in.unget(nextChar);

		_type = TT_OP_EQUALS; return;

	case '>':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_CMP_GE; return;
		if (nextChar == '>')
		{
			nextChar = in.get();

			if (nextChar == '=') _type = TT_OP_SHIFT_RIGHT_EQUALS; return;

			in.unget(nextChar);

			_type = TT_OP_SHIFT_RIGHT; return;
		}

		in.unget(nextChar);

		_type = TT_OP_CMP_GT; return;

	case '<':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_CMP_LE; return;
		if (nextChar == '<')
		{
			nextChar = in.get();

			if (nextChar == '=') _type = TT_OP_SHIFT_LEFT_EQUALS; return;

			in.unget(nextChar);

			_type = TT_OP_SHIFT_LEFT; return;
		}

		in.unget(nextChar);

		_type = TT_OP_CMP_LT; return;

	case '-':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_MINUS_EQUALS; return;
		if (nextChar == '-') _type = TT_OP_MINUS2;       return;

		in.unget(nextChar);

		_type = TT_OP_MINUS; return;

	case '!':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_CMP_NE; return;

		in.unget(nextChar);

		_type = TT_OP_NOT; return;

	case '|':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_PIPE_EQUALS; return;
		if (nextChar == '|')
		{
			nextChar = in.get();

			if (nextChar == '=') _type = TT_OP_PIPE2_EQUALS; return;

			in.unget(nextChar);

			_type = TT_OP_PIPE2; return;
		}

		in.unget(nextChar);

		_type = TT_OP_PIPE; return;

	case '+':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_PLUS_EQUALS; return;
		if (nextChar == '+') _type = TT_OP_PLUS2;       return;

		in.unget(nextChar);

		_type = TT_OP_PLUS; return;

	case '/':
		nextChar = in.get();

		if (nextChar == '=') _type = TT_OP_SLASH_EQUALS; return;

		in.unget(nextChar);

		_type = TT_OP_SLASH; return;
	}

	if (isalpha(nextChar) || nextChar == '_')
	{
		_type = TT_IDENTIFIER;

		while (isalnum(nextChar) || nextChar == '_')
		{
			_data += (char)nextChar;

			nextChar = in.get();
		}

		in.unget(nextChar);

		return;
	}

	if (isdigit(nextChar))
	{
		_type = TT_NUMBER;

		bool foundDot   (false);
		bool foundPlus  (false);
		bool foundZero  (nextChar == '0');
		bool foundPrefix(!foundZero);

		while (true)
		{
			_data += (char)nextChar;

			nextChar = in.get();

			if (!foundPrefix)
			{
				// Can only find prefix at start.
				foundPrefix = true;

				if (nextChar == 'D' || nextChar == 'd') continue;
				if (nextChar == 'O' || nextChar == 'o') continue;
				if (nextChar == 'X' || nextChar == 'x') continue;
			}

			if (isxdigit(nextChar)) continue;

			if (!foundDot && nextChar == '.')
			{
				foundDot = true;
				continue;
			}

			if (!foundPlus && (nextChar == '+' || nextChar == '-'))
			{
				foundPlus = true;
				continue;
			}

			break;
		}

		in.unget(nextChar);

		return;
	}

	if (nextChar == '"')
	{
		_type = TT_STRING;

		while (in.isInQuote())
		{
			_data += (char)nextChar;

			nextChar = in.get();
		}

		return;
	}
}



std::ostream& operator << (std::ostream& out, const SourceToken& in)
{
	out << "[" << in.getType() << "] " << in.getName();

	for (size_t index = 0; index < in.getBase().size(); ++index)
		out << " : " << in.getBase()[index];

	out << " = " << in.getValue() << " {" << in.getData() << "}";

	return out;
}

SourceStream& operator >> (SourceStream& in, SourceToken& out)
{
	out = SourceToken(in);

	if (option_debug_token)
		std::cerr << out << "\n";

	return in;
}



