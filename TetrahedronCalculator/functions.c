#include "functions.h"
#include <cstring>

struct solid get_tetra (float side)
{
	struct solid tetra;

	tetra.area = sqrt(3)*side*side;
	tetra.volume = (sqrt(2)/12.0)*side*side*side; 

	tetra.faces = (TFACES);
	tetra.vertices = (TVERTICES);
	tetra.edges = (TEDGES);
	
	return tetra;
}

void printsolid(char *name, struct solid mysolid)
{
	fprintf(stdout, "%s area = %.4f\n", name, mysolid.area);
	fprintf(stdout, "%s volume = %.4f\n", name, mysolid.volume);
	fprintf(stdout, " eulers  = %d\n", mysolid.vertices - mysolid.edges + mysolid.faces);

}