cmake_minimum_required(VERSION 2.6)

project(DH-dlc)



if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_definitions(-DTARGET_OS_WIN32)
	set(PATH_SEP "'\\\\'" CACHE INTERNAL "PATH_SEP" FORCE)
else()
	add_definitions(-DTARGET_OS_UNIX)
	set(PATH_SEP "'/'" CACHE INTERNAL "PATH_SEP" FORCE)
endif()



if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall)
endif()



add_subdirectory(DH-dlc)
add_subdirectory(DH-wad)



