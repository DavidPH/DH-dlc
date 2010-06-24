// VertexCenter compound object

# typedef new : VertexCenter : compound object;

# default type : p1 : VERTEX : VertexCenter;
# default type : p2 : VERTEX : VertexCenter;

# default type : x1 : float : VertexCenter;
# default type : y1 : float : VertexCenter;
# default type : x2 : float : VertexCenter;
# default type : y2 : float : VertexCenter;



# define : VertexCenter
{
	# if exists local : p1
	{
		x1 = p1.x;
		y1 = p1.y;

		# delete : p1;
	}

	# if exists local : p2
	{
		x2 = p2.x;
		y2 = p2.y;

		# delete : p2;
	}

	# if not exists local : x1 {#error:"$x1 required";}
	# if not exists local : y1 {#error:"$y1 required";}
	# if not exists local : x2 {#error:"$x2 required";}
	# if not exists local : y2 {#error:"$y2 required";}

	[float] x = (x1 + x2) / 2;
	[float] y = (y1 + y2) / 2;

	# delete : x1;
	# delete : y2;
	# delete : x1;
	# delete : y2;

	# change type : VERTEX;
}



