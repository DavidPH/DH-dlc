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
	2010/02/28 - Update for lo_type enum.
*/

#include "process_token.hpp"

#include "compound_objects.hpp"
#include "global_object.hpp"
#include "options.hpp"
#include "process_file.hpp"
#include "SourceToken.hpp"

#include "exceptions/CompilerException.hpp"
#include "exceptions/SyntaxException.hpp"

#include "parsing/FunctionHandlerDDL.hpp"
#include "parsing/FunctionHandlerDHLX.hpp"
#include "parsing/parsing.hpp"

#include "../common/foreach.hpp"



void process_token(SourceTokenDDL const & st, SourceScannerDDL & sc)
{
	if (st.empty())
		return;

	// commands start with #
	if (st.getName().size() > 0 && st.getName()[0] == '#')
	{
		// # default type : NAME : TYPE : [CONTEXT]
		if (st.getName() == command_name_defaulttype())
		{
			name_t type_name(parse_name(st.getBase(0)));
			type_t type = type_t::get_type(st.getBase(1));
			type_t context;

			if (!st.getBase(2).empty())
				context = type_t::get_type(st.getBase(2));

			type_t::add_default_type(type_name, context, type);

			return;
		}

		// # define : TYPE { data }
		if (st.getName() == command_name_define())
		{
			add_compound_object(st.getBase(0), st.getData());

			return;
		}

		// [return type] # function : name [: return type ...] [:: argument type ...] { data }
		if (st.getName() == command_name_function())
		{
			std::vector<type_t> returnTypes;

			std::string functionName(st.getBase(0));

			std::vector<std::string> const & base(st.getBase());
			size_t index(1);

			returnTypes.push_back(type_t::get_type(st.getType()));
			while (index < base.size())
			{
				if (base[index].empty())
				{
					++index;
					break;
				}

				returnTypes.push_back(type_t::get_type(base[index]));

				++index;
			}

			std::vector<type_t> argTypes;

			while (index < base.size())
			{
				if (base[index].empty())
				{
					++index;
					break;
				}

				argTypes.push_back(type_t::get_type(base[index]));

				++index;
			}

			FOREACH_T(std::vector<type_t>, it, returnTypes)
			{
				switch (it->getNativeType())
				{
				case type_t::NT_NONE:
					break;


				case type_t::NT_BOOL_T:
					FunctionHandler<bool_t>::add_function(functionName, new FunctionHandlerDDL<bool_t>(st.getData(), argTypes));
					break;


				case type_t::NT_INT_S_T:
					FunctionHandler<int_s_t>::add_function(functionName, new FunctionHandlerDDL<int_s_t>(st.getData(), argTypes));
					break;

				case type_t::NT_INT_T:
					FunctionHandler<int_t>::add_function(functionName, new FunctionHandlerDDL<int_t>(st.getData(), argTypes));
					break;

				case type_t::NT_INT_L_T:
					FunctionHandler<int_l_t>::add_function(functionName, new FunctionHandlerDDL<int_l_t>(st.getData(), argTypes));
					break;


				case type_t::NT_REAL_S_T:
					FunctionHandler<real_s_t>::add_function(functionName, new FunctionHandlerDDL<real_s_t>(st.getData(), argTypes));
					break;

				case type_t::NT_REAL_T:
					FunctionHandler<real_t>::add_function(functionName, new FunctionHandlerDDL<real_t>(st.getData(), argTypes));
					break;

				case type_t::NT_REAL_L_T:
					FunctionHandler<real_l_t>::add_function(functionName, new FunctionHandlerDDL<real_l_t>(st.getData(), argTypes));
					break;


				case type_t::NT_STRING_T:
					FunctionHandler<string_t>::add_function(functionName, new FunctionHandlerDDL<string_t>(st.getData(), argTypes));
					break;

				case type_t::NT_STRING8_T:
					FunctionHandler<string8_t>::add_function(functionName, new FunctionHandlerDDL<string8_t>(st.getData(), argTypes));
					break;

				case type_t::NT_STRING16_T:
					FunctionHandler<string16_t>::add_function(functionName, new FunctionHandlerDDL<string16_t>(st.getData(), argTypes));
					break;

				case type_t::NT_STRING32_T:
					FunctionHandler<string32_t>::add_function(functionName, new FunctionHandlerDDL<string32_t>(st.getData(), argTypes));
					break;

				case type_t::NT_STRING80_T:
					FunctionHandler<string80_t>::add_function(functionName, new FunctionHandlerDDL<string80_t>(st.getData(), argTypes));
					break;

				case type_t::NT_STRING320_T:
					FunctionHandler<string320_t>::add_function(functionName, new FunctionHandlerDDL<string320_t>(st.getData(), argTypes));
					break;


				case type_t::NT_TYPE_T:
					break;


				case type_t::NT_UBYTE_T:
					FunctionHandler<ubyte_t>::add_function(functionName, new FunctionHandlerDDL<ubyte_t>(st.getData(), argTypes));
					break;

				case type_t::NT_SWORD_T:
					FunctionHandler<sword_t>::add_function(functionName, new FunctionHandlerDDL<sword_t>(st.getData(), argTypes));
					break;

				case type_t::NT_UWORD_T:
					FunctionHandler<uword_t>::add_function(functionName, new FunctionHandlerDDL<uword_t>(st.getData(), argTypes));
					break;

				case type_t::NT_SDWORD_T:
					FunctionHandler<sdword_t>::add_function(functionName, new FunctionHandlerDDL<sdword_t>(st.getData(), argTypes));
					break;

				case type_t::NT_UDWORD_T:
					FunctionHandler<udword_t>::add_function(functionName, new FunctionHandlerDDL<udword_t>(st.getData(), argTypes));
					break;
				}
			}

			return;
		}

		// # include : FILENAME
		if (st.getName() == command_name_include())
		{
			process_file(st.getBase(0));

			return;
		}

		// # precision : NEW_PRECISION
		if (st.getName() == command_name_precision())
		{
			set_precision(parse<int_s_t>(st.getBase(0)));

			return;
		}

		// # typedef : OLD_TYPE : NEW_TYPE
		if (st.getName() == command_name_typedef())
		{
			type_t::add_redirect_type(st.getBase(1), type_t::get_type(st.getBase(0)));

			return;
		}

		// # typedef new : TYPE : MODE
		if (st.getName() == command_name_typedefnew())
		{
			if (st.getBase(1) == "dynamic")
				type_t::add_type(st.getBase(0), type_t::MODE_DYNAMIC);

			else if (st.getBase(1) == "value")
				type_t::add_type(st.getBase(0), type_t::MODE_VALUE);

			else if (st.getBase(1) == "object")
				type_t::add_type(st.getBase(0), type_t::MODE_OBJECT);

			else if (st.getBase(1) == "compoundobject")
				type_t::add_type(st.getBase(0), type_t::MODE_COMPOUNDOBJECT);

			else if (st.getBase(1) == "inline")
				type_t::add_type(st.getBase(0), type_t::MODE_INLINE);

			else
				throw CompilerException("unknown typedefnew:" + st.getBase(1));

			return;
		}

		global_object->doCommand(st.getName(), st);

		return;
	}

	global_object->addObject(parse_name(st.getName()), st);
}

