David's Doom Level Compiler (DH-dlc) and David's WAD Builder (DH-wad)

See README.LANGUAGE.txt for language specs.

For basic usage, run either program without arguments.



DH-dlc is a utility that allows you to write Doom maps in text. It's designed
to be easy to make simple and short maps as well as complex and massive maps.
Having a background in programming is a plus, but all you really need is to be
able to easily visualize your map without a GUI.

Some of the bigger features of DH-dlc include:
	* "Variables" whose values can be set and referenced.
	* Values can be set using math that is written intuitively.
	* The ability to embed scripts in a map, as well as push map values
	  into them.
	* Numbers of unlimited size so that expressions don't overflow in the
	  middle of evaluation.
	* Expression evaluator includes functions for trigonometry and other
	  mathematical concepts.
	* Compound objects that make common/repetitive level components easier,
	  faster, and less bug-prone. Even many complex things can be defined
	  as a compound object, not just "simple" architecture.
	* Support for UDMF or binary output, in some cases without alteration.
	* Commands which can make compound objects more powerful and dynamic.
	* Functions for random number generation combined with other features
	  makes it possible to design a random map generator using DH-dlc.
	* Cross-platform support.
	* Maps that can be written for maintainability, re-usability, and
	  portability (ie, turn a Doom map into a Hexen map).

DH-wad is mostly just a way to convert DH-dlc output files into a usable wad,
but it can be used to make entire WADs for source ports which don't support
other archive types.

DH-dlc was written mainly to do UDMF output, and uses UDMF names for things.
However, it also supports binary output, and is reasonably good at converting
UDMF-style written maps to binary format (automatic flag handling). If
intending to do binary output, flags can be explicitly defined. Hexen format is
also reasonably well supported, with line id and extra flags being set when
possible.

Between DH-dlc and DH-wad, it is possible to create maps that are in fact
original .exe compatible. While I, the author, think this is a cool feature, it
is largely a side effect of current source ports being compatible with original
.exe formats. As noted above, the primary function of DH-dlc is to output UDMF.
Binary output is supported because UDMF is still fairly new, and not many ports
support it. Plus, Doom is a game steeped in history, however brief it's been so
far, so not supporting original binary formats just seems odd.



If found, the GNU Multi-Precision Library (GMPLib) will be used. If it is
not found, it will be disabled automatically. You can force non-use by passing
-DUSE_GMPLIB=N to cmake. GMPLib can be acquired from gmplib.org.

DH-wad should have no requirements beyond a C++ compiler.

In both cases, you will need CMake installed. This can be obtained from
cmake.org (Ubuntu package: cmake).

If using a POSIX compliant OS and shell (or reasonably compliant like Linux),
the following commands will compile the program:
	mkdir release
	cd release
	cmake ..
	make

If using a non-POSIX OS (Windows), then the process is a little more involved.
However, these are the commands I use to do it (I use MinGW and MSYS):
set CMAKE_INCLUDE_PATH=C:\MSYS\1.0\local\include
set CMAKE_LIBRARY_PATH=C:\MSYS\1.0\local\lib
set CPATH=C:\MSYS\1.0\local\include
mkdir release
cd release
cmake .. -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -G "MinGW Makefiles"
mingw32-make

The files in the map-libs directory are library files. std.ddl is automatically
included by default. udmf.ddl can be included instead of std.ddl by using the
--do-udmf-lib option. All other files define very generic compound objects that
are useful for any project. It is strongly suggested that this be added to your
DH_DLC_PATH environment variable.



In order to actually play the maps you write, you need to compile the level and
build a wad out of it.

