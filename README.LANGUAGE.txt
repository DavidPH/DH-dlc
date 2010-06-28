David's Doom Level Compiler (DH-dlc) Language

Sometimes the term "Implementation NOTE:" is used. I use that to indicate
something that works in a way which should be considered not a part of the
language, but is merely a side effect of how the program works. If you want
forward compatibility, don't rely on it being true forever.



Sections in this file are listed alphabetically. I'd suggest reading the
section on Basic Syntax first.



Arrays: (or Dynamic Names):
	name[index]
	name<key>

Appends the result of index to name. Can be used in expressions and for bases.

Because the result of index is just appended to name, the following are
equivalent:
	v[1] = 0;
	v<1> = 0;
	v1   = 0;

As such, it is possible to mix dynamic and static names easily. As far as I'm
concerned, this is a feature and should be treated as such.

The difference between <> and [] is that [] is evaluated as an int, while <> is
evaluated as a string.



Basic Syntax:
	[type] name:base = value {data}

:base can be repeated as many times as desired, in the event of duplicate key
names, the last base listed will take priority.

Every file is broken into tokens like this. In cases where {data} is omitted,
the statement must instead end with a semicolon ';'. It is valid syntax to have
duplicate or extraneous semicolons, as in C. Such statements shall do nothing.

Not all statements will have all elements. Additionally, there are some
variations depending on what the statement is intended to do.

It is possible to move [type] to before value. When doing so, it can be changed
to (type), but this loses the ability to be quoted (see Quoting). For example,
the following are all equivalent:
	[int] T_ZombieMan = 3004;
	T_ZombieMan = [int] 3004;
	T_ZombieMan = (int) 3004;

A word of caution, one cannot use [type] in BOTH places, although one CAN use
[type] and (type). For example:
	[int] T_ZombieMan = [int] 3004; // error
	[int] T_ZombieMan = (int) 3004; // OK (see Type Casting)

	// legal, but creates the identifier (int)T_ZombieMan:
	(int) T_ZombieMan = [int] 3004;
	(int) T_ZombieMan = (int) 3004;

For the reason (and explanation of the error message you get), see Unary
Functions.

Type is not always required. For example, the #defaulttype command sets a
default type (see Commands). Also, if assigning a variable to another variable
not in an expression, the type defaults to the other type. This is the order
that the type is gathered from (in all cases, name can be omitted):
	[type] name:base = value {data}
	name:base = [type] value {data}
	name:base = (type) value {data}
	name:base = type {data}
	type:base = value {data}
	name:base = value {data} // will use first base's type
	name      = value {data} // if name has already been defined, it will retain its type.
	name      = name2 {data} // type is name2's type
	name      = value {data} // if #defaulttype has been used for name

When declaring objects, the name is optional. As in:
	[LINEDEF] {id = 1;}
	LINEDEF   {id = 2;} // UDMF-style declaration

If extending an object, the type and/or name may be omitted:
	[LINEDEF] a_line {id = 1;}
	another_line:a_line {id = 2;}
	:a_line {id = 3;}
	THING:a_line {type = 3004;} // cross-type bases are legal

As mentioned earlier, object type can be provided in the place of value:
	a_line = LINEDEF {id = 1;}

In the above example, the data could be omitted, and replaced by a semicolon:
	[LINEDEF] a_line;
	a_line = LINEDEF;
Creating an empty object which can have keys added later:
	[int] a_line.id = 5;
