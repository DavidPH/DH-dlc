Include(CheckTypeSize)

CHECK_TYPE_SIZE("void*" SIZEOF_POINTER)

CHECK_TYPE_SIZE(    "short int" SIZEOF_SHORT)
CHECK_TYPE_SIZE(          "int" SIZEOF_INT)
CHECK_TYPE_SIZE(     "long int" SIZEOF_LONG)
CHECK_TYPE_SIZE("long long int" SIZEOF_LONGLONG)

CHECK_TYPE_SIZE(      "float" SIZEOF_FLOAT)
CHECK_TYPE_SIZE(     "double" SIZEOF_DOUBLE)
CHECK_TYPE_SIZE("long double" SIZEOF_LONGDOUBLE)

if(NOT DEFINED CHAR_BIT)
	message(STATUS "Check size of CHAR_BIT")

	set(CHAR_BIT_SOURCE_FILE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/Check_CHAR_BIT.c")
	set(CHAR_BIT_SOURCE
		"#include <limits.h>\n"
		"#include <stdio.h>\n"
		"int main()\n"
		"{\n"
		"	printf(\"%i\", CHAR_BIT)\;\n"
		"	return 0\;\n"
		"}\n"
	)

	FILE(WRITE ${CHAR_BIT_SOURCE_FILE} ${CHAR_BIT_SOURCE})

	TRY_RUN(CHAR_BIT_RUN_RESULT CHAR_BIT_COMPILE_RESULT
		${CMAKE_BINARY_DIR} ${CHAR_BIT_SOURCE_FILE}
		COMPILE_OUTPUT_VARIABLE CHAR_BIT_COMPILE_OUTPUT
		RUN_OUTPUT_VARIABLE CHAR_BIT_RUN_OUTPUT
	)

	if(NOT CHAR_BIT_COMPILE_RESULT)
		message(STATUS "Check size of CHAR_BIT - failed")

		FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
			"Determining size of CHAR_BIT failed to compile with output:\n${CHAR_BIT_COMPILE_OUTPUT}\n\n"
		)

	elseif(CHAR_BIT_RUN_RESULT)
		message(STATUS "Check size of CHAR_BIT - failed")

		FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
			"Determining size of CHAR_BIT failed to run with output:\n${CHAR_BIT_RUN_OUTPUT}\n\n"
		)

	else()
		set(CHAR_BIT ${CHAR_BIT_RUN_OUTPUT} CACHE INTERNAL "CHAR_BIT")

		message(STATUS "Check size of CHAR_BIT - done")
	endif()
endif()

if(NOT DEFINED POINTER_BIT OR NOT DEFINED SPOINTER_MAX OR NOT DEFINED SPOINTER_MIN OR NOT DEFINED UPOINTER_MAX)
	message(STATUS "Generate POINTER limits")

	math(EXPR POINTER_BIT "${SIZEOF_POINTER} * ${CHAR_BIT}")
	math(EXPR POINTER_BIT_MOD4 "${POINTER_BIT} % 4")

	if(${POINTER_BIT_MOD4} EQUAL 3)
		set(RANGE_START 7)

		set(SPOINTER_MAX "0x3")
		set(UPOINTER_MAX "0x7")

	elseif(${POINTER_BIT_MOD4} EQUAL 2)
		set(RANGE_START 6)

		set(SPOINTER_MAX "0x1")
		set(UPOINTER_MAX "0x3")

	elseif(${POINTER_BIT_MOD4} EQUAL 1)
		set(RANGE_START 5)

		set(SPOINTER_MAX "0x0")
		set(UPOINTER_MAX "0x1")

	else()
		set(RANGE_START 8)

		set(SPOINTER_MAX "0x7")
		set(UPOINTER_MAX "0xF")
	endif()

	foreach(i RANGE ${RANGE_START} ${POINTER_BIT} 4)
		set(SPOINTER_MAX "${SPOINTER_MAX}F")
		set(UPOINTER_MAX "${UPOINTER_MAX}F")
	endforeach()

	if(${SIZEOF_POINTER} LESS ${SIZEOF_INT} OR ${SIZEOF_POINTER} EQUAL ${SIZEOF_INT})
		set(UPOINTER_MAX "${UPOINTER_MAX}U")

	elseif(${SIZEOF_POINTER} LESS ${SIZEOF_LONG} OR ${SIZEOF_POINTER} EQUAL ${SIZEOF_LONG})
		set(SPOINTER_MAX "${SPOINTER_MAX}L")
		set(UPOINTER_MAX "${UPOINTER_MAX}UL")

	elseif(HAVE_SIZEOF_LONGLONG)
		set(SPOINTER_MAX "${SPOINTER_MAX}LL")
		set(UPOINTER_MAX "${UPOINTER_MAX}ULL")

	else()
		message(SEND_ERROR "Unable to generate POINTER limits")
	endif()

	set(SPOINTER_MIN "-${SPOINTER_MAX}-1")

	set( POINTER_BIT  ${POINTER_BIT} CACHE INTERNAL  "POINTER_BIT" FORCE)
	set(SPOINTER_MAX ${SPOINTER_MAX} CACHE INTERNAL "SPOINTER_MAX" FORCE)
	set(SPOINTER_MIN ${SPOINTER_MIN} CACHE INTERNAL "SPOINTER_MIN" FORCE)
	set(UPOINTER_MAX ${UPOINTER_MAX} CACHE INTERNAL "UPOINTER_MAX" FORCE)

	message(STATUS "Generate POINTER limits - done")