If working with a single level, the following three commands would work
(replace MAP with your map's name, of course):
	mkdir MAP
	DH-dlc --dir MAP MAP.ddl
	DH-wad --map MAP MAP.wad

If you want to pack multiple maps into a wad or need to include other files in
the wad file, then read on.

To compile, you need a source file and a destination directory. Compiling
should be as simple as (replace destination and source):
	DH-dlc destination source

Building the wad requires that the compiled level directory be in another
directory. Essentially, the following:
./maps/
./maps/E1M1/
./maps/E1M1/TEXTMAP
./E1M1.ddl

With that in place:
	DH-wad maps output

Note that any number of maps could be under ./maps/ in this case. As well as
other things you may want in the wad (like a MAPINFO lump).

If possible, the following simple makefile could be used:
directory = MyMaps
output    = MyMaps.wad

sources := $(wildcard *.ddl)
objects := $(patsubst %.ddl,$(directory)/%,$(sources))

$(output) : $(objects)
	DH-wad $(output) $(directory)
	zdbsp $(output) --output $(output)

$(objects) : $(directory)/% : %.ddl
	mkdir -p $@
	DH-dlc $@ $<
	touch $@



Environment Variables:

Currently, only one environment variable is used, DH_DLC_PATH. It is used as a
colon ':' separated list of paths to look for files in. This includes files
specified at the command line.



File Extensions:

Being primarily a Linux program, file extensions are merely a courtesy. But
that's no reason that I can't document my own naming convention:
	.ddl (DH Doom Level) DH-dlc source files.
	.dhlu (DHlevelUDMF) An earlier version of the compiler that some of the
		libraries were written for.



Additional notes about using DH-wad:

Let me start by saying that while DH-wad can handle special lumps, you are
encouraged to use a .pk3 for non-map data. The handling of non-map lumps is
just for completeness and to be able to make wads for older source ports. This
is much like the support for binary output of DH-dlc.

It's important to note that EVERY file/folder under the primary directory (the
one specified by the command line) will have some kind of effect on output. Do
not leave excess files lying about, unless you are certain they are harmless.
The only exception is excess files in a map directory. Currently, unknown files
are ignored. Since this might be regarded by some as a feature, it will
probably remain, but even still.

Certain directory names are handled specially:
	A  | A_START  | ACS
	FF | FF_START | flats
	HI | HI_START | hires
	SS | SS_START | sprites
	TX | TX_START | textures
		The files in these directories will be placed within the
		appropriate *_START and *_END lumps.

All directories encountered that are not special directories are handled as a
map. This means having a start and end lump. The directory's name is used for
the start lump, and ENDMAP used for the end. More importantly, the order of
files is fixed. Currently, all other files (those not known of) are ignored.

In a map's directory, if a file with the map's name (ie E1M1) is found, it will
be used for the start lump. The same for ENDMAP. This is for certain source
ports that need that (FraggleScript, for instance). The same also applies to
HI_START and related lumps, except that they aren't skipped when the directory
is iterated over, so the results will most likely not be useful. The extensions
.txt and .lmp are accepted for these files as well as no extension, in that
order (.txt, .lmp, none).

Sub-sub directories are cumulative. Sub directories in a level are ignored like
other unknown names.

The following files are added to maps. Extensions are optional. If a file
exists with an extension and without, it will be added twice, so don't do that.
	TEXTMAP.txt
	THINGS.lmp
	LINEDEFS.lmp
	SIDEDEFS.lmp
	VERTEXES.lmp
	SEGS.lmp
	SSECTORS.lmp
	NODES.lmp
	SECTORS.lmp
	REJECT.lmp
	BLOCKMAP.lmp
	BEHAVIOR.o
	SCRIPTS.acs

The following command line options are worth noting:
	--directory DIR Adds files in DIR and sub-directories to the wad
		without checking for special sub-directories. Multiple uses are
		cumulative.

	--explicit Uses args as pairs of strings indicating filename-lumpname.
		If lumpname is empty, use filename. If filename is empty, add
		empty lump. Everything added by --directory and --map is
		ignored. It is strongly suggested that --output be used when
		using this option.

	--iwad Outputs an IWAD instead of a PWAD.

	--map DIR Adds DIR as if it were a map/special directory. Multiple uses
		are cumulative.

	--output FILE Outputs to FILE (does not automatically add extension).



History:

This is a (brief) history of this program. It is not a changelog, nor does it
have any bearing on usage. It's just for historical record.

