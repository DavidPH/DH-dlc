/* VertexCircle compound object

	Creates a circle of VERTEXes around a center point.

	Control variables:
		radius: Radius of circle.
		steps: Number of VERTEXes to create.
		x, y: Center of circle.

	Output variables:
		v[steps+1]: Array of VERTEXes in a circle starting with 0 and
			ending with steps. v[steps] is v[0].
*/

# include : "VertexOffset.ddl";

# typedef new : VertexCircle : compound object;

# default type : radius : float : VertexCircle;

# default type : steps : int : VertexCircle;

# default type : x : float : VertexCircle;
# default type : y : float : VertexCircle;

# define : VertexCircle
{
	# if not exists local : radius {#error:"$radius required";}

	# if not exists local : steps {#error:"$steps required";}

	# if not exists local : x {#error:"$x required";}
	# if not exists local : y {#error:"$y required";}

	# for : index : 0 : steps
	{
		[VertexOffset] v[index]
		{
			x = x;
			y = y;

			a = (360 / steps) * index;

			d = radius;
		}
	}

	[VERTEX] v[steps] = v[0];
}



