// "Standard" definitions.

# typedef new :       bool   : value;
# typedef new : short float  : value;
# typedef new :       float  : value;
# typedef new : long  float  : value;
# typedef new : short int    : value;
# typedef new :       int    : value;
# typedef new : long  int    : value;
# typedef new :       string : value;

# typedef new : LINEDEF : object;
# typedef new : SECTOR  : object;
# typedef new : SIDEDEF : object;
# typedef new : THING   : object;
# typedef new : VERTEX  : object;



# default type : namespace : string;



# default type : id : int : LINEDEF;

# default type : v1 : VERTEX : LINEDEF;
# default type : v2 : VERTEX : LINEDEF;

# default type : blocking      : bool : LINEDEF;
# default type : blockmonsters : bool : LINEDEF;
# default type : twosided      : bool : LINEDEF;
# default type : dontpegtop    : bool : LINEDEF;
# default type : dontpegbottom : bool : LINEDEF;
# default type : secret        : bool : LINEDEF;
# default type : blocksound    : bool : LINEDEF;
# default type : dontdraw      : bool : LINEDEF;
# default type : mapped        : bool : LINEDEF;

# default type : passuse : bool : LINEDEF;

# default type : translucent   : bool : LINEDEF;
# default type : jumpover      : bool : LINEDEF;
# default type : blockfloaters : bool : LINEDEF;

# default type : playercross   : bool : LINEDEF;
# default type : playeruse     : bool : LINEDEF;
# default type : monstercross  : bool : LINEDEF;
# default type : monsteruse    : bool : LINEDEF;
# default type : impact        : bool : LINEDEF;
# default type : playerpush    : bool : LINEDEF;
# default type : monsterpush   : bool : LINEDEF;
# default type : missilecross  : bool : LINEDEF;
# default type : repeatspecial : bool : LINEDEF;

# default type : special : int : LINEDEF;
# default type : arg0    : int : LINEDEF;
# default type : arg1    : int : LINEDEF;
# default type : arg2    : int : LINEDEF;
# default type : arg3    : int : LINEDEF;
# default type : arg4    : int : LINEDEF;

# default type : sidefront : SIDEDEF : LINEDEF;
# default type : sideback  : SIDEDEF : LINEDEF;

# default type : comment : string : LINEDEF;



# default type : heightfloor   : int : SECTOR;
# default type : heightceiling : int : SECTOR;

# default type : texturefloor   : string : SECTOR;
# default type : textureceiling : string : SECTOR;

# default type : lightlevel : int : SECTOR;

# default type : special : int : SECTOR;
# default type : id      : int : SECTOR;

# default type : comment : string : SECTOR;



# default type : offsetx : int : SIDEDEF;
# default type : offsety : int : SIDEDEF;

# default type : texturetop    : string : SIDEDEF;
# default type : texturebottom : string : SIDEDEF;
# default type : texturemiddle : string : SIDEDEF;

# default type : sector : SECTOR : SIDEDEF;

# default type : comment : string : SIDEDEF;



# default type : id : int : THING;

# default type : x : float : THING;
# default type : y : float : THING;

# default type : height : float : THING;

# default type : angle : int : THING;

# default type : type : int : THING;

# default type : skill1 : bool : THING;
# default type : skill2 : bool : THING;
# default type : skill3 : bool : THING;
# default type : skill4 : bool : THING;
# default type : skill5 : bool : THING;
# default type : ambush : bool : THING;
# default type : single : bool : THING;
# default type : dm     : bool : THING;
# default type : coop   : bool : THING;

# default type : friend : bool : THING;

# default type : dormant : bool : THING;
# default type : class1  : bool : THING;
# default type : class2  : bool : THING;
# default type : class3  : bool : THING;

# default type : standing    : bool : THING;
# default type : strifeally  : bool : THING;
# default type : translucent : bool : THING;
# default type : invisible   : bool : THING;

# default type : special : int : THING;
# default type : arg0    : int : THING;
# default type : arg1    : int : THING;
# default type : arg2    : int : THING;
# default type : arg3    : int : THING;
# default type : arg4    : int : THING;

# default type : comment : string : THING;



# default type : x : float : VERTEX;
# default type : y : float : VERTEX;



