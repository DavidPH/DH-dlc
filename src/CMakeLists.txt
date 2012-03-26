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

if(USE_GMPLIB)
	target_link_libraries(DH-dlc gmp gmpxx)
endif()



