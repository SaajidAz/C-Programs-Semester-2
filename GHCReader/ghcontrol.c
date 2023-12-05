
#include "ghcontrol.h"
#include <cstring>
#include <string.h>


const char alarmnames[NALARMS][ALARMNMSZ] =
        {"No Alarms","High Temperature","Low Temperature","High Humidity",
 "Low Humidity","High Pressure","Low Pressure"};


/** @brief prints the constant sname along with CENG153 Greenhouse Controller 
@version 13FEB2022* 
@author Saajid Aziz* 
@param const char * sname* 
@return void
*/

SenseHat Sh;


void GhDisplayHeader(const char * sname)
{
	fprintf(stdout,"%s's CENG153 Greenhouse Controller\n",sname);
}






	u_int64_t GhGetSerial(void)
{
	static u_int64_t serial = 0;
	FILE * fp;
	char buf[SYSINFOBUFSZ];
	char searchstring[] = SEARCHSTR;
	fp = fopen ("/proc/cpuinfo", "r");
	if (fp != NULL)
	{while (fgets(buf, sizeof(buf), fp) != NULL)
	{
	if (
	!strncasecmp(searchstring, buf, strlen(searchstring)))
	{
	sscanf(buf+strlen(searchstring), "%Lx", &serial);}
	}
	fclose(fp);
	}
	if(serial==0)
	{system("uname -a");
	system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
	fp = fopen ("stamp.txt", "r");
	if (fp != NULL)
	{
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
	sscanf(buf, "%Lx", &serial);
	}
	fclose(fp);
	}	
	}
 	return serial;}



 	int GhGetRandom(int range)
{
	return rand() % range;
}

	void GhDelay(int milliseconds)
{
	long wait;
	clock_t now,start;
	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait )
	{
		now = clock();
	}
}





	void GhControllerInit(void)
{
	srand((unsigned) time(NULL));
}

	void GhDisplayControls( controls ctrl)
{
	
	if (ctrl.heater == ON)
		fprintf(stdout, "\nThe heater is ON");
	else 
		fprintf(stdout, "\nThe heater is OFF");	

	if (ctrl.humidifier == ON)
		fprintf(stdout, "\tThe humidifier is ON\n");
	else 
		fprintf(stdout, "\tThe humidifier is OFF\n");
}

	void GhDisplayReadings( readings rdata)
{
	printf("\n%sReadings\tT: %4.1fC\tH: %4.1f%%\tP: %6.1fmB",
	ctime(&rdata.rtime),rdata.temperature,rdata.humidity,
	rdata.pressure);
}
	void GhDisplayTargets( setpoints spts)
{
	fprintf(stdout,"\nTarget Temperature: %5.1fC\tTarget Humidity: %5.1f", spts.temperature, spts.humidity);
}

	 controls GhSetControls( setpoints target,  readings rdata)
{
	struct controls cset;

	if(rdata.temperature < target.temperature)
		{cset.heater = ON;}
	else
		{cset.heater = OFF;}

	if(rdata.humidity < target.humidity)
		{cset.humidifier = ON;}
	else 	
		{cset.humidifier = OFF;}

	return cset;
}

 setpoints GhSetTargets(void)
{
    struct setpoints cpoints;
    cpoints = GhRetrieveSetpoints("setpoints.dat");
    
    if (cpoints.temperature == 0)
    {
        cpoints.temperature = STEMP;
        cpoints.humidity = SHUMID;
        GhSaveSetpoints("setpoints.dat", cpoints);
    }
    
    return cpoints;
}

	float GhGetHumidity(void)
{
	#if SIMHUMIDITY
	return GhGetRandomRange(LSHUMID, USHUMID);
	#else
	return Sh.GetHumidity();
	#endif
}

	float GhGetPressure(void)
{
	#if SIMPRESSURE
	return GhGetRandomRange(LSPRESS, USPRESS);
	#else
	return Sh.GetPressure();
	#endif
}

	float GhGetTemperature(void)
{
	#if SIMTEMPERATURE
	return GhGetRandomRange(LSTEMP, USTEMP);
	#else
	return Sh.GetTemperature();
	#endif
}

	 readings GhGetReadings(void)
{
	 readings now = {0};
	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}

	int GhGetRandomRange(int lower, int upper)
{
        return rand() % (upper + 1 - lower) + lower ;
}

int GhLogData(const char *fname,  readings ghdata)
{
	FILE *fp;
	char ltime[CTIMESTRSZ];

fp = fopen(fname,"a");
if(fp == NULL)
{
	fprintf(stdout,"\nCan't open file, data not retrieved!\n");
	return 0;
}

strcpy(ltime, ctime(&ghdata.rtime));

ltime[3]=',';
ltime[7]=',';
ltime[10]=',';
ltime[19]=',';

fprintf(fp,"\n%.24s,%5.1f,%5.1f,%6.1f", ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);

fclose(fp);
return 1;
}

int GhSaveSetpoints(const char *fname,  setpoints spts)
{
	FILE *fp;
		
	fp = fopen(fname,"w");
	if(fp == NULL)
	{
	fprintf(stdout,"\nCan't open file, data not retrieved!\n");
	return 0;
	}

	fwrite(&spts, sizeof( setpoints),1, fp);

	fclose(fp);
	return 1;

}

 setpoints GhRetrieveSetpoints(const char *fname) 
{
    
     setpoints spts = {0.0, 0.0};
    
    
    FILE *fp;
    
        if ((fp = fopen(fname, "rb")) == NULL) 
	{
        return spts;
    	}
    
 
    fread(&spts, sizeof( setpoints), 1, fp);
    
    
    fclose(fp);
    return spts;
}


