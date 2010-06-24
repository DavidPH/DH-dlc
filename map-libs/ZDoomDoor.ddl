// ZDoomDoor compound object

# include : "std-zdoom.ddl";



# typedef new : ZDoomDoor : compound object;

# default type : v1 : VERTEX : ZDoomDoor;
# default type : v2 : VERTEX : ZDoomDoor;
# default type : v3 : VERTEX : ZDoomDoor;
# default type : v4 : VERTEX : ZDoomDoor;

# default type : side1 : SIDEDEF : ZDoomDoor;
# default type : side2 : SIDEDEF : ZDoomDoor;

# default type :           height : int : ZDoomDoor;
# default type : ZDoomDoor_height : int;

# default type :           textureceiling : string : ZDoomDoor;
# default type : ZDoomDoor_textureceiling : string;
# default type :           texturefloor   : string : ZDoomDoor;
# default type : ZDoomDoor_texturefloor   : string;
# default type :           texturetrak    : string : ZDoomDoor;
# default type : ZDoomDoor_texturetrak    : string;

# default type :           speed : int : ZDoomDoor;
# default type : ZDoomDoor_speed : int;
# default type :           delay : int : ZDoomDoor;
# default type : ZDoomDoor_delay : int;
# default type :           lock  : int : ZDoomDoor;
# default type : ZDoomDoor_lock  : int;



# define : ZDoomDoor
{
	# if not exists local : v1 {#error:"$v1 required";}
	# if not exists local : v2 {#error:"$v2 required";}
	# if not exists local : v3 {#error:"$v3 required";}
	# if not exists local : v4 {#error:"$v4 required";}

	# if not exists local : side1 {#error:"$side1 required";}
	# if not exists local : side2 {#error:"$side2 required";}

	# if not exists local : door_sector
		{door_sector = SECTOR;}

		door_sector.heightceiling = 0;
		door_sector.heightfloor   = 0;

		# if exists : ZDoomDoor_height
		{
			door_sector.heightceiling = ZDoomDoor_height;
			door_sector.heightfloor   = ZDoomDoor_height;
		}

		# if exists local : height
		{
			door_sector.heightceiling = height;
			door_sector.heightfloor   = height;
		}

		door_sector.textureceiling = "$DOORTRAK";
		door_sector.texturefloor   = "$DOORTRAK";

		# if exists : ZDoomDoor_textureceiling
			{door_sector.textureceiling = ZDoomDoor_textureceiling;}
		# if exists : ZDoomDoor_texturefloor
			{door_sector.texturefloor = ZDoomDoor_texturefloor;}

		# if exists local : textureceiling
			{door_sector.textureceiling = textureceiling;}
		# if exists local : texturefloor
			{door_sector.texturefloor = texturefloor;}

		door_sector.# if cmp : door_sector.texturefloor : eq : "$DOORTRAK" : string
		{
			rotationfloor   = <facing>(v1, v4);
			rotationceiling = <facing>(v1, v4);
		}

	# if not exists local : side_door
		{side_door = SIDEDEF;}

		side_door.sector = door_sector;

	# if not exists local : side_trak
		{side_trak = SIDEDEF;}

		side_trak.texturemiddle = "$DOORTRAK";

		# if exists : ZDoomDoor_texturetrak
			{side_trak.texturemiddle = ZDoomDoor_texturetrak;}

		# if exists local : texturetrak
			{side_trak.texturemiddle = texturetrak;}

		side_trak.sector = door_sector;

	# if not exists local : line1
		{line1 = LINEDEF;}

		line1.v1 = v2;
		line1.v2 = v1;

		line1.twosided      = true;
		line1.playeruse     = true;
		line1.repeatspecial = true;

		line1.special = 13;
		line1.arg0    = 0;
		line1.arg1    = 32;
		line1.arg2    = 140;
		line1.arg3    = 0;
		line1.arg4    = 0;

		# if exists : ZDoomDoor_speed
			{line1.arg1 = ZDoomDoor_speed;}
		# if exists : ZDoomDoor_delay
			{line1.arg2 = ZDoomDoor_delay;}
		# if exists : ZDoomDoor_lock
			{line1.arg3 = ZDoomDoor_lock;}

		# if exists local : speed
			{line1.arg1 = speed;}
		# if exists local : delay
			{line1.arg2 = delay;}
		# if exists local : lock
			{line1.arg3 = lock;}

		line1.sidefront = side1;
		line1.sideback  = side_door;

	# if not exists local : line2
		{line2 = LINEDEF;}

		line2.v1 = v2;
		line2.v2 = v3;

		line2.dontpegtop    = true;
		line2.dontpegbottom = true;

		line2.sidefront = side_trak;

	# if not exists local : line3
		{line3 = LINEDEF;}

		line3.v1 = v4;
		line3.v2 = v3;

		line3.twosided      = true;
		line3.playeruse     = true;
		line3.repeatspecial = true;

		line3.special = 13;
		line3.arg0    = 0;
		line3.arg1    = 32;
		line3.arg2    = 140;
		line3.arg3    = 0;
		line3.arg4    = 0;

		# if exists : ZDoomDoor_speed
			{line3.arg1 = ZDoomDoor_speed;}
		# if exists : ZDoomDoor_delay
			{line3.arg2 = ZDoomDoor_delay;}
		# if exists : ZDoomDoor_lock
			{line3.arg3 = ZDoomDoor_lock;}

		# if exists local : speed
			{line3.arg1 = speed;}
		# if exists local : delay
			{line3.arg2 = delay;}
		# if exists local : lock
			{line3.arg3 = lock;}

		line3.sidefront = side2;
		line3.sideback  = side_door;

	# if not exists local : line4
		{line4 = LINEDEF;}

		line4.v1 = v4;
		line4.v2 = v1;

		line4.dontpegtop    = true;
		line4.dontpegbottom = true;

		line4.sidefront = side_trak;
}



