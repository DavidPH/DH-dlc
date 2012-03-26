/*
    Copyright 2010 David Hill

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

#include "FunctionHandlerDHLX.hpp"

#include "parsing.hpp"

#include "../options.hpp"

#include "../exceptions/UnknownFunctionException.hpp"

#include "../LevelObject/LevelObjectPointer.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"



template<typename T>
FunctionHandlerDHLX<T>::FunctionHandlerDHLX() : _argt(), _data()
{

}
template<typename T>
FunctionHandlerDHLX<T>::FunctionHandlerDHLX(SourceScannerDHLX & sc) : _argt(), _data(sc.getblock(SourceTokenDHLX::TT_OP_BRACE_O, SourceTokenDHLX::TT_OP_BRACE_C))
{

}
template<typename T>
FunctionHandlerDHLX<T>::FunctionHandlerDHLX(SourceScannerDHLX & sc, std::vector<type_t> const & argt) : _argt(argt), _data(sc.getblock(SourceTokenDHLX::TT_OP_BRACE_O, SourceTokenDHLX::TT_OP_BRACE_C))
{

}
template<typename T>
FunctionHandlerDHLX<T>::~FunctionHandlerDHLX()
{

}

template<typename T>
T FunctionHandlerDHLX<T>::operator () (SourceScannerDHLX & sc) const
{
	obj_t funcObj = LevelObject::create();

	obj_t returnType = LevelObject::create(type_t::type_type(), type_t::type_auto<T>());
	funcObj->addObject(name_t::name_return_type, returnType);

	int_s_t argc(0);

	while (true)
	{
		SourceTokenDHLX argTerm(sc.get());
		SourceTokenDHLX::TokenType argTermType(argTerm.getType());
		sc.unget(argTerm);

		if (argTermType == SourceTokenDHLX::TT_OP_PARENTHESIS_C)
			break;

		obj_t argObj;
		if (size_t(argc) < _argt.size())
			argObj = LevelObject::create(_argt[argc], sc);
		else
			argObj = LevelObject::create(type_t::get_type(sc.get(SourceTokenDHLX::TT_IDENTIFIER).getData()), sc);

		name_t argName(parse_name(key_name_arg() + make_string(argc)));
		funcObj->addObject(argName, argObj);

		argc += 1;

		argTerm     = sc.get();
		argTermType = argTerm.getType();

		if (argTermType == SourceTokenDHLX::TT_OP_PARENTHESIS_C)
		{
			sc.unget(argTerm);
			break;
		}
		else if (argTermType != SourceTokenDHLX::TT_OP_COMMA)
		{
			throw CompilerException("Unexpected token " + make_string(argTermType));
		}
	}

	obj_t argcObj = LevelObject::create(type_t::type_shortint(), argc);
	funcObj->addObject(name_t(key_name_argc()), argcObj);

	SourceScannerDHLX data(_data);
	funcObj->addData(data);

	obj_t returnValue = funcObj->getObject(name_t::name_return_value);
	return convert<T, obj_t>(returnValue);
}
template<typename T>
T FunctionHandlerDHLX<T>::operator () (std::vector<std::string> const & args) const
{
	throw UnknownFunctionException("function not available in DDL");
}



template class FunctionHandlerDHLX<bool_t>;
template class FunctionHandlerDHLX<int_s_t>;
template class FunctionHandlerDHLX<int_t>;
template class FunctionHandlerDHLX<int_l_t>;
template class FunctionHandlerDHLX<real_s_t>;
template class FunctionHandlerDHLX<real_t>;
template class FunctionHandlerDHLX<real_l_t>;
template class FunctionHandlerDHLX<string_t>;
template class FunctionHandlerDHLX<string8_t>;
template class FunctionHandlerDHLX<string16_t>;
template class FunctionHandlerDHLX<string32_t>;
template class FunctionHandlerDHLX<string80_t>;
template class FunctionHandlerDHLX<string320_t>;
template class FunctionHandlerDHLX<ubyte_t>;
template class FunctionHandlerDHLX<sword_t>;
template class FunctionHandlerDHLX<uword_t>;
template class FunctionHandlerDHLX<sdword_t>;
template class FunctionHandlerDHLX<udword_t>;



