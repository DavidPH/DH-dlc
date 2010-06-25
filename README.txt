David's Doom Level Compiler (DH-dlc) and David's WAD Builder (DH-wad)



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
