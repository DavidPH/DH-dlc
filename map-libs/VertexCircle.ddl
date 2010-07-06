/* VertexCircle compound object

	Creates a circle of VERTEXes around a center point.

	Required keys:
		[float] radius: Radius of circle.
		[float] steps: Number of VERTEXes to create.
		[float] x, y: Center of circle.

	Required libraries:
		VertexOffset

	Optional keys:
		[float] start: Start angle. (Default: 0)
		[float] stop: Stop angle. (Default: 360)

	Output keys:
		[VERTEX] v[steps+1]: Array of VERTEXes in a circle starting with
			0 and ending with steps. v[steps] is v[0].
*/

#include:"VertexOffset.ddl";

# typedef new : VertexCircle : compound object;

# default type : radius : float : VertexCircle;

# default type : steps : int : VertexCircle;

# default type : x : float : VertexCircle;
# default type : y : float : VertexCircle;

# default type : start : float : VertexCircle;
# default type : stop  : float : VertexCircle;

#define:VertexCircle
{
	# if not exists local : radius {#error:"$radius required";}

	# if not exists local : steps {#error:"$steps required";}

	# if not exists local : x {#error:"$x required";}
	# if not exists local : y {#error:"$y required";}

	# if not exists local : start {start =   0;}
	# if not exists local : stop  {stop  = 360;}

	[float] _spread      = start - stop;
	[bool]  _indexOffset = true;

	#if:<cmpf>(_spread, lt, 0)
	{
		_spread = -_spread;

		_indexOffset = false;
	}

	#for:index:0:steps
	{
		[VertexOffset] v[index]
		{
			x = x;
			y = y;

			#if:_indexOffset
				{a = ((_spread / steps) * (steps-1-index)) + start;}
			#else
				{a = ((_spread / steps) * (        index)) + start;}

			d = radius;
		}
	}

	[VERTEX] v[steps] = v[0];

	# delete volatile;
}



