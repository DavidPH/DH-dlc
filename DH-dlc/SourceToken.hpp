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
	06/02/2010 - SourceStream extraction function no longer inline because
		of new --debug-token option handling.
*/

#ifndef SOURCETOKEN_H
#define SOURCETOKEN_H

#include <ostream>
#include <vector>

#include "SourceStream.hpp"



class SourceTokenDDL
{
	public:
		typedef std::string TokenType;

		explicit SourceTokenDDL();
		explicit SourceTokenDDL(SourceStream & in);

		void clear();

		bool empty() const;

		std::string const & getName() const;
		std::string const & getType() const;

		std::string const & getValue() const;
		std::string const & getData() const;

		std::vector<std::string> const & getBase() const;
		std::string const & getBase(size_t, std::string const & = "") const;

	private:
		std::string type, name, value, data;
		std::vector<std::string> base;
};

class SourceTokenDHLX
{
	public:
		enum TokenType
		{
			TT_NONE, TT_EOF,

			TT_IDENTIFIER,
			TT_NUMBER,
			TT_STRING,

			TT_OP_AND,                // &
			TT_OP_AND_EQUALS,         // &=
			TT_OP_AND2,               // &&
			TT_OP_AND2_EQUALS,        // &&=
			TT_OP_ASTERIX,            // *
			TT_OP_ASTERIX_EQUALS,     // *=
			TT_OP_BRACE_C,            // }
			TT_OP_BRACE_O,            // {
			TT_OP_BRACKET_C,          // ]
			TT_OP_BRACKET_O,          // [
			TT_OP_CMP_EQ,             // ==
			TT_OP_CMP_GE,             // >=
			TT_OP_CMP_GT,             // >
			TT_OP_CMP_LE,             // <=
			TT_OP_CMP_LT,             // <
			TT_OP_CMP_NE,             // !=
			TT_OP_COLON,              // :
			TT_OP_COMMA,              // ,
			TT_OP_EQUALS,             // =
			TT_OP_HASH,               // #
			TT_OP_MINUS,              // -
			TT_OP_MINUS_EQUALS,       // -=
			TT_OP_MINUS2,             // --
			TT_OP_NOT,                // !
			TT_OP_PERIOD,             // .
			TT_OP_PIPE,               // |
			TT_OP_PIPE_EQUALS,        // |=
			TT_OP_PIPE2,              // ||
			TT_OP_PIPE2_EQUALS,       // ||=
			TT_OP_PARENTHESIS_C,      // )
			TT_OP_PARENTHESIS_O,      // (
			TT_OP_PLUS,               // +
			TT_OP_PLUS_EQUALS,        // +=
			TT_OP_PLUS2,              // ++
			TT_OP_SEMICOLON,          // ;
			TT_OP_SHIFT_LEFT,         // <<
			TT_OP_SHIFT_LEFT_EQUALS,  // <<=
			TT_OP_SHIFT_RIGHT,        // >>
			TT_OP_SHIFT_RIGHT_EQUALS, // >>=
			TT_OP_SLASH,              // /
			TT_OP_SLASH_EQUALS,       // /=
		};

		explicit SourceTokenDHLX();
		explicit SourceTokenDHLX(SourceStream & in);

		std::string const & getData() const;
		TokenType getType() const;

	private:
		std::string _data;
		TokenType   _type;
};



std::ostream & operator << (std::ostream & out, SourceTokenDDL     const & in);
std::ostream & operator << (std::ostream & out, SourceTokenDHLX    const & in);
std::ostream & operator << (std::ostream & out, SourceTokenDHLX::TokenType in);

SourceStream & operator >> (SourceStream & in, SourceTokenDDL  & out);
SourceStream & operator >> (SourceStream & in, SourceTokenDHLX & out);



inline void SourceTokenDDL::clear()
{
	type.clear();
	name.clear();
	value.clear();
	data.clear();
	base.clear();
}



inline bool SourceTokenDDL::empty() const
{
	return type.empty() && name.empty() && value.empty() && data.empty() && base.empty();
}



inline std::string const & SourceTokenDDL::getName() const
{
	return name;
}
inline std::string const & SourceTokenDDL::getType() const
{
	return type;
}
inline std::string const & SourceTokenDDL::getValue() const
{
	return value;
}
inline std::string const & SourceTokenDDL::getData() const
{
	return data;
}

inline std::vector<std::string> const & SourceTokenDDL::getBase() const
{
	return base;
}
inline std::string const & SourceTokenDDL::getBase(size_t index, std::string const & def) const
{
	if (index < base.size())
		return base[index];
	else
		return def;
}



#endif



