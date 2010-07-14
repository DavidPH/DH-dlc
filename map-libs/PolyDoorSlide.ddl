/* PolyDoorSlide compound object

	Creates a sliding poly-object door.

	   sector2
	    side2
	v4 -line2- -v3
	|           |
	v1 -line1- -v2
	    side1
	   sector1

	Required keys:
		[LINEDEF] line1
		[LINEDEF] line2

		[int] global.AS_Polyobj_DoorSlide
		[int] global.AS_Polyobj_SartLine
		[int] global.T_PolyObjectAnchor
		[int] global.T_PolyObjectStartHarmless

	Required libraries:
		VertexCenter
		VertexOffset

	Optional keys:
		[SECTOR] sector[1] (Default: side[1].sector)
		[SECTOR] sector[2] (Default: side[2].sector)
		[SIDEDEF] side[1] (Default: line[1].sidefront)
		[SIDEDEF] side[2] (Default: line[2].sidefront)
		[VERTEX] v[1] (Default: line[1].v1)
		[VERTEX] v[2] (Default: line[1].v2)
		[VERTEX] v[3] (Default: line[2].v1)
		[VERTEX] v[4] (Default: line[2].v2)

		[int] door_delay (Default: 70)
		[float] door_flange (Default: global.DOOR_FLANGE or 1/256)
		[int] door_sound (Default: global.DOOR_SOUND or 1)
		[int] door_speed (Default: 16)

		[string] texture_door (Default: "DOOR2_4")
		[string] texture_trak (Default: global.DOOR_TRAK or "DOORTRAK")

	Output keys:
		TODO
*/

# if not exists : POLY_COUNT  {[short int] POLY_COUNT  = 0;}
# if not exists : POLY_OFFSET {[float]     POLY_OFFSET = 0x3FFF;}

# typedef new : PolyDoorSlide : compound object;

# default type : line1 : LINEDEF : PolyDoorSlide;
# default type : line2 : LINEDEF : PolyDoorSlide;

# default type : sector1 : SECTOR : PolyDoorSlide;
# default type : sector2 : SECTOR : PolyDoorSlide;

# default type : v1 : VERTEX : PolyDoorSlide;
# default type : v2 : VERTEX : PolyDoorSlide;
# default type : v3 : VERTEX : PolyDoorSlide;
# default type : v4 : VERTEX : PolyDoorSlide;

# default type : door_delay  : int   : PolyDoorSlide;
# default type : door_flange : float : PolyDoorSlide;
# default type : door_sound  : int   : PolyDoorSlide;
# default type : door_speed  : int   : PolyDoorSlide;

# default type : texture_door : string : PolyDoorSlide;
# default type : texture_trak : string : PolyDoorSlide;



/*

                              side2
                4- - - - - - -line2- - - - - - -3
                |                               |
3---------------4- - - - - - - -2- - - - - - - -1---------------2
|                               |                               |
|    pocket1                 divider                 pocket2    |
|                               |                               |
2---------------1- - - - - - - -1- - - - - - - -4---------------3
                |                               |
                1- - - - - - -line1- - - - - - -2
                              side1

4---------------5---------------6
|4-------------3|4-------------3|
||    door1    |||    door2    ||
|1-------------2|1-------------2|
3---------------2---------------1

*/