endif()

if(NOT DEFINED SINT_BIGGEST OR NOT DEFINED UINT_BIGGEST)
	message(STATUS "Determine largest available int")

	if(HAVE_SIZEOF_LONGLONG)
		set(       SINT_BIGGEST   "signed long long int")
		set(       UINT_BIGGEST "unsigned long long int")

		set(SIZEOF_SINT_BIGGEST ${SIZEOF_LONGLONG})
		set(SIZEOF_UINT_BIGGEST ${SIZEOF_LONGLONG})

		set(SINT_BIGGEST_MAX "LLONG_MAX")
		set(SINT_BIGGEST_MIN "LLONG_MIN")
		set(UINT_BIGGEST_MAX "ULLONG_MAX")

	else()
		set(       SINT_BIGGEST   "signed long int")
		set(       UINT_BIGGEST "unsigned long int")

		set(SIZEOF_SINT_BIGGEST ${SIZEOF_LONG})
		set(SIZEOF_UINT_BIGGEST ${SIZEOF_LONG})

		set(SINT_BIGGEST_MAX "LONG_MAX")
		set(SINT_BIGGEST_MIN "LONG_MIN")
		set(UINT_BIGGEST_MAX "ULONG_MAX")

	endif()

	math(EXPR SINT_BIGGEST_BIT "${SIZEOF_SINT_BIGGEST} * ${CHAR_BIT}")
	math(EXPR UINT_BIGGEST_BIT "${SIZEOF_UINT_BIGGEST} * ${CHAR_BIT}")

	set(SINT_BIGGEST     ${SINT_BIGGEST}     CACHE INTERNAL "SINT_BIGGEST"     FORCE)
	set(SINT_BIGGEST_BIT ${SINT_BIGGEST_BIT} CACHE INTERNAL "SINT_BIGGEST_BIT" FORCE)
	set(SINT_BIGGEST_MAX ${SINT_BIGGEST_MAX} CACHE INTERNAL "SINT_BIGGEST_MAX" FORCE)
	set(SINT_BIGGEST_MIN ${SINT_BIGGEST_MIN} CACHE INTERNAL "SINT_BIGGEST_MIN" FORCE)

	set(UINT_BIGGEST     ${UINT_BIGGEST}     CACHE INTERNAL "UINT_BIGGEST"     FORCE)
	set(UINT_BIGGEST_BIT ${UINT_BIGGEST_BIT} CACHE INTERNAL "UINT_BIGGEST_BIT" FORCE)
	set(UINT_BIGGEST_MAX ${UINT_BIGGEST_MAX} CACHE INTERNAL "UINT_BIGGEST_MAX" FORCE)

	message(STATUS "Determine largest available int - done")

endif()

if(NOT DEFINED FLOAT_BIGGEST)
	message(STATUS "Determine largest available float")

	if(HAVE_SIZEOF_LONGDOUBLE)
		set(       FLOAT_BIGGEST "long double")
		set(SIZEOF_FLOAT_BIGGEST ${SIZEOF_LONGDOUBLE})

		set(FLOAT_BIGGEST_MAX "LDBL_MAX")
		set(FLOAT_BIGGEST_MIN "LDBL_MIN")

	elseif(HAVE_SIZEOF_DOUBLE)
		set(       FLOAT_BIGGEST "double")
		set(SIZEOF_FLOAT_BIGGEST ${SIZEOF_DOUBLE})

		set(FLOAT_BIGGEST_MAX "DBL_MAX")
		set(FLOAT_BIGGEST_MIN "DBL_MIN")

	else()
		set(       FLOAT_BIGGEST "float")
		set(SIZEOF_FLOAT_BIGGEST ${SIZEOF_FLOAT})

		set(FLOAT_BIGGEST_MAX "FLT_MAX")
		set(FLOAT_BIGGEST_MIN "FLT_MIN")

	endif()

	math(EXPR FLOAT_BIGGEST_BIT "${SIZEOF_FLOAT_BIGGEST} * ${CHAR_BIT}")

	set(FLOAT_BIGGEST     ${FLOAT_BIGGEST}     CACHE INTERNAL "FLOAT_BIGGEST"     FORCE)
	set(FLOAT_BIGGEST_BIT ${FLOAT_BIGGEST_BIT} CACHE INTERNAL "FLOAT_BIGGEST_BIT" FORCE)
	set(FLOAT_BIGGEST_MAX ${FLOAT_BIGGEST_MAX} CACHE INTERNAL "FLOAT_BIGGEST_MAX" FORCE)
	set(FLOAT_BIGGEST_MIN ${FLOAT_BIGGEST_MIN} CACHE INTERNAL "FLOAT_BIGGEST_MIN" FORCE)

	message(STATUS "Determine largest available float - done")

