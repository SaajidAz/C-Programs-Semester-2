/** @serial 102c9975
 *  @file labt.c
 */
#include "functions.h"

int main(void)
{

	float side = 0;
	struct solid mytetra = {0};
	char name[25]="tetrahedron";
	fprintf(stdout,"Saajid Aziz\n");
	
	while(side != 1)
	{
		fprintf(stdout, "Enter the length of the tetrahedron:");
		fscanf(stdin,"%f", &side);
		fprintf(stdout, "side length entered: %.2f", side);
		mytetra = get_tetra(side);
		printsolid(name, mytetra);
		
	}




	return EXIT_FAILURE;

}
