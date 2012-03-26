//DDL
// VertexOffset compound object

# typedef new : VertexOffset : compound object;

# default type : b : VERTEX : VertexOffset;
# default type : a : float  : VertexOffset;
# default type : d : float  : VertexOffset;

# default type : x : float  : VertexOffset;
# default type : y : float  : VertexOffset;



# define : VertexOffset
{
	# if exists local : b
	{
		x = b.x;
		y = b.y;

		# delete : b;
	}

	# if not exists local : a {#error:"$a required";}
	# if not exists local : d {#error:"$d required";}
	# if not exists local : x {#error:"$x required";}
	# if not exists local : y {#error:"$y required";}

	// Needed as radians, but degrees make a better interface.
	a = [deg2rad]a;

	x += d * [sin](([pi] / 2) - a);
	y += d * [sin]a;

	# delete : a;
	# delete : d;

	# change type : VERTEX;
}