endif()

if(NOT DEFINED USE_GMPLIB)
	# Default to enabled.
	set(USE_GMPLIB Y)
endif()

if (USE_GMPLIB)
	find_path(GMPXX_INCLUDE_DIR gmpxx.h)
	find_library(GMPXX_LINK_DIR gmpxx.h)

	if (NOT GMPXX_INCLUDE_DIR)
		#message(SEND_ERROR "GMPXX include files not found")

		message(STATUS "GMPXX include files not found")

		add_definitions(-DUSE_GMPLIB=0)

		set(USE_GMPLIB N)

	elseif (NOT GMPXX_LINK_DIR)
		#message(SEND_ERROR "GMPXX link files not found")

		message(STATUS "GMPXX link files not found")

		add_definitions(-DUSE_GMPLIB=0)

		set(USE_GMPLIB N)

	else()
		message(STATUS "GMPXX include files found at ${GMPXX_INCLUDE_DIR}")

		message(STATUS "GMPXX link files found at ${GMPXX_LINK_DIR}")

		include_directories("${GMPXX_INCLUDE_DIR}")

		link_directories("${GMPXX_LINK_DIR}")

		add_definitions(-DUSE_GMPLIB=1)
	endif()
else()

	add_definitions(-DUSE_GMPLIB=0)
endif()

set(TYPES_LIMITS_CMAKE_H ${CMAKE_CURRENT_SOURCE_DIR}/types_limits_cmake.hpp)
set(TYPES_LIMITS_H       ${CMAKE_CURRENT_BINARY_DIR}/types_limits.hpp)

if(${TYPES_LIMITS_CMAKE_H} IS_NEWER_THAN ${TYPES_LIMITS_H})
	message(STATUS "Configure types_limits.hpp")

	configure_file(${TYPES_LIMITS_CMAKE_H} ${TYPES_LIMITS_H})

	message(STATUS "Configure types_limits.hpp - done")
endif()

add_executable(DH-dlc
	compound_objects.cpp
	global_object.cpp
	main.cpp
	math.cpp
	options.cpp
	process_file.cpp
	process_stream.cpp
	process_token.cpp
	scripts.cpp
	SourceScanner.cpp
	SourceStream.cpp
	SourceToken.cpp
	types.cpp

	exceptions/CompilerException.cpp
	exceptions/FunctionException.cpp
	exceptions/InvalidTypeException.cpp
	exceptions/NoDefaultTypeException.cpp
	exceptions/NoSuchElementException.cpp
	exceptions/ParsingException.cpp
	exceptions/SyntaxException.cpp
	exceptions/UnknownCommandException.cpp
	exceptions/UnknownFunctionException.cpp

	LevelObject/LevelObject.cpp
	LevelObject/LevelObject_add.cpp
	LevelObject/LevelObject_do.cpp
	LevelObject/LevelObject_encode.cpp
	LevelObject/LevelObjectData.cpp
	LevelObject/LevelObjectMap.cpp
	LevelObject/LevelObjectName.cpp
	LevelObject/LevelObjectPointer.cpp
	LevelObject/LevelObjectType.cpp

	parsing/FunctionHandler.cpp
	parsing/FunctionHandlerDDL.cpp
	parsing/FunctionHandlerDHLX.cpp
	parsing/FunctionHandlerNative.cpp
	parsing/parsing.cpp
	parsing/parsing_functions.cpp

	types/binary.cpp
	types/int_t.cpp
	types/real_t.cpp
	types/string_t.cpp

	../common/IO.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(USE_GMPLIB)
	target_link_libraries(DH-dlc gmp gmpxx)
endif()