Let me begin by admitting that I hate GUIs. When they're used properly (to
enhance the flow of information to the user), they're great. When they're used
just to look nice, they are the work of the Devil. So it's not hard to imagine
my disdain for Windows wherein EVERYTHING is a GUI. (And it doesn't come with a
C compiler so it's not really an OS in my book.)

That said, I tried various GUI-based map editors for Doom. Since I was running
Linux, however, my options were limited to native binaries or Wine. At the
time native Linux options were pretty scarce (I'll admit I didn't look very
hard, but I've heard similar things elsewhere), so in actuality, I was limited
to programs that worked in Wine.

Eventually, I found something, which crashed on exit BEFORE saving settings.
(Good argument for save-on-edit, but that's a different subject.) It was nice,
but I found it to be a pain to work with (I never managed to make a decent
door). That, and I was always worried that it was going to crash and wipe out
my map(s). (Yes, I know that's what back-ups are for, but it's not always
practical to do a back-up as often as I save (every ten seconds).)

Since I had a modest understanding of what went into a Doom level, and I had
taken (or was in the middle of taking, I forget now) a Java course, I decided
to write a simple program that would read in text and output Doom level data.
At the time, I wanted to avoid multiple source files for a program, so I made
two programs: DHlevelDOOM and DHwadBUILD (each a single source file).

Needless to say, it was crude. How crude, you ask? Here's the syntax overview:
Include FILENAME

LINEDEF NAME BEGIN  END    FLAGS  ACTION TAG    RIGHT   LEFT
             VERTEX VERTEX UShort UShort UShort SIDEDEF SIDEDEF

SECTOR NAME FLOOR  CEILING FLOOR  CEILING LIGHT  SPECIAL TAG
            SShort SShort  String String  UShort UShort  UShort

SIDEDEF NAME XOFFSET YOFFSET UPPER  LOWER  MIDDLE SECTOR
             SShort  SShort  String String String SECTOR

SShort NAME VALUE
            SShort

String NAME VALUE
            String

THING NAME X      Y      ANGLE  TYPE   FLAGS
           SShort SShort UShort UShort UShort

UShort NAME VALUE
            UShort

VERTEX NAME X      Y
            SShort SShort

Additionally, lines starting with // or COMMENT are ignored. (In the
case of COMMENT, there must be whitespace after to be recognized)

That's an exerpt from its output with no arguments from the latest version. In
the beginning, it was even less feature rich. Here's some things that were not
part of the original design:
	// style comments.
	VALUE types.
	Expression evaluator.
	VERTEX key access (post-creation).

And then I started reading up on Hexen map format (DHlevelDOOM was Doom
format). And it was then that I realized that the compiler and language were
insufficient. And thus was born DHlevelHEX.

This is an excerpt from the lang.txt that I wrote for it:
TYPE[NAME[!][:[#]NAME]];

TYPE[NAME[!][:NAME]]
{
	KEY=VALUE[,
	KEY=VALUE]
	...
}

As an example of what that's trying to say:
VERTEX[Room_1_v1] {x = 0, y = 0}
VERTEX[Room_1_v2:Room_1_v1] {y = 128}

New features (from DHLD):
	No more whitespace dependance.
	Named keys.
	Default values for keys.
	Inheritance for objects (include super access).
	The ability to reference any KEY of any object at any time.
	Could do Hexen format or Doom format.
	Embedded ACS code.

And then I found out about UDMF.

At first I thought it was great, I could write maps in UDMF instead of these
stupid languages I came up with (DHLX was better than DHLD, but even still).
I'll give you a moment to recover from laughter.

Done? So after that fell through, I was still left with the fact that UDMF was
a better way to store maps. And I could use the opportunity to improve my own
language. Thus came DHlevelUDMF.

Here's the basic syntax section of lang.txt:
NAME[.KEY...] = (TYPE) VALUE;
NAME[.KEY...] = TYPE[:NAME...];
[NAME[.KEY...] = ]TYPE[:NAME...]
{
	KEY = (TYPE) VALUE;
	KEY = TYPE[:NAME...];
	KEY = TYPE[:NAME...] { ... }
	COMMAND;
}

New features:
	UDMF support. (No Doom or Hexen format anymore, though.)
	Could define objects in objects.
	Somewhat simpler syntax.
	Compound objects.
	Any KEY could have any type.
	Multiple inheritance. (Not actually useful, just could.)
	Objects were mutable.
	Commands to invoke action. (Not originally, but near the end.)

Having had enough of Java, I then wanted to learn C++. Once I found GMP, I
started DH-dlc with the intention of making a much more extensible and adaptive
language/program. Basically, just look at the list at the top of the document.

In closing, here's hoping that the future is ever-brighter than the past.



