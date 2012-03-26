sources = main.cpp \
	compound_objects.cpp \
	global_object.cpp \
	math.cpp \
	options.cpp \
	process_file.cpp \
	process_stream.cpp \
	process_token.cpp \
	scripts.cpp \
	SourceScanner.cpp \
	SourceStream.cpp \
	SourceToken.cpp \
	types.cpp \
	exceptions/CompilerException.cpp \
	exceptions/FunctionException.cpp \
	exceptions/InvalidTypeException.cpp \
	exceptions/NoDefaultTypeException.cpp \
	exceptions/NoSuchElementException.cpp \
	exceptions/ParsingException.cpp \
	exceptions/SyntaxException.cpp \
	exceptions/UnknownCommandException.cpp \
	exceptions/UnknownFunctionException.cpp \
	LevelObject/LevelObject.cpp \
	LevelObject/LevelObject_add.cpp \
	LevelObject/LevelObject_do.cpp \
	LevelObject/LevelObject_encode.cpp \
	LevelObject/LevelObjectData.cpp \
	LevelObject/LevelObjectMap.cpp \
	LevelObject/LevelObjectName.cpp \
	LevelObject/LevelObjectPointer.cpp \
	LevelObject/LevelObjectType.cpp \
	parsing/FunctionHandler.cpp \
	parsing/FunctionHandlerDDL.cpp \
	parsing/FunctionHandlerDHLX.cpp \
	parsing/FunctionHandlerNative.cpp \
	parsing/parsing.cpp \
	parsing/parsing_functions.cpp \
	types/binary.cpp \
	types/int_t.cpp \
	types/real_t.cpp \
	types/string_t.cpp \
	../common/IO.cpp

objects = $(sources:.cpp=.o)

ifeq ($(findstring $(host_triplet),mingw32),mingw32)
exename = DH-dlc.exe
DEFFLAGS = -DTARGET_OS_WIN32 -DUSE_GMPLIB=0
else
exename = DH-dlc
DEFFLAGS = -DTARGET_OS_LINUX -DUSE_GMPLIB=0
endif

.PHONY: all
all: $(exename)

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS) $(CXXFLAGS) $(DEFFLAGS)

$(exename) : $(objects)
	$(CXX) $(LDFLAGS) $(objects) $(LDLIBS) -o $(exename)

.PHONY: clean
clean:
	rm -f $(objects) $(exename)