#define:PolyDoorSlide
{
	# if not exists local : side1 {side1 = line1.sidefront;}
	# if not exists local : side2 {side2 = line2.sidefront;}

	# if not exists local : sector1 {sector1 = side1.sector;}
	# if not exists local : sector2 {sector2 = side2.sector;}

	# if not exists local : v1 {v1 = line1.v1;}
	# if not exists local : v2 {v2 = line1.v2;}
	# if not exists local : v3 {v3 = line2.v1;}
	# if not exists local : v4 {v4 = line2.v2;}



	[float] _angle1 = <facing>(v1, v2);
	[float] _angle2 = <facing>(v2, v3);
	[float] _angle3 = <facing>(v3, v4);
	[float] _angle4 = <facing>(v4, v1);

	[float] _dist1 = <distance>(v1, v2);
	[float] _dist2 = <distance>(v2, v3);
	[float] _dist3 = <distance>(v3, v4);
	[float] _dist4 = <distance>(v4, v1);

	[float] _flange = 1/256;
	[float] _offset = 1/256;

	[int] _doorAngle = (float)[round][deg2byte]_angle3;
	[int] _doorDelay = 70;
	[int] _doorDist  = _dist1/2;
	[int] _doorSound = 1;
	[int] _doorSpeed = 16;

	[string] _textureDoor = "$DOOR2_4";
	[string] _textureTrak = "$DOORTRAK";

	[short int] _poly1 = global.POLY_COUNT + 0;
	[short int] _poly2 = global.POLY_COUNT + 1;
	global.POLY_COUNT += 2;



	# if exists local : door_delay  {_doorDelay = door_delay;}
	# if exists       : DOOR_FLANGE {_flange    = DOOR_FLANGE;}
	# if exists local : door_flange {_flange    = door_flange;}
	# if exists       : DOOR_SOUND  {_doorSound = DOOR_SOUND;}
	# if exists local : door_sound  {_doorSound = door_sound;}
	# if exists local : door_speed  {_doorSpeed = door_speed;}

	# if exists local : texture_door {_textureDoor = texture_door;}
	# if exists       : DOOR_TRAK    {_textureTrak = DOOR_TRAK;}
	# if exists local : texture_trak {_textureTrak = texture_trak;}



	[SECTOR] sector0
	{
		heightceiling = sector1.heightceiling;
		heightfloor   = sector1.heightfloor;

		textureceiling = _textureTrak;
		texturefloor   = _textureTrak;
	}
	[SIDEDEF] side0
	{
		sector = sector0;
	}



	line1.twosided = true;
	line2.twosided = true;

	line1.sideback = side0;
	line2.sideback = side0;

	line1.sidefront.texturemiddle = $-;
	line2.sidefront.texturemiddle = $-;



	[object] pocket1;

	[SIDEDEF] pocket1.side1
	{
		sector = sector0;

		texturemiddle = _textureTrak;
	}
	[SIDEDEF] pocket1.side2 : pocket1.side1 {}
	[SIDEDEF] pocket1.side3 : pocket1.side2 {}
	[SIDEDEF] pocket1.side4 = side0;
	[SIDEDEF] pocket1.side5 : pocket1.side3 {}
	[SIDEDEF] pocket1.side6 : pocket1.side5 {}

	[VertexOffset] pocket1.v1 {b =         v1; a = _angle4+180; d = _flange;}
	[VertexOffset] pocket1.v2 {b = pocket1.v1; a = _angle1+180; d = _dist1/2;}
	[VertexOffset] pocket1.v4 {b =         v4; a = _angle4;     d = _flange;}
	[VertexOffset] pocket1.v3 {b = pocket1.v4; a = _angle1+180; d = _dist3/2;}
	[VERTEX]       pocket1.v5 = pocket1.v1;
	[VERTEX]       pocket1.v6 = v1;
	[VERTEX]       pocket1.v7 = v4;

	[object] pocket2;

	[SIDEDEF] pocket2.side1
	{
		sector = sector0;

		texturemiddle = _textureTrak;
	}
	[SIDEDEF] pocket2.side2 : pocket2.side1 {}
	[SIDEDEF] pocket2.side3 : pocket2.side2 {}
	[SIDEDEF] pocket2.side4 = side0;
	[SIDEDEF] pocket2.side5 : pocket2.side3 {}
	[SIDEDEF] pocket2.side6 : pocket2.side5 {}

	[VertexOffset] pocket2.v1 {b =         v3; a = _angle2+180; d = _flange;}
	[VertexOffset] pocket2.v2 {b = pocket2.v1; a = _angle3+180; d = _dist3/2;}
	[VertexOffset] pocket2.v4 {b =         v2; a = _angle2;     d = _flange;}
	[VertexOffset] pocket2.v3 {b = pocket2.v4; a = _angle3+180; d = _dist1/2;}
	[VERTEX]       pocket2.v5 = pocket2.v1;
	[VERTEX]       pocket2.v6 = v3;
	[VERTEX]       pocket2.v7 = v2;

	#for:indexPocket:1:3
	{
		#for:index:1:5
		{
			[LINEDEF] pocket[indexPocket].line[index]
			{
				sidefront = pocket[indexPocket].side[index];

				v1 = pocket[indexPocket].v[index  ];
				v2 = pocket[indexPocket].v[index+1];
			}
		}

		pocket[indexPocket].line4.{sideback = sidefront; twosided = true;}

		[LINEDEF] pocket[indexPocket].line5
		{
			v1 = pocket[indexPocket].v6;
			v2 = pocket[indexPocket].v1;

			sidefront = pocket[indexPocket].side5;
		}
		[LINEDEF] pocket[indexPocket].line6
		{
			v1 = pocket[indexPocket].v4;
			v2 = pocket[indexPocket].v7;

			sidefront = pocket[indexPocket].side5;
		}
	}



	[object] divider;

	[VertexCenter] divider.v1 {p1 = pocket1.v1; p2 = pocket2.v4;}
	[VertexCenter] divider.v2 {p1 = pocket1.v4; p2 = pocket2.v1;}

	[LINEDEF] divider.line5
	{
		twosided = true;

		sidefront = side0;
		sideback  = side0;

		v1 = divider.v1;
		v2 = divider.v2;
	}
	divider.line1:divider.line5 {v1 = pocket1.v1; v2 = divider.v1;}
	divider.line2:divider.line5 {v1 = divider.v1; v2 = pocket2.v4;}
	divider.line3:divider.line5 {v1 = pocket2.v1; v2 = divider.v2;}
	divider.line4:divider.line5 {v1 = divider.v2; v2 = pocket1.v4;}



	[object] doorholder;

	[VERTEX] doorholder.v1 : pocket2.v4 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v2 : divider.v1 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v3 : pocket1.v1 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v4 : pocket1.v4 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v5 : divider.v2 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v6 : pocket2.v1 {x += POLY_OFFSET; y += POLY_OFFSET;}
	[VERTEX] doorholder.v7 = doorholder.v1;

	doorholder.#for:index:1:7
	{
		[LINEDEF] line[index]
		{
			sidefront = side0;

			v1 = v[index  ];
			v2 = v[index+1];
		}
	}

	doorholder.
	{
		[LINEDEF] line7
		{
			twosided = true;

			sideback  = side0;
			sidefront = side0;

			v1 = v2;
			v2 = v5;
		}
	}



	[object] door1;

	[SIDEDEF] door1.side1
	{
		sector = sector0;

		texturemiddle = _textureDoor;
	}
	[SIDEDEF] door1.side2 : door1.side1
	{
		texturemiddle = _textureTrak;
	}
	[SIDEDEF] door1.side3 : door1.side1
	{
		offsetx = <distance>(doorholder.v6, doorholder.v5);
	}
	[SIDEDEF] door1.side4 : door1.side2;

	[VertexOffset] door1.v1 {b = doorholder.v3; a = <facing>(b, doorholder.v5); d = _offset;}
	[VertexOffset] door1.v2 {b = doorholder.v2; a = <facing>(b, doorholder.v4); d = _offset;}
	[VertexOffset] door1.v3 {b = doorholder.v5; a = <facing>(b, doorholder.v3); d = _offset;}
	[VertexOffset] door1.v4 {b = doorholder.v4; a = <facing>(b, doorholder.v2); d = _offset;}
	[VERTEX]       door1.v5 = door1.v1;

	[object] door2;

	[SIDEDEF] door2.side1
	{
		offsetx = <distance>(doorholder.v3, doorholder.v2);

		sector = sector0;

		texturemiddle = _textureDoor;
	}
	[SIDEDEF] door2.side2 : door1.side1
	{
		offsetx = 0;

		texturemiddle = _textureTrak;
	}
	[SIDEDEF] door2.side3 : door1.side1
	{
		offsetx = 0;
	}
	[SIDEDEF] door2.side4 : door1.side2;

	[VertexOffset] door2.v1 {b = doorholder.v2; a = <facing>(b, doorholder.v6); d = _offset;}
	[VertexOffset] door2.v2 {b = doorholder.v1; a = <facing>(b, doorholder.v5); d = _offset;}
	[VertexOffset] door2.v3 {b = doorholder.v6; a = <facing>(b, doorholder.v2); d = _offset;}
	[VertexOffset] door2.v4 {b = doorholder.v5; a = <facing>(b, doorholder.v1); d = _offset;}
	[VERTEX]       door2.v5 = door2.v1;

	#for:indexDoor:1:3
	{
		#for:index:1:5
		{
			[LINEDEF] door[indexDoor].line[index]
			{
				sidefront = door[indexDoor].side[index];

				v1 = door[indexDoor].v[index  ];
				v2 = door[indexDoor].v[index+1];
			}
		}

		#for:index:1:4:2
		{
			door[indexDoor].line[index].
			{
				playeruse     = true;
				repeatspecial = true;

				special = AS_Polyobj_DoorSlide;
				arg0 = _poly1;
				arg1 = _doorSpeed;
				arg2 = _doorAngle;
				arg3 = _doorDist;
				arg4 = _doorDelay;
			}
		}
	}

	door1.line4.
	{
		special = AS_Polyobj_StartLine;
		arg0 = _poly1;
		arg1 = _poly2;
		arg2 = _doorSound;
		arg3 = 0;
	}

	door2.line4.
	{
		special = AS_Polyobj_StartLine;
		arg0 = _poly2;
		arg1 = 0;
		arg2 = 0;
		arg3 = 0;
	}



	#for:index:1:3
	{
		[VertexCenter] anchor[index]
		{
			p1 = door[index].v1;
			p2 = door[index].v3;

			#compound;

			#changetype:THING;

			angle = _poly[index];

			type = T_PolyObjectAnchor;
		}
	}

	[VertexCenter] start1
	{
		p1 = divider.v1;
		p2 = pocket1.v4;

		#compound;

		#changetype:THING;

		angle = _poly1;

		type = T_PolyObjectStartHarmless;
	}
	[VertexCenter] start2
	{
		p1 = divider.v1;
		p2 = pocket2.v1;

		#compound;

		#changetype:THING;

		angle = _poly2;

		type = T_PolyObjectStartHarmless;
	}


	# delete volatile;
}



