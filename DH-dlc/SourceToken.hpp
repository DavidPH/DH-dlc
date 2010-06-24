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



class SourceToken
{
	public:
		SourceToken();
		SourceToken(SourceStream&);

		void clear();

		bool empty() const;

		std::string const & getName() const;
		std::string const & getType() const;

		std::string const & getValue() const;
		std::string const & getData() const;

		std::vector<std::string> const & getBase() const;
		std::string const & getBase(size_t, std::string const & = "") const;

		friend std::ostream& operator << (std::ostream&, const SourceToken&);

	private:
		std::string type, name, value, data;
		std::vector<std::string> base;
};



inline void SourceToken::clear()
{
	type.clear();
	name.clear();
	value.clear();
	data.clear();
	base.clear();
}



inline bool SourceToken::empty() const
{
	return type.empty() && name.empty() && value.empty() && data.empty() && base.empty();
}



inline std::string const & SourceToken::getName() const
{
	return name;
}
inline std::string const & SourceToken::getType() const
{
	return type;
}
inline std::string const & SourceToken::getValue() const
{
	return value;
}
inline std::string const & SourceToken::getData() const
{
	return data;
}

inline std::vector<std::string> const & SourceToken::getBase() const
{
	return base;
}
inline std::string const & SourceToken::getBase(size_t index, std::string const & def) const
{
	if (index < base.size())
		return base[index];
	else
		return def;
}



std::ostream& operator << (std::ostream&, const SourceToken&);

SourceStream& operator >> (SourceStream& in, SourceToken& out);



#endif



