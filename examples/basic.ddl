//DHLX
//
// This example very crudely demonstrates the most basic map. It also shows the
// basic layout of a DH-dlc source file.
//
// To compile:
// DH-dlc --dir basic basic.ddl
// DH-wad --map basic basic.wad

// Quickly define some vertexes.
VERTEX vert0 {x = -256; y = -256;}
VERTEX vert1 {x = -256; y = +256;}
VERTEX vert2 {x = -512; y = +256;}
VERTEX vert3 {x = -512; y = +512;}
VERTEX vert4 {x = +256; y = +512;}
VERTEX vert5 {x = +256; y = -256;}

// And a simple sector.
SECTOR sector0
{
   heightceiling = 256;
   heightfloor = 0;

   textureceiling = "F_SKY1";
   texturefloor = "F_SKY1";
}

SIDEDEF side0
{
   sector = sector0;

   texturebottom = "-";
   texturemiddle = "DOORTRAK"; // So ugly that it MUST be an example!
   texturetop = "-";
}

// Make some lines!
LINEDEF line0
{
   sidefront = side0;

   v1 = vert0;
   v2 = vert1;
}
// These lines inherit all the members of line0, including using the same side.
LINEDEF line1 : line0 {v1 = vert1; v2 = vert2;}
LINEDEF line2 : line0 {v1 = vert2; v2 = vert3;}
LINEDEF line3 : line0 {v1 = vert3; v2 = vert4;}
LINEDEF line4 : line0 {v1 = vert4; v2 = vert5;}
LINEDEF line5 : line0 {v1 = vert5; v2 = vert0;}

// Need a player start of course.
THING player1
{
   coop = true;
   dm = true;
   single = true;

   skill1 = true;
   skill2 = true;
   skill3 = true;
   skill4 = true;
   skill5 = true;

   type = 1;

   x = 0;
   y = 0;
}

// And something to do once you fire the map up.
THING enemy1
{
   coop = true;
   dm = true;
   single = true;

   skill1 = true;
   skill2 = true;
   skill3 = true;
   skill4 = true;
   skill5 = true;

   type = 3004;

   x = 0;
   y = 128;
}

// EOF