The object.key notation can be used with unlimited depth, and works much the
same as otherwise. (The [int] wasn't needed, since id has a default type.)

Syntax for commands follows the above, with the caveat that [type] and value
are without meaning (but legal). All commands start with #. A name that starts
with # but is not a known command is an error. Commands use :base elements for
arguments. For more information and a list of known commands, see Commands.



Commands:
	#command:arg;
	#command:arg {data}

An unknown command is an error.

The copious whitespace in documenting commands is just there for readability.
It is not required, but is valid.

Global only commands:

# default type : NAME : TYPE : CONTEXT;
NAME is the key name to give a default type to.
TYPE is the default type to give.
CONTEXT is the type in which this default type applies. If not given or empty,
applies to global assignment.

Examples:
	# default type : x : float : VERTEX;
	# default type : namespace : string;

# define : TYPE {data}
Defines a compound object. When compounding as TYPE, data is added to the
compounded object. See Compound Objects.

[RETURN TYPE] # function : NAME {DATA}
Defines a custom function that is invoked like a normal function. Arguments can
be accessed by arg0, arg1, arg2, etc. The number of args is stored in argc. The
command #return is used to return a value. If the function ends without
returning a value, the result is undefined.

# include : FILENAME;
Processes the indicated file. If file is not found, then directories indicated
by --include are checked, followed by those in the environment variable
DH_DLC_PATH. If file is still not found, an error message is printed but it
does not count against --error-limit.

It is important to quote FILENAME on case sensitive file systems, or else it
may be read as lowercase or uppercase, depending on options.

Example:
	# include : "Hall.ddl";

# precision : NEW PRECISION;
Changes internal precision of floats and ints, overriding --precision. This is
a holdover from the prior compiler (DHLU), and is only kept because I don't
have any particular reason to remove it (and it COULD become useful some day).

# typedef : OLD TYPE : NEW TYPE;
Allows the use of NEW TYPE in the place of OLD TYPE for types.

Example:
	# typedef : ZDoomDoor : Door;
	Door { /* data */ }

# typedef new : TYPE : value|object|compound object;
Defines a new type. The second argument changes how the new type is handled
internally.

value: If bool, [short|long] float, [short|long] int, or string, is used as
dictated internally. Else, acts as an object that doesn't get output.
object: Objects of this type are output.
compound object: Objects of this type are not output, and automatically get
compounded (see Compound Objects).

Conditional commands:
# if [not] [bool|exists|false|true] [local] : ARG ... {data}
Defaults to bool.

Adding not inverts the check.

Adding bool checks that each ARG parsed as a bool is true.

Adding exists adds data if each ARG has been declared.

Adding false makes it a boolean test that is true if each ARG does not exist
or is false in a boolean sense (zero or empty string).

Adding true makes it a boolean test that is true if each ARG exists and is
true in a boolean sense (non-zero, non-empty string).

Adding local makes command only check the current object. Does not affect bool.

Under any circumstance I can think of, not false == true and not true == false.
This, however, is not a guarantee on my part, merely an observation.

If no ARG is provided, always true.

# if cmp : NAME1 : OP : NAME2 : TYPE {data}
If TYPE is given, NAMEs are taken as values to be parsed.
Effect depends on OP:
	eq: Adds data if NAME1 == NAME2.
	ne: Adds data if NAME1 != NAME2.
	gt: Adds data if NAME1 >  NAME2.
	ge: Adds data if NAME1 >= NAME2.
	lt: Adds data if NAME1 <  NAME2.
	le: Adds data if NAME1 <= NAME2.

# else {data}
Adds data if last #if did not add data.

# else if [not] [exists|false|true] [local] : ARG ... {data}
Adds data if last #if did not add data and #if is true.

# else if cmp : NAME1 : OP : NAME2 : TYPE {data}
Adds data if last #if did not add data and #ifcmp is true.

Implementation NOTE: #else can be used anywhere, including consecutively. It
Works based on the last #if command. If multiple #else commands are used, every
other one is added, effectively reversing the last #if result each time.

Flow control commands:

# break;
Terminates the current (innermost) loop.

# continue;
Skips the rest of the current iteration of the current loop.

# do : value {data}
Works like while, except that value is evaluated after data and therefore will
always add data at least once.

# for : name : start : stop : step {data}
Is equivalent to:
	[int] name = start;
	#while : <cmpis>(name, lt, stop)
		{data; name += step;}
	#delete : name;
step defaults to 1. If a type is used with the command, the iterator will be
that type.

If #continue is used in a for loop, the iterator is still incremented.

# return : value;
Returns a value from a function. The return type is determined by the function.
If not called from a function, the result is undefined.

# while : value {data}
Command loops until value evaluates to false, adding data each loop. If value
is false to begin with, data will be added 0 times.

Remaining commands:

# change type : TYPE : VALUE;
Changes this object's type. If the new type is a value, it is set to VALUE.

# compound : TYPE;
See Compound Objects. Also, this command IS available globally. Using this
command will prevent automatic compounding of the object it's used on.

# debug : MESSAGE;
Prints message out stderr if the --debug option was used. MESSAGE is parsed as
a string.

# delete : NAME;
Removes NAME from this object. Does NOT prevent output.

# delete volatile;
Removes all names that are volatile. Currently, that means starting with _ but
not __.

# delete _;
Removes all names that start with _ (but not __) from this object.

# error : MESSAGE;
Prints message out stderr and counts against --error-limit. MESSAGE is parsed
as a string.

# info : MESSAGE;
Prints message out stderr. MESSAGE is parsed as a string.

# script : FILENAME {data}
Outputs data to the file indicated by FILENAME. Multiple #script commands are
cumulative.

# script-acs {data}
Equivalent to #script:"SCRIPTS"{data}. If the --do-extensions option is
enabled, filename is "SCRIPTS.acs" instead. The --script-acs option can change
the name. For these reasons, you should never mix #script-acs and
#script:"SCRIPTS".

# script-fraggle {data}
Adds a script using the name of the map. Name is set using the --map-name
option. It defaults to the last directory in the output directory. For example,
if you output to maps/E1M1, map name would default to E1M1. The command name
comes from FraggleScript.

In all script cases, excess arguments are parsed as strings and added before
data. Non-string identifiers can be used as-is, but to perform math you must
type cast (see Type Casting).

It is legal to say #script:FILENAME;. This will cause the specified file to be
created, but with nothing in it (unless some is added later in the code).
Useful in Hexen binary output mode to create an empty SCRIPTS or BEHAVIOR file
to ensure the map is read as Hexen format.

Implementation NOTE: While we all yearn for the day when computers will do what
we mean rather than what we say, we aren't there yet (and computers never will,
so I wouldn't suggest waiting). There are some caveats to using the script
command, especially the data part. See Embedding Scripts.

Example:
	[LINEDEF] A_Line
	{
		id = 1;
		# script-acs : "$#define A_Line_id " : id : "$\n";
	}

# warn : MESSAGE
Prints message out stderr and counts against --warn-limit. MESSAGE is parsed as
a string.



Compound Objects:
	[type] name:base {data}

Compound objects are a way to easily recreate common map components such as
doors and poly objects. It removes the need to remember the exact details of
these things. It also makes it much more obvious in your map source what you're
doing.

Creating a custom compound object type is easy:
	# typedef : TYPE : compound object;
	# define  : TYPE {DATA}
	[TYPE] NAME {DATA2}

In this simple example, DATA2 is added to NAME followed by DATA.

Normally, DATA is added to NAME after DATA2, as in above. However, there are
other ways to use compound objects. If the new compound object is declared
without any data, then DATA is not automatically added. Additionally, if there
is an explicit #compound in DATA2, no automatic compounding takes place.

When using the #compound command, it is possible to use a different compound
object type than the current type by passing it as arg1. For those with C/C++
experience, this makes it sound a lot like macros, which it is.

Extending compound objects work just like normal objects, with all of the same
syntax options (see Basic Syntax).



Embedding Scripts:
	#script:FILENAME:arg {data}

For an overview of the basic syntax, see Commands.

Embedding scripts can be a very powerful tool, especially when combined with an
external program that automates compiling such as make. For instance, you could
have each map source define its own MAPINFO element and then combine them into
the wad's MAPINFO. Or you could have DECORATE actors that are unique to a map
be in the map its unique to. It also allows you to add data from your map into
those files. You could even, strangely enough, embed a script into the map's
source, such as FraggleScript or ACS (which isn't really a script at all, since
it has to be compiled, but that's probably just me). This means you could have
a script that is activated by a line be defined near that line.

For the most part, all of this fits and works nicely witin a {data} block.
However, I draw your attention to an implied element to the above paragraph:
#script is universal. It doesn't know anything about the script you're
embedding. So how does it properly extract the script? Currently, it does it by
brace counting. This seems all well and good, of course, since almost anything
You might put in a script would only use balanced braces (a '}' for every '{').
But, what about this:
	printf("Yay! :}\n");
Well, because it's quoted, it's okay. Single quotes are also detected:
	printf("A character: %c\n", '}'); // OK
Comments, too:
	printf("stuff\n"); // a brace for no reason: }
	/* brace time }}} */

What happens if a rogue brace is incorrectly counted (or uncounted)? Depends.

If it's an extra '{' (or missing '}'), then you should get an unexpected end of
data error. This may sound harsh, but the alternative is to silently dump the
entire rest of the map into that script file.

If it's an extra '}' (or missing '{'), then you may get a syntax error. Or not,
it depends on the nature of the data that "leaked out". At the very least, you
will get an unbalanced braces error at the end of the file.

The good news? It's only a problem with certain languages you might try to
embed. Well formed ACS is fine. DECORATE and MAPINFO lumps are also safe. I do
not, in fact, know of any Doom text lump that allows unbalanced braces from any
port.

Using :arg elements is somewhat safer, in so far as you know you have to quote
any basic syntax, but it's a little awkward. Imagine the following (converted
from a ZDoom Wiki example):
	#script-acs:'$script 51 (void)
	{
		PrintBold(s:"Security issues the standard kit.\\n");
		GiveInventory("Pistol", 1);
		GiveInventory("Clip", 50);
		GiveInventory("Armor", 75);
	}'

In closing, embedding scripts is powerful, and 99.999% of the time requires no
further consideration. Even if that .001% takes up 90% of the documentation on
the subject...



Forward Compatibility and Miscellaneous Gotchas:

Sometimes a feature is added that could break existing, but obscure, map
sources. Other times there are command line options that can cause problems.
This section gives suggestions on how to avoid most of that. (Of course, it's
always possible that even code following these rules could be broken if it was
for a really good reason, but still).

Avoid non-alphanumerics in object names. It is not expressly an error to do so,
but I consider all non-alphanumerics up for grabs as far as new features are
concerned.

Don't assume that unquoted source will be of a particular case.

Don't assume you know what #script-* will use for a filename. Specifically,
don't use #script-acs and #script:"SCRIPTS", because what the former means
depends on various command line options (same for all #script-*).

Don't use #precision unless you absolutely must have a certain precision for
the map to compile correctly.



Functions:
	<function>(arg,...)

Most functions have required arguments.

The type before the name is what context the function is available in. For
example, when evaluating an int, the [int] version of <random> is called.

Which version is called can be changed by type casting (see Type Casting).

Unless stated otherwise, each function has a short and long version.

[bool] <cmpfs>([short float] val1, op, [short float] val2)
[bool] <cmpf>([float] val1, op, [float] val2)
[bool] <cmpfl>([long float] val1, op, [long float] val2)
[bool] <cmpis>([short int] val1, op, [short int] val2)
[bool] <cmpi>([int] val1, op, [int] val2)
[bool] <cmpil>([long int] val1, op, [long int] val2)
[bool] <cmps>([string] val1, op, [string] val2)
[bool] <cmp>([long float] val1, op, [long float] val2)

	Compares two values, returns true if (val1 op val2).

	op must be one of the following:
		eq (==) (equal to)
		ne (!=) (not equal to)
		gt (>)  (greater than)
		ge (>=) (greater than or equal to)
		lt (<)  (less than)
		le (<=) (less than or equal to)

	Any other op is an error. op is taken literally.

[float] <cmp>([float] val1, [float] val2)

	Returns -1 if val1 < val2, 0 if val1 == val2, or 1 if val1 > val2.

[float] <distance>([VERTEX] point1, [VERTEX] point2)
[float] <distance>([float] x1, [float] y1, [float] x2, [float] y2)

	Returns the distance between two points.

[float] <facing>([VERTEX] point1, [VERTEX] point2)
[float] <facing>([float] x1, [float] y1, [float] x2, [float] y2)

	Returns the facing, in degrees, from point 1 to point 2.

	The name angle also works.

[float] <hypot>([float] side1, [float] side2)

	Returns [sqrt]((side1*side1)+(side2*side2)).

	hypot is short for hypotenuse.

[float] <random>([float] min, [float] max)

	Returns a random number between min and max.

[int] <cmp>([int] val1, [int] val2)

	Returns -1 if val1 < val2, 0 if val1 == val2, or 1 if val1 > val2.

[int] <distance>([VERTEX] point1, [VERTEX] point2)
[int] <distance>([int] x1, [int] y1, [int] x2, [int] y2)

	Returns the distance between two points.

[int] <hypot>([int] side1, [int] side2)

	Returns [sqrt]((side1*side1)+(side2*side2)).

	hypot is short for hypotenuse.

[int] <random>([int] min, [int] max)

	Returns a random number between min and max.

[string] <cmp>([string] val1, [string] val2)

	Returns "-1" if val1 < val2, "0" if val1 == val2, or "1" if val1 >
	val2.



Numeric Literals:

Which type a literal is evaluated as (float or int) depends on the current type
context. Usually, this means the type being assigned.
	[int]   i = 3; // int literal
	[float] f = 3; // float literal

Numbers can be expressed in hex (0x), octal (0o, 0), decimal (0d), or binary
(0b). If no base is given explicitly, it is assumed to be decimal.

Floats can be prefixed and allow exponent. Ints also support an exponent.

When using exponents with a base other than 10, remember that it works by
multiplying by 10 in the specified base (0x10e2 = 0x1000). This can be useful
for flags (0b1eN where N is the flag number).

Implementation NOTE: Exponents are only detected properly if they match the
regex [0-9][eE][+-][0-9] (if using an unsigned exponent, this does not apply).
Just a warning to be careful with float literals given in hex with an exponent.
I'm sure someone, someday, will do that. Additionally, if using hex base (or
any base greater than 14), exponent must include the sign to be converted
correctly.



Quoting:

Quoting is very simple, and serves the following purposes:
	1. Preserves whitespace and case.
	2. Negates the meaning of the characters of basic syntax (see Basic
	Syntax).
	3. Enables escape sequences (\r, \n, \t, \', \", \\).

Normally, all whitespace is discarded and letters may be forced to lower case,
if the --no-case-sensitive option is used.

Quoting DOES NOT negate the meaning of secondary syntax, such as functions. Nor
does it indicate a string literal (see String Literals).

For example:
	[string] A_String1 = <a_string_func>(arg1, arg2, arg3);
	[string] A_String2 = "<a_string_func>(arg1, arg2, arg3)";
will both result in a function call. The only way to prevent this is via string
literals (see String Literals).

For those familiar with shell scripting, these rules are very similiar. The
following, for example, are all equivalent:
	[string] A_String3 = $ some" text; "and then some;
	[string] A_String4 = $ "some text; andthensome";
	[string] A_String5 = $ some" "text"; "and then some;

(Actually, they aren't because unquoted data may or may not have its case
changed to upper or lower case, depending on command line options.)

Like shell, single quotes (') can be used to quote, but they operate exactly
the same as double quotes (").



String Literals:
	$string

string can be quoted to preserve whitespace (see Quoting).

Unlike most languages, quoting is totally unrelated to string literals. The
only thing that makes a string literal is being prefixed with a '$' character.

It is no longer possible to use implicit literal strings:
	[string] A_String1 = "$A_STRING";
	[string] A_String3 = A_String1 + A_String2; // ERROR

Any element in a string expression must be an explicit literal, or a variable.
This can be changed back to the older behaviour with the --no-strict-strings
option.



Type Casting: (or Evaluate As):
	(type)expression

Causes expression to be evaluated as (type). For example:
	[int] area = (float)([pi] * radius * radius);

Without the "cast", [pi] would evaluate as 3. Actually, it's not available to
[int] evaluation for this reason, so it's an error. So, another use for
casting is access to functions.

The name comes from the C syntax which looks similar, and has similar usage.
With the exception that it's backwards. Consider the following statements which
are basically the same:
	[int] i = (float)((3/2)*3); // DH-dlc
	int   i = (float)((3/2)*3); // C
In C, (3/2) yields 1, which then multiplied by 3, then that whole expression
yields 3.0, and then back to 3. In DH-dlc, (3/2) yields 1.5, which is then
multiplied by 3, and the expression yields 4 because it's in an int context.

The point is that in (type)(expression), (expression) is evaluated AS (type),
not converted to (type) after.

Anyone with programming experience knows that the preceding example applies to
all languages with type casting. So why call this type casting? Two reasons: 1,
it looks like type casting. And 2, it's still about changing type, just the
evaluation type instead of the resulting type.

Also, I know that in the C example the reason they aren't evaluated as floats
is because they would need to have .0 appended. In DH-dlc, however, a numeric
constant is an int or float based on the type it's being evaluated as.



Types:

There are three types of types, as noted in the description of the #typedefnew
command, value, object, and compoundobject.

value: does not get output, does not get compounded
object: does get output, does not get compounded
compoundobject: does not get output, does get compounded

Internally, certain value types are treated specially. In fact, there are only
two kinds of types internally, value and object.

value: stores a single value
object: stores a key-value map of other objects/values

These are the internally recognized value types:
	bool: stores true or false
	int: stores an integer
	float: stores a rational
	string: stores text

The numeric types also have short and long variants. Short numbers have a fixed
size that is decided when the program is compiled (currently, they are basic C
types ssize_t and double). Normal numbers have a fixed size that is based on
the --precision option. Long numbers are of unlimited size and precision.

If GMPLib is not used, then long numbers are simply the largest available type,
normal floats are long floats, and normal ints are still bound by precision,
but are at most long.



Unary Functions:

Unary functions are very simple, and come in two simple flavors, modifier and
constant:
	[modifier] expression
	[constant]

Note that constants are not actually constant, their value may be different
between invocations. The name mostly comes from the original unary "constant",
[pi] (pi is calculated to precision, and recalculated if precision changes),
which is conceptually constant.

They can be used in an expression normally:
	[float] hypot = [sqrt]((sideA * sideA) + (sideB * sideB));
	[float] area = [pi] * radius * radius;
	[int]   dice = [random]6 + 2; // 2-8
	[int]   dice2 = [random](6 + 2); // 0-8

Notice that the modifier affects the expression immediately to its right.

Unknown unary functions are an error.
In addition, modifer and constants are discreet lists, using
([constant] expression) or ([modifer]) is an error.

These are the known unary functions sorted by type (functions are available to
short and long types as well):

[modifier]:
[bool]:
[exists]
	Returns true if expression is a declared name.

[not]
	Inverts the boolean value of expression.

[float]:
[abs]
	Returns the absolute value of expression.

[acos]
[asin]
[atan]
[cos]
[sin]
[tan]
	These have the normal trigonomic meaning.
	Implementation NOTE: During calculation, the number is stored in a
	short float, which may result in a negligible loss of precisision.

[byte2deg]
[byte2rad]
[deg2byte]
[deg2rad]
[rad2byte]
[rad2deg]
	Converts between degrees, radians, and byte angles.
	The result is not clamped (see below).

[byteangle]
[degrees]
[radians]
	Clamps expression to be within normal bounds of the given unit of
	measurement. The result is mathematically equivalent to the original.
	For example (degrees):
		-90  -> 270
		400  -> 40
		-180 -> 180
		360  -> 0
		1000 -> 280
	byteangle is (0.0 <= expression < 256.0).
	degrees is (0.0 <= expression < 360.0).
	radians is (0.0 <= expression < [pi]*2).

[random]
	Generates a random number between 0.0 and expression.

[round]
	Rounds expression to an int, rounding half-up. For example:
		1.10 -> 1
		1.90 -> 2
		1.50 -> 2
		1.49 -> 1
	Useful for reducing loss of precision to at most 0.5.

[sqrt]
	Calculates square root of expression. The precision of the result is
	bound to the current precision.

[int]:
[abs]
	Returns the absolute value of expression.

[byte2deg]
[byte2rad]
[deg2byte]
[deg2rad]
[rad2byte]
[rad2deg]
	See float for description.

[byteangle]
[degrees]
[radians]
	See float for description.

[random]
	Generates a random number between 0 and expression.

[sqrt]
	Calculates square root of expression.

[string]:
[quote]
	Quotes a string, including inserting escape sequences. Used for pushing
	strings into an embedded script safely.

[constant]:
[bool]:
[random]
	Generates true or false randomly.

[float]:
[pi]
	pi, calculated to the set precision.

[random]
	Generates a random number between 0.0 and 1.0.

[int]: none

[string]:
[mapname]
	Returns the map's name which is either set by the --map-name option or
	generated based on the output directory's name.



