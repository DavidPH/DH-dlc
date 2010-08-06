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

#include "../SourceScanner.hpp"

#include <map>
#include <string>
#include <vector>



template<typename T>
class FunctionHandler
{
	public:
		virtual T operator () (SourceScannerDHLX & sc) const = 0;
		virtual T operator () (std::vector<std::string> const & args) const = 0;

		static FunctionHandler<T> const * add_function(std::string const & name, FunctionHandler<T> const * func);
		static FunctionHandler<T> const & get_function(std::string const & name);
		static bool                       has_function(std::string const & name);

	private:
		static std::map<std::string, FunctionHandler<T> const *> * function_map;
};

/*
	Handles built-in ("native") functions.
*/
template<typename T>
class FunctionHandlerNative : public FunctionHandler<T>
{
	public:
		typedef T(*funcDDL_t)(std::vector<std::string> const &);
		typedef T(*funcDHLX_t)(SourceScannerDHLX &);

		explicit FunctionHandlerNative();
		explicit FunctionHandlerNative(funcDDL_t);
		explicit FunctionHandlerNative(funcDHLX_t);
		explicit FunctionHandlerNative(funcDDL_t, funcDHLX_t);

		virtual T operator () (SourceScannerDHLX & sc) const;
		virtual T operator () (std::vector<std::string> const & args) const;

	private:
		funcDDL_t  _funcDDL;
		funcDHLX_t _funcDHLX;
};



template<typename T>
inline FunctionHandlerNative<T>::FunctionHandlerNative() : _funcDDL(NULL), _funcDHLX(NULL)
{

}
template<typename T>
inline FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDDL_t funcDDL) : _funcDDL(funcDDL), _funcDHLX(NULL)
{

}
template<typename T>
inline FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDHLX_t funcDHLX) : _funcDDL(NULL), _funcDHLX(funcDHLX)
{

}
template<typename T>
inline FunctionHandlerNative<T>::FunctionHandlerNative(FunctionHandlerNative<T>::funcDDL_t funcDDL, FunctionHandlerNative<T>::funcDHLX_t funcDHLX) : _funcDDL(funcDDL), _funcDHLX(funcDHLX)
{

}



