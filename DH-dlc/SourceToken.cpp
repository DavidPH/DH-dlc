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

#include "LevelObjectType.hpp"
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
		if (type_t::has_type(name))
		{
			type = name;
			name.clear();
			setType = true;
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

			nextChar = -2;

			if (type_t::has_type(type))
			{
				setType = true;
			}
			else
			{
				value = "[" + type + "]";
				type.clear();
			}
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

			nextChar = -2;

			if (type_t::has_type(type))
			{
				setType = true;
			}
			else
			{
				value = "(" + type + ")";
				type.clear();
			}
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
		if (type_t::has_type(value))
		{
			type = value;
			value.clear();
			setType = true;
		}
	}

	if (termChar == ';')
		return;

	// data
	if (termChar == '{')
		data = in.getbrace();
}



std::ostream& operator << (std::ostream& out, const SourceToken& in)
{
	out << "[" << in.type << "] " << in.name;

	for (size_t index = 0; index < in.base.size(); ++index)
		out << " : " << in.base[index];

	out << " = " << in.value << " {" << in.data << "}";

	return out;
}

SourceStream& operator >> (SourceStream& in, SourceToken& out)
{
	out = SourceToken(in);

	if (option_debug_token)
		std::cerr << out << "\n";

	return in;
}



