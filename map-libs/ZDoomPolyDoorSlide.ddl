// ZDoomPolyDoorSlide compound object

# include : "VertexCenter.ddl";
# include : "VertexOffset.ddl";

# include : "lib-udmf-zdoom.ddl";



# if not exists : __POLY_COUNT  {[int]   __POLY_COUNT  = 0;}
# if not exists : __POLY_OFFSET {[float] __POLY_OFFSET = 0x3FFF;}

# typedef new : ZDoomPolyDoorSlide : compound object;

# default type : v1 : VERTEX : ZDoomPolyDoorSlide;
# default type : v2 : VERTEX : ZDoomPolyDoorSlide;
# default type : v3 : VERTEX : ZDoomPolyDoorSlide;
# default type : v4 : VERTEX : ZDoomPolyDoorSlide;

# default type : side1 : SIDEDEF : ZDoomPolyDoorSlide;
# default type : side2 : SIDEDEF : ZDoomPolyDoorSlide;

# default type :                    delay : int : ZDoomPolyDoorSlide;
# default type : ZDoomPolyDoorSlide_delay : int;
# default type :                    speed : int : ZDoomPolyDoorSlide;
# default type : ZDoomPolyDoorSlide_speed : int;

# default type :                    texturedoor : string : ZDoomPolyDoorSlide;
# default type : ZDoomPolyDoorSlide_texturedoor : string;
# default type :                    texturetrak : string : ZDoomPolyDoorSlide;
# default type : ZDoomPolyDoorSlide_texturetrak : string;



/*

                              side2
                3- - - - - - - - - - - - - - - -4
                |                               |
                |                               |
                |                               |
3---------------4- - - - - - - -2- - - - - - - -1---------------2
|                               |                               |
|    pocket2                 divider                 pocket1    |
|                               |                               |
2---------------1- - - - - - - -1- - - - - - - -4---------------3
                |                               |
                |                               |
                |                               |
                2- - - - - - - - - - - - - - - -1
                              side1

pocket1 = 4v, 3l
pocket2 = 4v, 3l
divider = 2v, 5l

base = 4v, 0l, 2s
rest = 0v, 6l, 2s

4-----------------5-----------------6
|1---------------4|1---------------4|
||               |||               ||
||     door2     |||     door1     ||
||               |||               ||
|2---------------3|2---------------3|
3-----------------2-----------------1

door1 = 4v, 4l, 1s
door2 = 4v, 4l, 1s

box = 6v, 7l

The whole construct can be in any orientation, although because
the angle of movement is a byte angle, certain angles will
most likely cause bleeding.

*/



