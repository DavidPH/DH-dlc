sources = main.cpp \
	Lump.cpp \
	options.cpp \
	process_directory.cpp \
	process_file.cpp \
	process_map.cpp \
	../common/IO.cpp

objects = $(sources:.cpp=.o)

ifeq ($(findstring $(host_triplet),mingw32),mingw32)
exename = DH-wad.exe
DEFFLAGS = -DTARGET_OS_WIN32
else
exename = DH-wad
DEFFLAGS = -DTARGET_OS_LINUX
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



