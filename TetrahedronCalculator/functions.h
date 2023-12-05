#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//constants
#define TVERTICES 4
#define TFACES 4
#define TEDGES 6




//structures
struct solid
	{
        float area;	// surface area of a 3D solid
        float volume;	// volume of a 3D solid
        int faces;	// number of faces in a 3D solid
        int vertices;	// number of vertices in a 3D solid
        int edges;	// number of edges in a 3D solid
	};
struct solid get_tetra (float side);


//function prototypes
void printsolid(char *name, struct solid mysolid);