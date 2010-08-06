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

#include "FunctionHandler.hpp"

#include "../types.hpp"

#include "../exceptions/UnknownFunctionException.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"



template<typename T>
typename FunctionHandler<T>::func_map_t * FunctionHandler<T>::func_map;



template<typename T>
FunctionHandler<T> const * FunctionHandler<T>::add_function(std::string const & name, FunctionHandler<T> const * func)
{
	if (!func_map) func_map = new func_map_t;

	return (*func_map)[name] = func;
}
template<typename T>
FunctionHandler<T> const & FunctionHandler<T>::get_function(std::string const & name)
{
	if (!func_map) throw UnknownFunctionException(name);

	FunctionHandler<T> const * func = (*func_map)[name];

	if (!func) throw UnknownFunctionException(name);

	return *func;
}
template<typename T>
bool FunctionHandler<T>::has_function(std::string const & name)
{
	if (!func_map) return false;

	return (*func_map)[name] != NULL;
}



template class FunctionHandler<bool_t>;
template class FunctionHandler<int_s_t>;
template class FunctionHandler<int_t>;
template class FunctionHandler<int_l_t>;
template class FunctionHandler<real_s_t>;
template class FunctionHandler<real_t>;
template class FunctionHandler<real_l_t>;
template class FunctionHandler<string_t>;
template class FunctionHandler<string8_t>;
template class FunctionHandler<string16_t>;
template class FunctionHandler<string32_t>;
template class FunctionHandler<string80_t>;
template class FunctionHandler<string320_t>;
template class FunctionHandler<ubyte_t>;
template class FunctionHandler<sword_t>;
template class FunctionHandler<uword_t>;
template class FunctionHandler<sdword_t>;
template class FunctionHandler<udword_t>;



