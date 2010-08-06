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

#include "FunctionHandler.hpp"

#include "../types.hpp"

#include "../exceptions/UnknownFunctionException.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"



template<typename T>
FunctionHandler<T> const * FunctionHandler<T>::add_function(std::string const & name, FunctionHandler<T> const * func)
{
	if (!function_map) function_map = new std::map<std::string, FunctionHandler<T> const *>;

	return (*function_map)[name] = func;
}
template<typename T>
FunctionHandler<T> const & FunctionHandler<T>::get_function(std::string const & name)
{
	if (!function_map) throw UnknownFunctionException(name);

	FunctionHandler<T> const * func = (*function_map)[name];

	if (!func) throw UnknownFunctionException(name);

	return *func;
}
template<typename T>
bool FunctionHandler<T>::has_function(std::string const & name)
{
	if (!function_map) return false;

	return (*function_map)[name] != NULL;
}

template<typename T>
T FunctionHandlerNative<T>::operator () (SourceScannerDHLX & sc) const
{
	if (!_funcDHLX) throw UnknownFunctionException("function not available in DHLX");

	return _funcDHLX(sc);
}
template<typename T>
T FunctionHandlerNative<T>::operator () (std::vector<std::string> const & args) const
{
	if (!_funcDDL) throw UnknownFunctionException("function not available in DDL");

	return _funcDDL(args);
}



#define INSTANCE_TEMPLATE(TYPE) \
template class FunctionHandlerNative<TYPE##_t>

INSTANCE_TEMPLATE(bool);
INSTANCE_TEMPLATE(int_s);
INSTANCE_TEMPLATE(int);
INSTANCE_TEMPLATE(int_l);
INSTANCE_TEMPLATE(real_s);
INSTANCE_TEMPLATE(real);
INSTANCE_TEMPLATE(real_l);
INSTANCE_TEMPLATE(string);
INSTANCE_TEMPLATE(string8);
INSTANCE_TEMPLATE(string16);
INSTANCE_TEMPLATE(string32);
INSTANCE_TEMPLATE(string80);
INSTANCE_TEMPLATE(string320);
INSTANCE_TEMPLATE(ubyte);
INSTANCE_TEMPLATE(sword);
INSTANCE_TEMPLATE(uword);
INSTANCE_TEMPLATE(sdword);
INSTANCE_TEMPLATE(udword);

#undef INSTANCE_TEMPLATE