int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc, uint8_t length)
{
	int i;
	if (length > 7)
		length = 7;
	if (bar >= 0 && bar < 8 && length >= 0 && length < 8)
		{
			for (i=0; i<= length; i++)
        			{
            			Sh.LightPixel(i, bar, pxc);
        			}
			for (i=length+1; i <8;  i++)
				{
				Sh.LightPixel(i,bar, BLACK);
				}

			return EXIT_SUCCESS;
		}  
	else
		{return EXIT_FAILURE;}


}

void GhDisplayAll( readings rd,  setpoints sd)
{
	int rv,sv,avh,avl;
	Sh.WipeScreen();

	rv  = (NUMPTS * (((rd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	sv  = (NUMPTS * (((sd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	
	GhSetVerticalBar(TBAR, GREEN, rv);
	if (rd.temperature < sd.temperature) Sh.LightPixel(sv,TBAR, RED);
	else Sh.LightPixel(sv,TBAR,MAGENTA);

	rv  = (NUMPTS * (((sd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	sv  = (NUMPTS * (((sd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	
	GhSetVerticalBar(HBAR, GREEN, rv);
	if (rd.humidity < sd.humidity) Sh.LightPixel(sv,HBAR, RED);
	else Sh.LightPixel(sv,HBAR,MAGENTA);

	
	rv  = (NUMPTS * (((rd.pressure-LSPRESS) / (USPRESS-LSPRESS))+0.05))-1.0;
	


	GhSetVerticalBar(PBAR, GREEN, rv);

}



alarmlimits GhSetAlarmLimits(void)
{
	alarmlimits calarm;

	calarm.hight = HIGHATEMP;
	calarm.lowt = LOWATEMP;
	calarm.highh = HIGHAHUMID;
	calarm.lowh = LOWAHUMID;
	calarm.highp = HIGHAPRESS;
	calarm.lowp = LOWAPRESS;
	
	return calarm;
	
}

alarms *GhSetAlarms(alarms *head, alarmlimits alarmpt, readings rdata)
{
	

if(rdata.temperature >= alarmpt.hight)
	{
	GhSetOneAlarm(HTEMP, rdata.rtime, rdata.temperature, head);
	}
	else
	{
		head = GhClearOneAlarm(HTEMP,head);
	}

if(rdata.temperature <= alarmpt.lowt)
    {
        GhSetOneAlarm(LTEMP, rdata.rtime, rdata.temperature, head);
    }
    else
    {
        head = GhClearOneAlarm(LTEMP,head);
    }


if(rdata.humidity >= alarmpt.highh)
    {
        GhSetOneAlarm(HHUMID, rdata.rtime, rdata.humidity, head);
    }
    else
    {
        head = GhClearOneAlarm(HHUMID,head);
    }

    if(rdata.humidity <= alarmpt.lowh)
    {
        GhSetOneAlarm(LHUMID, rdata.rtime, rdata.humidity, head);
    }
    else
    {
        head = GhClearOneAlarm(LHUMID,head);
    }


if(rdata.pressure >= alarmpt.highp)
    {
        GhSetOneAlarm(HPRESS, rdata.rtime, rdata.pressure, head);
    }
    else
    {
        head = GhClearOneAlarm(HPRESS,head);
    }

    if(rdata.pressure <= alarmpt.lowp)
    {
        GhSetOneAlarm(LPRESS, rdata.rtime, rdata.pressure, head);
    }
    else
    {
        head = GhClearOneAlarm(LPRESS,head);
    }

return head;
}


void GhDisplayAlarms(alarms *head)
{	
alarms *cur;
cur = head;
int numalarm=0;

while (cur != NULL)
    	{
        if( cur->code != NOALARM)
        	{
          	  fprintf(stdout,"%s alarm %s",alarmnames[cur->code],ctime( &(cur->atime) ));
		  numalarm++;
        	}

        cur=cur->next;

	}
printf("there is %d alarm(s)", numalarm);
}

int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarms *head)
{
    alarms *cur;
    alarms *last;

    cur = head;
    // Is it the first item on an empty list?
    if (cur->code != NOALARM)
    {
        while (cur != NULL) // loop until the last record of the linked list
        {
            if (cur->code == code) // alarm already set?
            {
                return 0; 
            }
            last = cur;
            cur = cur->next; // current is not the next item in list
        }
        cur = (alarms *) calloc(1,sizeof(alarms));
        if (cur == NULL) { return 0;} // return if fail
        last->next = cur; // update the previous record
    }
    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;
    return 1;
}

alarms *GhClearOneAlarm(alarm_e code, alarms *head)
{
     alarms *last = head;
    alarms *cur = head;
    
   
    if (cur != NULL && cur->code == code && cur->next == NULL) {
        cur->code = NOALARM;
        return head;
    }
    
 
    if (cur != NULL && cur->code == code && cur->next != NULL) {
        head = cur->next;
        free(cur);
        return head;
    }
    
   
    while (cur != NULL && cur->code != code) {
        last = cur;
        cur = cur->next;
    }
    
   
    if (cur != NULL && cur->code == code) {
        last->next = cur->next;
        free(cur);
        return head;
    }
    
    
    return head;
}