# define : ZDoomPolyDoorSlide
{
	# if not exists local : v1 {#error:"$v1 required";}
	# if not exists local : v2 {#error:"$v2 required";}
	# if not exists local : v3 {#error:"$v3 required";}
	# if not exists local : v4 {#error:"$v4 required";}

	# if not exists local : side1 {#error:"$side1 required";}
	# if not exists local : side2 {#error:"$side2 required";}



	[float] _angle_0   = <facing>(v2, v1);
	[float] _angle_45  = [degrees](_angle_0 + 45);
	[float] _angle_90  = [degrees](_angle_0 + 90);
	[float] _angle_135 = [degrees](_angle_0 + 135);
	[float] _angle_180 = [degrees](_angle_0 + 180);
	[float] _angle_225 = [degrees](_angle_0 + 225);
	[float] _angle_270 = [degrees](_angle_0 + 270);
	[float] _angle_315 = [degrees](_angle_0 + 315);

	[float] _angleUp    = _angle_90;
	[float] _angleDown  = _angle_270;
	[float] _angleLeft  = _angle_180;
	[float] _angleRight = _angle_0;

	[float] _boxOffset = 1;

	[float] _doorDepth  = <distance>(v1, v4) / 3;
	[float] _doorHeight = 128;
	[float] _doorWidth  = <distance>(v1, v2) / 2;

	# if exists : __DOOR_SIZE {_doorDepth = __DOOR_SIZE;}

	[float] _flangeSize = (<distance>(v1, v4) - _doorDepth) / 2;

	# if cmp : _flangeSize : lt : 0 : float
	{
		# debug : ("$Attempted to make a poly-door with a flange size of ") + _flangeSize;

		_doorDepth  = <distance>(v1, v4);
		_flangeSize = 0;

		# debug : ("$Resetting door/flange size to") + _doorDepth + ("$/") + _flangeSize;
	}

	[int] _polyNum1 = __POLY_COUNT;
	[int] _polyNum2 = __POLY_COUNT+1;

	global.__POLY_COUNT += 2;

	[int] _special       = 8;
	[int] _special_delay = 140;
	[int] _special_speed = 32;

	# if exists : __DOOR_DELAY {_special_delay = __DOOR_DELAY;}
	# if exists : __DOOR_SPEED {_special_speed = __DOOR_SPEED;}

	# if exists : ZDoomPolyDoorSlide_delay {_special_delay = ZDoomPolyDoorSlide_delay;}
	# if exists : ZDoomPolyDoorSlide_speed {_special_speed = ZDoomPolyDoorSlide_speed;}

	# if exists local : delay {_special_delay = delay;}
	# if exists local : speed {_special_speed = speed;}

	[string] _texture_door = "$DOOR2_4";
	[string] _texture_trak = "$DOORTRAK";

	# if exists : __DOOR_TEXTURE {_texture_door = __DOOR_TEXTURE;}
	# if exists : __TRAK_TEXTURE {_texture_trak = __TRAK_TEXTURE;}

	# if exists : ZDoomPolyDoorSlide_texturedoor {_texture_door = ZDoomPolyDoorSlide_texturedoor;}
	# if exists : ZDoomPolyDoorSlide_texturetrak {_texture_trak = ZDoomPolyDoorSlide_texturetrak;}

	# if exists local : texturedoor {_texture_door = texturedoor;}
	# if exists local : texturetrak {_texture_trak = texturetrak;}

	[int] _typeAnchor = 9300;
	[int] _typeStart  = 9301;

	# if not exists local : pocket1_v1
		{pocket1_v1 = VertexOffset;}
	# else  {pocket1_v1.#changetype:VertexOffset;}

		pocket1_v1.b = v4;
		pocket1_v1.a = _angleDown;
		pocket1_v1.d = _flangeSize;

		pocket1_v1.#compound;

	# if not exists local : pocket1_v2
		{pocket1_v2 = VertexOffset;}
	# else  {pocket1_v2.#changetype:VertexOffset;}

		pocket1_v2.b = pocket1_v1;
		pocket1_v2.a = _angleRight;
		pocket1_v2.d = _doorWidth;

		pocket1_v2.#compound;

	# if not exists local : pocket1_v3
		{pocket1_v3 = VertexOffset;}
	# else  {pocket1_v3.#changetype:VertexOffset;}

		pocket1_v3.b = pocket1_v2;
		pocket1_v3.a = _angleDown;
		pocket1_v3.d = _doorDepth;

		pocket1_v3.#compound;

	# if not exists local : pocket1_v4
		{pocket1_v4 = VertexOffset;}
	# else  {pocket1_v4.#changetype:VertexOffset;}

		pocket1_v4.b = pocket1_v3;
		pocket1_v4.a = _angleLeft;
		pocket1_v4.d = _doorWidth;

		pocket1_v4.#compound;

	# if not exists local : pocket2_v1
		{pocket2_v1 = VertexOffset;}
	# else  {pocket2_v1.#changetype:VertexOffset;}

		pocket2_v1.b = v2;
		pocket2_v1.a = _angleUp;
		pocket2_v1.d = _flangeSize;

		pocket2_v1.#compound;

	# if not exists local : pocket2_v2
		{pocket2_v2 = VertexOffset;}
	# else  {pocket2_v2.#changetype:VertexOffset;}

		pocket2_v2.b = pocket2_v1;
		pocket2_v2.a = _angleLeft;
		pocket2_v2.d = _doorWidth;

		pocket2_v2.#compound;

	# if not exists local : pocket2_v3
		{pocket2_v3 = VertexOffset;}
	# else  {pocket2_v3.#changetype:VertexOffset;}

		pocket2_v3.b = pocket2_v2;
		pocket2_v3.a = _angleUp;
		pocket2_v3.d = _doorDepth;

		pocket2_v3.#compound;

	# if not exists local : pocket2_v4
		{pocket2_v4 = VertexOffset;}
	# else  {pocket2_v4.#changetype:VertexOffset;}

		pocket2_v4.b = pocket2_v3;
		pocket2_v4.a = _angleRight;
		pocket2_v4.d = _doorWidth;

		pocket2_v4.#compound;

	# if not exists local : divider_v1
		{divider_v1 = VertexCenter;}
	# else  {divider_v1.#changetype:VertexCenter;}

		divider_v1.p1 = pocket1_v4;
		divider_v1.p2 = pocket2_v1;

		divider_v1.#compound;

	# if not exists local : divider_v2
		{divider_v2 = VertexCenter;}
	# else  {divider_v2.#changetype:VertexCenter;}

		divider_v2.p1 = pocket2_v4;
		divider_v2.p2 = pocket1_v1;

		divider_v2.#compound;

	# if not exists local : door1_v1
		{door1_v1 = VertexOffset;}
	# else	{door1_v1.#changetype:VertexOffset;}

		door1_v1.b = divider_v2;
		door1_v1.a = _angleRight;
		door1_v1.d = _boxOffset;

		door1_v1.#compound;

		door1_v1.x += __POLY_OFFSET;
		door1_v1.y += __POLY_OFFSET;

	# if not exists local : door1_v2
		{door1_v2 = VertexOffset;}
	# else	{door1_v2.#changetype:VertexOffset;}

		door1_v2.b = divider_v1;
		door1_v2.a = _angleRight;
		door1_v2.d = _boxOffset;

		door1_v2.#compound;

		door1_v2.x += __POLY_OFFSET;
		door1_v2.y += __POLY_OFFSET;

	# if not exists local : door1_v3
		{door1_v3 = VertexOffset;}
	# else	{door1_v3.#changetype:VertexOffset;}

		door1_v3.b = pocket1_v4;
		door1_v3.a = _angleRight;
		door1_v3.d = _boxOffset;

		door1_v3.#compound;

		door1_v3.x += __POLY_OFFSET;
		door1_v3.y += __POLY_OFFSET;

	# if not exists local : door1_v4
		{door1_v4 = VertexOffset;}
	# else	{door1_v4.#changetype:VertexOffset;}

		door1_v4.b = pocket1_v1;
		door1_v4.a = _angleRight;
		door1_v4.d = _boxOffset;

		door1_v4.#compound;

		door1_v4.x += __POLY_OFFSET;
		door1_v4.y += __POLY_OFFSET;

	# if not exists local : door2_v1
		{door2_v1 = VertexOffset;}
	# else	{door2_v1.#changetype:VertexOffset;}

		door2_v1.b = pocket2_v4;
		door2_v1.a = _angleLeft;
		door2_v1.d = _boxOffset;

		door2_v1.#compound;

		door2_v1.x += __POLY_OFFSET;
		door2_v1.y += __POLY_OFFSET;

	# if not exists local : door2_v2
		{door2_v2 = VertexOffset;}
	# else	{door2_v2.#changetype:VertexOffset;}

		door2_v2.b = pocket2_v1;
		door2_v2.a = _angleLeft;
		door2_v2.d = _boxOffset;

		door2_v2.#compound;

		door2_v2.x += __POLY_OFFSET;
		door2_v2.y += __POLY_OFFSET;

	# if not exists local : door2_v3
		{door2_v3 = VertexOffset;}
	# else	{door2_v3.#changetype:VertexOffset;}

		door2_v3.b = divider_v1;
		door2_v3.a = _angleLeft;
		door2_v3.d = _boxOffset;

		door2_v3.#compound;

		door2_v3.x += __POLY_OFFSET;
		door2_v3.y += __POLY_OFFSET;

	# if not exists local : door2_v4
		{door2_v4 = VertexOffset;}
	# else	{door2_v4.#changetype:VertexOffset;}

		door2_v4.b = divider_v2;
		door2_v4.a = _angleLeft;
		door2_v4.d = _boxOffset;

		door2_v4.#compound;

		door2_v4.x += __POLY_OFFSET;
		door2_v4.y += __POLY_OFFSET;

	# if not exists local : box_v1
		{box_v1 = VertexOffset;}
	# else	{box_v1.#changetype:VertexOffset;}

		box_v1.b = door1_v3;
		box_v1.a = _angle_315;	
		box_v1.d = _boxOffset;

		box_v1.#compound;

	# if not exists local : box_v3
		{box_v3 = VertexOffset;}
	# else	{box_v3.#changetype:VertexOffset;}

		box_v3.b = door2_v2;
		box_v3.a = _angle_225;	
		box_v3.d = _boxOffset;

		box_v3.#compound;

	# if not exists local : box_v2
		{box_v2 = VertexCenter;}
	# else	{box_v2.#changetype:VertexCenter;}

		box_v2.p1 = box_v1;
		box_v2.p2 = box_v3;

		box_v2.#compound;

	# if not exists local : box_v4
		{box_v4 = VertexOffset;}
	# else	{box_v4.#changetype:VertexOffset;}

		box_v4.b = door2_v1;
		box_v4.a = _angle_135;	
		box_v4.d = _boxOffset;

		box_v4.#compound;

	# if not exists local : box_v6
		{box_v6 = VertexOffset;}
	# else	{box_v6.#changetype:VertexOffset;}

		box_v6.b = door1_v4;
		box_v6.a = _angle_45;	
		box_v6.d = _boxOffset;

		box_v6.#compound;

	# if not exists local : box_v5
		{box_v5 = VertexCenter;}
	# else	{box_v5.#changetype:VertexCenter;}

		box_v5.p1 = box_v4;
		box_v5.p2 = box_v6;

		box_v5.#compound;



	// Calling it "sector" is traditional, but bug-prone.
	# if not exists local : door_sector
		{door_sector = SECTOR;}

		door_sector.texturefloor   = _texture_trak;
		door_sector.textureceiling = _texture_trak;

		door_sector.# if cmp : door_sector.texturefloor : eq : "$DOORTRAK" : string
		{
			rotationfloor   = _angle_90;
			rotationceiling = _angle_90;
		}

		door_sector.heightceiling = _doorHeight;

	# if not exists local : side_clear
		{side_clear = SIDEDEF;}

		side_clear.sector = door_sector;

	# if not exists local : side_door1
		{side_door1 = SIDEDEF;}

		side_door1.sector = door_sector;

		side_door1.texturemiddle = _texture_door;

	# if not exists local : side_door2
		{side_door2 = SIDEDEF;}

		side_door2.sector = door_sector;

		side_door2.texturemiddle = _texture_door;

		side_door2.offsetx = [](float)[round]_doorWidth;

	# if not exists local : side_trak
		{side_trak = SIDEDEF;}

		side_trak.sector = door_sector;

		side_trak.texturemiddle = _texture_trak;



	# if not exists local : line1
		{line1 = LINEDEF;}

		line1.v1 = v1;
		line1.v2 = v2;

		line1.twosided = true;

		line1.sidefront = side_clear;
		line1.sideback  = side1;

	# if not exists local : line2
		{line2 = LINEDEF;}

		line2.v1 = v2;
		line2.v2 = pocket2_v1;

		line2.sidefront = side_trak;

	# if not exists local : line3
		{line3 = LINEDEF;}

		line3.v1 = pocket2_v4;
		line3.v2 = v3;

		line3.sidefront = side_trak;

	# if not exists local : line4
		{line4 = LINEDEF;}

		line4.v1 = v3;
		line4.v2 = v4;

		line4.twosided = true;

		line4.sidefront = side_clear;
		line4.sideback  = side2;

	# if not exists local : line5
		{line5 = LINEDEF;}

		line5.v1 = v4;
		line5.v2 = pocket1_v1;

		line5.sidefront = side_trak;

	# if not exists local : line6
		{line6 = LINEDEF;}

		line6.v1 = pocket1_v4;
		line6.v2 = v1;

		line6.sidefront = side_trak;

	# if not exists local : pocket1_line1
		{pocket1_line1 = LINEDEF;}

		pocket1_line1.v1 = pocket1_v1;
		pocket1_line1.v2 = pocket1_v2;

		pocket1_line1.sidefront = side_trak;

	# if not exists local : pocket1_line2
		{pocket1_line2 = LINEDEF;}

		pocket1_line2.v1 = pocket1_v2;
		pocket1_line2.v2 = pocket1_v3;

		pocket1_line2.sidefront = side_trak;

	# if not exists local : pocket1_line3
		{pocket1_line3 = LINEDEF;}

		pocket1_line3.v1 = pocket1_v3;
		pocket1_line3.v2 = pocket1_v4;

		pocket1_line3.sidefront = side_trak;

	# if not exists local : pocket2_line1
		{pocket2_line1 = LINEDEF;}

		pocket2_line1.v1 = pocket2_v1;
		pocket2_line1.v2 = pocket2_v2;

		pocket2_line1.sidefront = side_trak;

	# if not exists local : pocket2_line2
		{pocket2_line2 = LINEDEF;}

		pocket2_line2.v1 = pocket2_v2;
		pocket2_line2.v2 = pocket2_v3;

		pocket2_line2.sidefront = side_trak;

	# if not exists local : pocket2_line3
		{pocket2_line3 = LINEDEF;}

		pocket2_line3.v1 = pocket2_v3;
		pocket2_line3.v2 = pocket2_v4;

		pocket2_line3.sidefront = side_trak;

	# if not exists local : divider_line1
		{divider_line1 = LINEDEF;}

		divider_line1.v1 = divider_v1;
		divider_line1.v2 = divider_v2;

		divider_line1.twosided = true;

		divider_line1.sidefront = side_clear;
		divider_line1.sideback  = side_clear;

	# if not exists local : divider_line2
		{divider_line2 = LINEDEF;}

		divider_line2.v1 = divider_v1;
		divider_line2.v2 = pocket2_v1;

		divider_line2.twosided = true;

		divider_line2.sidefront = side_clear;
		divider_line2.sideback  = side_clear;

	# if not exists local : divider_line3
		{divider_line3 = LINEDEF;}

		divider_line3.v1 = divider_v1;
		divider_line3.v2 = pocket1_v4;

		divider_line3.twosided = true;

		divider_line3.sidefront = side_clear;
		divider_line3.sideback  = side_clear;

	# if not exists local : divider_line4
		{divider_line4 = LINEDEF;}

		divider_line4.v1 = divider_v2;
		divider_line4.v2 = pocket1_v1;

		divider_line4.twosided = true;

		divider_line4.sidefront = side_clear;
		divider_line4.sideback  = side_clear;

	# if not exists local : divider_line5
		{divider_line5 = LINEDEF;}

		divider_line5.v1 = divider_v2;
		divider_line5.v2 = pocket2_v4;

		divider_line5.twosided = true;

		divider_line5.sidefront = side_clear;
		divider_line5.sideback  = side_clear;



	# if not exists local : box_line1
		{box_line1 = LINEDEF;}

		box_line1.v1 = box_v1;
		box_line1.v2 = box_v2;

		box_line1.sidefront = side_trak;

	# if not exists local : box_line2
		{box_line2 = LINEDEF;}

		box_line2.v1 = box_v2;
		box_line2.v2 = box_v3;

		box_line2.sidefront = side_trak;

	# if not exists local : box_line3
		{box_line3 = LINEDEF;}

		box_line3.v1 = box_v3;
		box_line3.v2 = box_v4;

		box_line3.sidefront = side_trak;

	# if not exists local : box_line4
		{box_line4 = LINEDEF;}

		box_line4.v1 = box_v4;
		box_line4.v2 = box_v5;

		box_line4.sidefront = side_trak;

	# if not exists local : box_line5
		{box_line5 = LINEDEF;}

		box_line5.v1 = box_v5;
		box_line5.v2 = box_v6;

		box_line5.sidefront = side_trak;

	# if not exists local : box_line6
		{box_line6 = LINEDEF;}

		box_line6.v1 = box_v6;
		box_line6.v2 = box_v1;

		box_line6.sidefront = side_trak;

	# if not exists local : box_line7
		{box_line7 = LINEDEF;}

		box_line7.v1 = box_v2;
		box_line7.v2 = box_v5;

		box_line7.twosided = true;

		box_line7.sidefront = side_trak;
		box_line7.sideback  = side_trak;

	# if not exists local : door1_line1
		{door1_line1 = LINEDEF;}

		door1_line1.v1 = door1_v1;
		door1_line1.v2 = door1_v2;

		door1_line1.special = 1;
		door1_line1.arg0    = _polyNum1;
		door1_line1.arg1    = _polyNum2;
		door1_line1.arg2    = 1;

		door1_line1.sidefront = side_trak;

	# if not exists local : door1_line2
		{door1_line2 = LINEDEF;}

		door1_line2.v1 = door1_v2;
		door1_line2.v2 = door1_v3;

		door1_line2.playeruse     = true;
		door1_line2.repeatspecial = true;

		door1_line2.special = _special;
		door1_line2.arg0    = _polyNum1;
		door1_line2.arg1    = _special_speed;
		door1_line2.arg2    = [](float)[round][deg2byte]_angleRight;
		door1_line2.arg3    = [](float)[round]_doorWidth;
		door1_line2.arg4    = _special_delay;

		door1_line2.sidefront = side_door2;

	# if not exists local : door1_line3
		{door1_line3 = LINEDEF;}

		door1_line3.v1 = door1_v3;
		door1_line3.v2 = door1_v4;

		door1_line3.sidefront = side_trak;

	# if not exists local : door1_line4
		{door1_line4 = LINEDEF;}

		door1_line4.v1 = door1_v4;
		door1_line4.v2 = door1_v1;

		door1_line4.playeruse     = true;
		door1_line4.repeatspecial = true;

		door1_line4.special = _special;
		door1_line4.arg0    = _polyNum1;
		door1_line4.arg1    = _special_speed;
		door1_line4.arg2    = [](float)[round][deg2byte]_angleRight;
		door1_line4.arg3    = [](float)[round]_doorWidth;
		door1_line4.arg4    = _special_delay;

		door1_line4.sidefront = side_door1;

	# if not exists local : door2_line1
		{door2_line1 = LINEDEF;}

		door2_line1.v1 = door2_v1;
		door2_line1.v2 = door2_v2;

		door2_line1.special = 1;
		door2_line1.arg0    = _polyNum2;
		door2_line1.arg1    = 0;
		door2_line1.arg2    = 1;

		door2_line1.sidefront = side_trak;

	# if not exists local : door2_line2
		{door2_line2 = LINEDEF;}

		door2_line2.v1 = door2_v2;
		door2_line2.v2 = door2_v3;

		door2_line2.playeruse     = true;
		door2_line2.repeatspecial = true;

		door2_line2.special = _special;
		door2_line2.arg0    = _polyNum1;
		door2_line2.arg1    = _special_speed;
		door2_line2.arg2    = [](float)[round][deg2byte]_angleRight;
		door2_line2.arg3    = [](float)[round]_doorWidth;
		door2_line2.arg4    = _special_delay;

		door2_line2.sidefront = side_door1;

	# if not exists local : door2_line3
		{door2_line3 = LINEDEF;}

		door2_line3.v1 = door2_v3;
		door2_line3.v2 = door2_v4;

		door2_line3.sidefront = side_trak;

	# if not exists local : door2_line4
		{door2_line4 = LINEDEF;}

		door2_line4.v1 = door2_v4;
		door2_line4.v2 = door2_v1;

		door2_line4.playeruse     = true;
		door2_line4.repeatspecial = true;

		door2_line4.special = _special;
		door2_line4.arg0    = _polyNum1;
		door2_line4.arg1    = _special_speed;
		door2_line4.arg2    = [](float)[round][deg2byte]_angleRight;
		door2_line4.arg3    = [](float)[round]_doorWidth;
		door2_line4.arg4    = _special_delay;

		door2_line4.sidefront = side_door2;



	# if not exists local : door1_anchor
		{door1_anchor = THING;}

		door1_anchor.x = (door1_v1.x + door1_v2.x + door1_v3.x + door1_v4.x) / 4;
		door1_anchor.y = (door1_v1.y + door1_v2.y + door1_v3.y + door1_v4.y) / 4;

		door1_anchor.type  = _typeAnchor;
		door1_anchor.angle = _polyNum1;

	# if not exists local : door1_start
		{door1_start = THING;}

		door1_start.x = (divider_v1.x + divider_v2.x + pocket1_v1.x + pocket1_v4.x) / 4;
		door1_start.y = (divider_v1.y + divider_v2.y + pocket1_v1.y + pocket1_v4.y) / 4;

		door1_start.type  = _typeStart;
		door1_start.angle = _polyNum1;

	# if not exists local : door2_anchor
		{door2_anchor = THING;}

		door2_anchor.x = (door2_v1.x + door2_v2.x + door2_v3.x + door2_v4.x) / 4;
		door2_anchor.y = (door2_v1.y + door2_v2.y + door2_v3.y + door2_v4.y) / 4;

		door2_anchor.type  = _typeAnchor;
		door2_anchor.angle = _polyNum2;

	# if not exists local : door2_start
		{door2_start = THING;}

		door2_start.x = (divider_v1.x + divider_v2.x + pocket2_v1.x + pocket2_v4.x) / 4;
		door2_start.y = (divider_v1.y + divider_v2.y + pocket2_v1.y + pocket2_v4.y) / 4;

		door2_start.type  = _typeStart;
		door2_start.angle = _polyNum2;



	# delete _;
}



