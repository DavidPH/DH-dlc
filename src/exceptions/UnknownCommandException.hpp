/*
    Copyright 2009 David Hill

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

#ifndef UNKNOWNCOMMANDEXCEPTION_H
#define UNKNOWNCOMMANDEXCEPTION_H

#include <string>

#include "CompilerException.hpp"



class UnknownCommandException : public CompilerException
{
	public:
		UnknownCommandException() throw();
		UnknownCommandException(const char*) throw();
		UnknownCommandException(const std::string&) throw();

		virtual const char* whatClass() const throw();
};



#endif /* UNKNOWNCOMMANDEXCEPTION_H */



