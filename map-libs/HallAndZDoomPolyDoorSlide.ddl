// HallAndZDoomPolyDoorSlide compound object

# include : "Hall.ddl"; // for Hall_ globals
# include : "ZDoomPolyDoorSlide.ddl";

# typedef new : HallAndZDoomPolyDoorSlide : compound object;

# default type : v1 : VERTEX : HallAndZDoomPolyDoorSlide;
# default type : v2 : VERTEX : HallAndZDoomPolyDoorSlide;
# default type : v3 : VERTEX : HallAndZDoomPolyDoorSlide;
# default type : v4 : VERTEX : HallAndZDoomPolyDoorSlide;

# default type : side1 : SIDEDEF : HallAndZDoomPolyDoorSlide;
# default type : side2 : SIDEDEF : HallAndZDoomPolyDoorSlide;

# default type : doorsize : float : HallAndZDoomPolyDoorSlide;

# default type : heightfloor   : int : HallAndZDoomPolyDoorSlide;
# default type : heightceiling : int : HallAndZDoomPolyDoorSlide;

# default type : texturefloor   : string : HallAndZDoomPolyDoorSlide;
# default type : textureceiling : string : HallAndZDoomPolyDoorSlide;
# default type : texturewall    : string : HallAndZDoomPolyDoorSlide;
# default type : texturedoor    : string : HallAndZDoomPolyDoorSlide;
# default type : texturetrak    : string : HallAndZDoomPolyDoorSlide;



# define : HallAndZDoomPolyDoorSlide
{
	# if not exists local : v1 {#error:"$v1 required";}
	# if not exists local : v2 {#error:"$v2 required";}
	# if not exists local : v3 {#error:"$v3 required";}
	# if not exists local : v4 {#error:"$v4 required";}

	# if not exists local : side1 {#error:"$side1 required";}
	# if not exists local : side2 {#error:"$side2 required";}

	[float] _doorSize = 48;
	# if exists : __DOOR_SIZE {_doorSize = __DOOR_SIZE;}
	# if exists local : doorsize {_doorSize = doorsize;}

	# if not exists local : v5
		{v5 = VertexOffset;}
	# else	{v5.#changetype:VertexOffset;}

		v5.b = v1;
		v5.a = <facing>(v1, v4);
		v5.d = _doorSize;

		v5.#compound;

	# if not exists local : v6
		{v6 = VertexOffset;}
	# else	{v6.#changetype:VertexOffset;}

		v6.b = v2;
		v6.a = <facing>(v2, v3);
		v6.d = _doorSize;

		v6.#compound;

	# if not exists local : v7
		{v7 = VertexOffset;}
	# else	{v7.#changetype:VertexOffset;}

		v7.b = v3;
		v7.a = <facing>(v3, v2);
		v7.d = _doorSize;

		v7.#compound;

	# if not exists local : v8
		{v8 = VertexOffset;}
	# else	{v8.#changetype:VertexOffset;}

		v8.b = v4;
		v8.a = <facing>(v4, v1);
		v8.d = _doorSize;

		v8.#compound;

	# if not exists local : hall_sector
		{hall_sector = SECTOR;}

		# if exists : Hall_heightceiling
			{hall_sector.heightceiling = Hall_heightceiling;}
		# if exists : Hall_heightfloor
			{hall_sector.heightfloor = Hall_heightfloor;}

		# if exists local : heightceiling
			{hall_sector.heightceiling = heightceiling;}
		# if exists local : heightfloor
			{hall_sector.heightfloor = heightfloor;}

		# if exists : Hall_textureceiling
			{hall_sector.textureceiling = Hall_textureceiling;}
		# if exists : Hall_texturefloor
			{hall_sector.texturefloor = Hall_texturefloor;}

		# if exists local : textureceiling
			{hall_sector.textureceiling = textureceiling;}
		# if exists local : texturefloor
			{hall_sector.texturefloor = texturefloor;}

	# if not exists local : side_hall
		{side_hall = SIDEDEF;}

		# if exists : Hall_texturewall
			{side_hall.texturemiddle = Hall_texturewall;}

		# if exists local : texturewall
			{side_hall.texturemiddle = texturewall;}

		side_hall.sector = hall_sector;

	# if not exists local : side_clear
		{side_clear = SIDEDEF;}

		# if exists : Hall_texturewall
		{
			side_clear.texturetop    = Hall_texturewall;
			side_clear.texturebottom = Hall_texturewall;
		}

		# if exists local : texturewall
		{
			side_clear.texturetop    = texturewall;
			side_clear.texturebottom = texturewall;
		}

		side_clear.sector = hall_sector;

	# if not exists local : line1
		{line1 = LINEDEF;}

		line1.v1 = v8;
		line1.v2 = v5;

		line1.sidefront = side_hall;

	# if not exists local : line2
		{line2 = LINEDEF;}

		line2.v1 = v6;
		line2.v2 = v7;

		line2.sidefront = side_hall;

	# if not exists local : door1
		{door1 = ZDoomPolyDoorSlide;}

		door1.v1 = v1;
		door1.v2 = v2;
		door1.v3 = v6;
		door1.v4 = v5;

		door1.side1 = side1;
		door1.side2 = side_clear;

		# if exists local : texturetrak
			{door1.texturetrak = texturetrak;}

		door1.#compound;

	# if not exists local : door2
		{door2 = ZDoomPolyDoorSlide;}

		door2.v1 = v8;
		door2.v2 = v7;
		door2.v3 = v3;
		door2.v4 = v4;

		door2.side1 = side_clear;
		door2.side2 = side2;

		# if exists local : texturetrak
			{door2.texturetrak = texturetrak;}

		door2.#compound;

	# delete : _doorSize;
}



