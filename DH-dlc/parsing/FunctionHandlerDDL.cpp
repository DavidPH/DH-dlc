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

#include "FunctionHandlerDDL.hpp"

#include "../options.hpp"

#include "../exceptions/UnknownFunctionException.hpp"

#include "../LevelObject/LevelObjectPointer.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"



template<typename T>
FunctionHandlerDDL<T>::FunctionHandlerDDL() : _argt(), _data()
{

}
template<typename T>
FunctionHandlerDDL<T>::FunctionHandlerDDL(std::string const & data) : _argt(), _data(data)
{

}
template<typename T>
FunctionHandlerDDL<T>::FunctionHandlerDDL(std::string const & data, std::vector<type_t> const & argt) : _argt(argt), _data(data)
{

}
template<typename T>
FunctionHandlerDDL<T>::~FunctionHandlerDDL()
{

}

template<typename T>
T FunctionHandlerDDL<T>::operator () (SourceScannerDHLX & sc) const
{
	throw UnknownFunctionException("function not available in DHLX");
}
template<typename T>
T FunctionHandlerDDL<T>::operator () (std::vector<std::string> const & args) const
{
	obj_t funcObj = LevelObject::create();

	// TODO: Should be able to make a LevelObject that contains a LevelObjectType.
	obj_t returnType = LevelObject::create(type_t::type_string(), type_t::type_auto<T>().makeString());
	funcObj->addObject(name_t::name_return_type, returnType);

	obj_t argcObj = LevelObject::create(type_t::type_shortint(), int_s_t(args.size()));
	funcObj->addObject(name_t(key_name_argc()), argcObj);

	for (size_t index = 0; index < args.size(); ++index)
	{
		// TODO: Should actually make use of the _argt vector.
		// TODO: Should make an arg_t for undefined types.
		obj_t argObj = LevelObject::create(type_t::type_string(), string_t(args[index]));

		name_t argName(std::string(key_name_arg()) + make_string(index));

		funcObj->addObject(argName, argObj);
	}

	funcObj->addData(_data);

	obj_t returnValue = funcObj->getObject(name_t::name_return_value);
	// XXX: Need to be able to do conversion with template types.
	//return Tconv(returnValue);
	return T();
}



template class FunctionHandlerDDL<bool_t>;
template class FunctionHandlerDDL<int_s_t>;
template class FunctionHandlerDDL<int_t>;
template class FunctionHandlerDDL<int_l_t>;
template class FunctionHandlerDDL<real_s_t>;
template class FunctionHandlerDDL<real_t>;
template class FunctionHandlerDDL<real_l_t>;
template class FunctionHandlerDDL<string_t>;
template class FunctionHandlerDDL<string8_t>;
template class FunctionHandlerDDL<string16_t>;
template class FunctionHandlerDDL<string32_t>;
template class FunctionHandlerDDL<string80_t>;
template class FunctionHandlerDDL<string320_t>;
template class FunctionHandlerDDL<ubyte_t>;
template class FunctionHandlerDDL<sword_t>;
template class FunctionHandlerDDL<uword_t>;
template class FunctionHandlerDDL<sdword_t>;
template class FunctionHandlerDDL<udword_t>;



