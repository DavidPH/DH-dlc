/*
    Copyright 2009, 2010, 2011 David Hill

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

#include "process_file.hpp"

#include "options.hpp"
#include "process_stream.hpp"
#include "SourceStream.hpp"
#include "SourceToken.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>



void process_file(std::string const & filename)
{
	static std::map<std::string, bool> filenameLoaded;

	if (filename.empty())
		return;

	if (filenameLoaded[filename])
		return;

	filenameLoaded[filename] = true;

	std::ifstream sourceFile(filename.c_str());

	if (!sourceFile)
	{
		for (size_t index = 0; index < option_include.size(); ++index)
		{
			sourceFile.close();
			sourceFile.clear();
			sourceFile.open((option_include[index] + filename).c_str());

			if (sourceFile)
				break;
		}

		if (!sourceFile)
		{
			std::cerr << "file not found:" << filename << '\n';
			return;
		}
	}

	std::string idstring;
	std::getline(sourceFile, idstring);
	sourceFile.seekg(0);

	if (idstring == "//DDL")
	{
		SourceStream ss(sourceFile);

		process_stream<SourceTokenDDL>(ss, filename);
	}
	else if (idstring == "//DHLX")
	{
		SourceStream ss(sourceFile, SourceStream::ST_DHLX);

		process_stream<SourceTokenDHLX>(ss, filename);
	}
	else
	{
		SourceStream ss(sourceFile);

		process_stream<SourceTokenDDL>(ss, filename);
	}

	sourceFile.close();
}



