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
	This class stores the name of a LevelObject in an objmap_t. Previously,
	str_t was used, but I'm changing that to try and make the code clearer.
	(And avoid potential "double dipping" of parse_name calls.)

	2010/03/02 - Original version.
	2010/06/24 - Make constructors explicit.
*/

#ifndef LEVELOBJECTNAME_H
#define LEVELOBJECTNAME_H

#include <ostream>
#include <string>
#include <vector>



class LevelObjectName
{
	public:
		         LevelObjectName(LevelObjectName const & other, size_t count = -1);
		/*
			This constructor is for special use, and does NOT parse
			the name. Does not even break up by '.'.
		*/
		explicit LevelObjectName(char const * name);
		explicit LevelObjectName(std::string const & raw_name);

		bool empty() const;

		bool isPrivate() const; // Not used yet.
		bool isVolatile() const; // For automatic purging.

		LevelObjectName getFirst() const;
		LevelObjectName getRest() const;

		std::string getString(size_t index = 0) const;

		size_t size() const;

		LevelObjectName & operator = (LevelObjectName const & other);

		bool operator < (LevelObjectName const & other) const;



		friend int cmp(LevelObjectName const &, LevelObjectName const &);

		friend std::ostream & operator << (std::ostream &, LevelObjectName const &);

	private:
		/*
			Only for internal use. Does not initialize to anything.
		*/
		explicit LevelObjectName();

		std::vector<std::string> _name;
};



int cmp(LevelObjectName const &, LevelObjectName const &);

std::ostream & operator << (std::ostream &, LevelObjectName const &);



#endif /* LEVELOBJECTNAME_H */



