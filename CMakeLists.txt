cmake_minimum_required(VERSION 2.6)

project(DH-dlc)



if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_definitions(-DTARGET_OS_WIN32 -DPATH_SEP='\\\\')
else()
	add_definitions(-DTARGET_OS_UNIX -DPATH_SEP='/')
endif()



if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall)
endif()



add_subdirectory(DH-dlc)



