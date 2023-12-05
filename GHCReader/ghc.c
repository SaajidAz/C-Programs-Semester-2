/** @serial 102c9975
 *  @file lab01.c
 */
#include "ghcontrol.h"


int main(void)
{
	readings creadings = {0};
	controls ctrl = {0};
	setpoints sets = {0};
	sets = GhSetTargets(); 
	int logged;
	alarmlimits alimits = { 0 };
	alarms *arecord;

	arecord = (alarms *) calloc(1,sizeof(alarms));
		if (arecord == NULL)
	{
		printf("\nCannot allocate memory\n");
		return EXIT_FAILURE;
	}

	

	time_t now;
	now = time(NULL);
	GhDisplayHeader("SAAJID AZIZ");
	while (1)
	{	
		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt", creadings);
		now = time(NULL);
		GhDisplayAll(creadings, sets);
		GhDisplayReadings(creadings);
		GhDelay(GHUPDATE);
		GhControllerInit();
		ctrl = GhSetControls(sets,creadings);
		GhSetAlarms(arecord,alimits,creadings);
		GhDisplayTargets(sets);
		alimits = GhSetAlarmLimits();
		GhDisplayControls(ctrl);
		GhDisplayAlarms(arecord);
			
	}




return EXIT_FAILURE;

}



