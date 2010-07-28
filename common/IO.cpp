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

#include "IO.hpp"



#ifdef TARGET_OS_WIN32
#include <windows.h>
#include <sys/stat.h>
#define PATHSEP '\\'
#else
#include <dirent.h>
#include <sys/stat.h>
#define PATHSEP '/'
#endif



namespace IO
{



bool isdir(char const * filename)
{
	#ifdef TARGET_OS_WIN32
	struct _stat stat_data;
	int stat_res = _stat(filename, &stat_data);
	#else
	struct stat stat_data;
	int stat_res = stat(filename, &stat_data);
	#endif

	// 0 means success.
	if (stat_res != 0) return false;

	#ifdef TARGET_OS_WIN32
	return (stat_data.st_mode & _S_IFMT) == _S_IFDIR;
	#else
	return S_ISDIR(stat_data.st_mode);
	#endif
}

std::vector<std::string> lsdir(char const * filename)
{
	std::vector<std::string> dirList;

	#ifdef TARGET_OS_WIN32
	WIN32_FIND_DATA findData;
	std::string     findDir(std::string(filename) + "*");
	HANDLE          findResult;

	findResult = FindFirstFile(findDir.c_str(), &findData);

	if (findResult == INVALID_HANDLE_VALUE)
		return dirList;

	do
	{
		std::string findName(findData.cFileName);

		if (findName == "." || findName == "..")
		{
			continue;
		}

		dirList.push_back(findName);
	}
	while (FindNextFile(findResult, &findData) != 0);

	FindClose(findResult);
	#else
	DIR*    dirFile = opendir(filename);
	dirent* nextDir;

	if (dirFile == NULL)
		return dirList;

	while ((nextDir = readdir(dirFile)) != NULL)
	{
		std::string findName(nextDir->d_name);

		if (findName == "." || findName == "..")
		{
			continue;
		}

		dirList.push_back(findName);
	}

	closedir(dirFile);
	#endif

	return dirList;
}

bool mkdir(std::string const & filename, bool recurse)
{
	if (recurse)
	{
		size_t lastsep = filename.find_last_of(PATHSEP);

		if (lastsep != std::string::npos)
		{
			mkdir(filename.substr(0, lastsep), recurse);
		}
	}

	#ifdef TARGET_OS_WIN32
	CreateDirectory(filename.c_str(), NULL);
	#else
	::mkdir(filename.c_str(), S_IRUSR|S_IWUSR|S_IXUSR | S_IRGRP|S_IXGRP | S_IROTH|S_IXOTH);
	#endif

	return isdir(filename.c_str());
}



}



