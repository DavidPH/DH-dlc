/*
    Copyright 2009, 2010 David Hill

    This file is part of DH-wad.

    DH-wad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DH-wad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DH-wad.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	2010/05/08 - Attempt to add Windows directory iteration.
*/

#include "process_directory.hpp"

#ifdef TARGET_OS_WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <string>

#include "process_file.hpp"
#include "process_map.hpp"



void process_directory(std::string const & dirName, bool primeDir)
{
	#ifdef TARGET_OS_WIN32
	WIN32_FIND_DATA findData;
	std::string     findDir(dirName + "*");
	HANDLE          findResult;

	findResult = FindFirstFile(findDir.c_str(), &findData);

	if (findResult == INVALID_HANDLE_VALUE)
		return;

	do
	{
		std::string fileName(findData.cFileName);

		if (fileName == "." || fileName == "..")
		{
			continue;
		}

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (primeDir)
			{
				process_map(dirName, fileName);
			}
			else
			{
				process_directory(fileName, false);
			}
		}
		else
		{
			process_file(dirName, fileName, fileName);
		}
	}
	while (FindNextFile(findResult, &findData) != 0);

	FindClose(findResult);
	#else
	DIR*    dirFile = opendir(dirName.c_str());
	dirent* nextDir;

	if (dirFile == NULL)
		return;

	while ((nextDir = readdir(dirFile)) != NULL)
	{
		std::string fileName(nextDir->d_name);

		if (fileName == "." || fileName == "..")
		{
			continue;
		}

		if (nextDir->d_type == DT_DIR)
		{
			if (primeDir)
			{
				process_map(dirName, fileName);
			}
			else
			{
				process_directory(fileName, false);
			}
		}
		else if (nextDir->d_type == DT_REG)
		{
			process_file(dirName, fileName, fileName);
		}
		else
		{
			continue;
		}
	}

	closedir(dirFile);
	#endif
}



