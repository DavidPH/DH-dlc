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

#include "FunctionHandlerNative.hpp"

#include "../types.hpp"

#include "../exceptions/UnknownFunctionException.hpp"

#include "../types/binary.hpp"
#include "../types/int_t.hpp"
#include "../types/real_t.hpp"
#include "../types/string_t.hpp"



template<typename T>
FunctionHandlerNative<T>::FunctionHandlerNative() : _funcDDL(NULL), _funcDHLX(NULL)
{

}
template<typename T>
FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDDL_t funcDDL) : _funcDDL(funcDDL), _funcDHLX(NULL)
{

}
template<typename T>
FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDHLX_t funcDHLX) : _funcDDL(NULL), _funcDHLX(funcDHLX)
{

}
template<typename T>
FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDDL_t funcDDL, FunctionHandlerNative<T>::funcDHLX_t funcDHLX) : _funcDDL(funcDDL), _funcDHLX(funcDHLX)
{

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



template class FunctionHandlerNative<bool_t>;
template class FunctionHandlerNative<int_s_t>;
template class FunctionHandlerNative<int_t>;
template class FunctionHandlerNative<int_l_t>;
template class FunctionHandlerNative<real_s_t>;
template class FunctionHandlerNative<real_t>;
template class FunctionHandlerNative<real_l_t>;
template class FunctionHandlerNative<string_t>;
template class FunctionHandlerNative<string8_t>;
template class FunctionHandlerNative<string16_t>;
template class FunctionHandlerNative<string32_t>;
template class FunctionHandlerNative<string80_t>;
template class FunctionHandlerNative<string320_t>;
template class FunctionHandlerNative<ubyte_t>;
template class FunctionHandlerNative<sword_t>;
template class FunctionHandlerNative<uword_t>;
template class FunctionHandlerNative<sdword_t>;
template class FunctionHandlerNative<udword_t>;



