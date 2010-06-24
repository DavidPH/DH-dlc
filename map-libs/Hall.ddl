// Hall compound object

# typedef new : Hall : compound object;

# default type : v1 : VERTEX : Hall;
# default type : v2 : VERTEX : Hall;
# default type : v3 : VERTEX : Hall;
# default type : v4 : VERTEX : Hall;

# default type : side1 : SIDEDEF : Hall;
# default type : side2 : SIDEDEF : Hall;

# default type :      heightfloor   : int : Hall;
# default type : Hall_heightfloor   : int;
# default type :      heightceiling : int : Hall;
# default type : Hall_heightceiling : int;

# default type :      texturefloor   : string : Hall;
# default type : Hall_texturefloor   : string;
# default type :      textureceiling : string : Hall;
# default type : Hall_textureceiling : string;
# default type :      texturewall    : string : Hall;
# default type : Hall_texturewall    : string;



# define : Hall
{
	# if not exists local : v1 {#error:"$v1 required";}
	# if not exists local : v2 {#error:"$v2 required";}
	# if not exists local : v3 {#error:"$v3 required";}
	# if not exists local : v4 {#error:"$v4 required";}

	# if not exists local : side1 {#error:"$side1 required";}
	# if not exists local : side2 {#error:"$side2 required";}

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

		line1.v1 = v1;
		line1.v2 = v2;

		line1.twosided = true;

		line1.sidefront = side_clear;
		line1.sideback  = side1;

	# if not exists local : line2
		{line2 = LINEDEF;}

		line2.v1 = v2;
		line2.v2 = v3;

		line2.sidefront = side_hall;

	# if not exists local : line3
		{line3 = LINEDEF;}

		line3.v1 = v3;
		line3.v2 = v4;

		line3.twosided = true;

		line3.sidefront = side_clear;
		line3.sideback  = side2;

	# if not exists local : line4
		{line4 = LINEDEF;}

		line4.v1 = v4;
		line4.v2 = v1;

		line4.sidefront = side_hall;
}



