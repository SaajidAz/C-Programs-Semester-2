
/** @brief Constants, structures, function prototypes for Gh control code*
@file ghcontrol.h*/

#ifndef GHCONTROL_H
#define GHCONTROL_H
#define GHUPDATE 2000

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sensehat.h"

// Constants
#define SEARCHSTR "serial\t\t:"
#define SYSINFOBUFSZ 512
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMULATE 1
#define LSTEMP -10
#define USTEMP 50
#define LSHUMID 0
#define USHUMID 100
#define LSPRESS 975
#define USPRESS 1016
#define STEMP 25.0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0
#define CTIMESTRSZ 25
#define SENSEHAT 1
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define ALARMNMSZ 18
#define NALARMS 7
#define LOWATEMP 10
#define HIGHATEMP 30
#define LOWAHUMID 25
#define HIGHAHUMID 70
#define LOWAPRESS 985
#define HIGHAPRESS 1016







//Structures
struct readings GhGetReadings(void);

struct setpoints GhSetTargets(void);

typedef struct readings
{
	time_t rtime;
	float temperature;
	float humidity;
	float pressure;
} readigs;

typedef struct setpoints
{
float temperature;
float humidity;
} setpoints;

typedef struct controls
{
int heater;
int humidifier;
} controls;


typedef enum { NOALARM ,HTEMP ,LTEMP ,HHUMID ,LHUMID ,HPRESS ,LPRESS  } alarm_e;

typedef struct alarmlimits
{
float lowt;
float hight;
float lowh;
float highh;
float lowp;
float highp;
} alarmlimits;

typedef struct alarms
{
alarm_e code;
time_t atime;
double value;
struct alarms * next;
} alarms;
 
void GhDisplayReadings(readings rdata);

controls GhSetControls(setpoints target, readings rdata);

void GhDisplayControls(controls ctrl);


// Function Prototypes
///@cond INTERNAL
 void GhDisplayHeader(const char * sname);

 u_int64_t GhGetSerial(void);

 int GhGetRandom(int range);

void GhDelay(int milliseconds);

void GhControllerInit(void);


void GhDisplayTargets( setpoints spts);




float GhGetHumidity(void);

float GhGetPressure(void);

float GhGetTemperature(void);


int GhGetRandomRange(int lower, int upper);
///@endcond

int GhLogData(const char *fname,  readings ghdata);

int GhSaveSetpoints(const char *fname,  setpoints spts);

 setpoints GhRetrieveSetpoints(const char *fname);

int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc, uint8_t length);

void GhDisplayAll( readings rd,  setpoints sd);

alarmlimits GhSetAlarmLimits(void);

alarms *GhSetAlarms(alarms *head, alarmlimits alarmpt, readings rdata);

void GhDisplayAlarms(alarms *head);

int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarms *head);

alarms * GhClearOneAlarm(alarm_e code, alarms *head);

#endif
