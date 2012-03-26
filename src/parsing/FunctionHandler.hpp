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

#ifndef HPP_FunctionHandler__PARSING_
#define HPP_FunctionHandler__PARSING_

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
		typedef std::map<std::string, FunctionHandler<T> const *> func_map_t;

		static func_map_t * func_map;
};



#endif /* HPP_FunctionHandler__PARSING_ */