void process_token(SourceTokenDHLX const & st, SourceScannerDHLX & sc)
{
	if ((st.getType() == SourceTokenDHLX::TT_EOF) || (st.getType() == SourceTokenDHLX::TT_NONE))
		return;

	if (st.getType() == SourceTokenDHLX::TT_OP_HASH)
	{
		SourceTokenDHLX commandToken(sc.get(SourceTokenDHLX::TT_IDENTIFIER));

		std::string commandString('#' + commandToken.getData());

		// # defaulttype name_t IDENTIFIER [IDENTIFIER]
		if (commandString == command_name_defaulttype())
		{
			name_t name(parse_name(sc));
			type_t type = type_t::get_type(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());
			type_t context;

			SourceTokenDHLX context_token(sc.get());

			if (context_token.getType() == SourceTokenDHLX::TT_IDENTIFIER)
				context = type_t::get_type(context_token.getData());
			else
				sc.unget(context_token);

			sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

			type_t::add_default_type(name, context, type);
		}
		// # define IDENTIFIER block
		else if (commandString == command_name_define())
		{
			std::string type(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());

			add_compound_object(type, sc);
		}
		// # function ... block
		else if (commandString == command_name_function())
		{
			std::vector<type_t> returnTypes;

			while (true)
			{
				SourceTokenDHLX typeToken(sc.get());

				if (typeToken.getType() != SourceTokenDHLX::TT_IDENTIFIER)
				{
					sc.unget(typeToken);
					break;
				}

				returnTypes.push_back(type_t::get_type(typeToken.getData()));
			}

			sc.get(SourceTokenDHLX::TT_OP_COLON);

			std::string functionName(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());

			std::vector<type_t> argTypes;

			while (true)
			{
				SourceTokenDHLX typeToken(sc.get());

				if (typeToken.getType() != SourceTokenDHLX::TT_IDENTIFIER)
				{
					sc.unget(typeToken);
					break;
				}

				argTypes.push_back(type_t::get_type(typeToken.getData()));
			}

			FOREACH_T(std::vector<type_t>, it, returnTypes)
			{
				switch (it->getNativeType())
				{
				case type_t::NT_NONE:
					break;


				case type_t::NT_BOOL_T:
					FunctionHandler<bool_t>::add_function(functionName, new FunctionHandlerDHLX<bool_t>(sc, argTypes));
					break;


				case type_t::NT_INT_S_T:
					FunctionHandler<int_s_t>::add_function(functionName, new FunctionHandlerDHLX<int_s_t>(sc, argTypes));
					break;

				case type_t::NT_INT_T:
					FunctionHandler<int_t>::add_function(functionName, new FunctionHandlerDHLX<int_t>(sc, argTypes));
					break;

				case type_t::NT_INT_L_T:
					FunctionHandler<int_l_t>::add_function(functionName, new FunctionHandlerDHLX<int_l_t>(sc, argTypes));
					break;


				case type_t::NT_REAL_S_T:
					FunctionHandler<real_s_t>::add_function(functionName, new FunctionHandlerDHLX<real_s_t>(sc, argTypes));
					break;

				case type_t::NT_REAL_T:
					FunctionHandler<real_t>::add_function(functionName, new FunctionHandlerDHLX<real_t>(sc, argTypes));
					break;

				case type_t::NT_REAL_L_T:
					FunctionHandler<real_l_t>::add_function(functionName, new FunctionHandlerDHLX<real_l_t>(sc, argTypes));
					break;


				case type_t::NT_STRING_T:
					FunctionHandler<string_t>::add_function(functionName, new FunctionHandlerDHLX<string_t>(sc, argTypes));
					break;

				case type_t::NT_STRING8_T:
					FunctionHandler<string8_t>::add_function(functionName, new FunctionHandlerDHLX<string8_t>(sc, argTypes));
					break;

				case type_t::NT_STRING16_T:
					FunctionHandler<string16_t>::add_function(functionName, new FunctionHandlerDHLX<string16_t>(sc, argTypes));
					break;

				case type_t::NT_STRING32_T:
					FunctionHandler<string32_t>::add_function(functionName, new FunctionHandlerDHLX<string32_t>(sc, argTypes));
					break;

				case type_t::NT_STRING80_T:
					FunctionHandler<string80_t>::add_function(functionName, new FunctionHandlerDHLX<string80_t>(sc, argTypes));
					break;

				case type_t::NT_STRING320_T:
					FunctionHandler<string320_t>::add_function(functionName, new FunctionHandlerDHLX<string320_t>(sc, argTypes));
					break;


				case type_t::NT_TYPE_T:
					break;


				case type_t::NT_UBYTE_T:
					FunctionHandler<ubyte_t>::add_function(functionName, new FunctionHandlerDHLX<ubyte_t>(sc, argTypes));
					break;

				case type_t::NT_SWORD_T:
					FunctionHandler<sword_t>::add_function(functionName, new FunctionHandlerDHLX<sword_t>(sc, argTypes));
					break;

				case type_t::NT_UWORD_T:
					FunctionHandler<uword_t>::add_function(functionName, new FunctionHandlerDHLX<uword_t>(sc, argTypes));
					break;

				case type_t::NT_SDWORD_T:
					FunctionHandler<sdword_t>::add_function(functionName, new FunctionHandlerDHLX<sdword_t>(sc, argTypes));
					break;

				case type_t::NT_UDWORD_T:
					FunctionHandler<udword_t>::add_function(functionName, new FunctionHandlerDHLX<udword_t>(sc, argTypes));
					break;
				}
			}
		}
		// # include [:] STRING;
		else if (commandString == command_name_include())
		{
			SourceTokenDHLX arg0(sc.get(SourceTokenDHLX::TT_STRING, SourceTokenDHLX::TT_OP_COLON));

			sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

			process_file(arg0.getData());
		}
		// # typedefnew IDENTIFIER IDENTIFIER;
		else if (commandString == command_name_typedefnew())
		{
			std::string mode(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());
			std::string type(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData());
			sc.get(SourceTokenDHLX::TT_OP_SEMICOLON);

			if (mode == "dynamic")
				type_t::add_type(type, type_t::MODE_DYNAMIC);

			else if (mode == "value")
				type_t::add_type(type, type_t::MODE_VALUE);

			else if (mode == "object")
				type_t::add_type(type, type_t::MODE_OBJECT);

			else if (mode == "compoundobject")
				type_t::add_type(type, type_t::MODE_COMPOUNDOBJECT);

			else if (mode == "inline")
				type_t::add_type(type, type_t::MODE_INLINE);

			else
				throw CompilerException("unknown typedefnew:" + mode);
		}
		else
		{
			global_object->doCommand(commandString, sc);
		}
	}
	else if (st.getType() == SourceTokenDHLX::TT_IDENTIFIER)
	{
		sc.unget(st);
		global_object->addObject(sc);
	}
	else
	{
		throw SyntaxException("invalid top-level token:" + make_string(st));
	}
}



