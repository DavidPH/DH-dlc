add_executable(DH-dlc
	compound_objects.cpp
	global_object.cpp
   LevelObject.cpp
	main.cpp
	math.cpp
   option.cpp
	options.cpp
	process_file.cpp
	process_stream.cpp
	process_token.cpp
	scripts.cpp
   SourceException.cpp
	SourceScanner.cpp
	SourceStream.cpp
	SourceToken.cpp
   SourceTokenC.cpp
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

	LevelObject/LevelObject_add.cpp
	LevelObject/LevelObject_do.cpp
	LevelObject/LevelObject_encode.cpp
	LevelObject/LevelObjectData.cpp
	LevelObject/LevelObjectMap.cpp
	LevelObject/LevelObjectName.cpp
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

# EOF

