
/*
Name: Umer Ahmed
ID: 1010091
Date: Febuary 4th 2019
Assignment 1
*/

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

char* addEventWrapper(char* filename,char* eventJSON,char* eventsummary,char* eventSD,char* eventST,char* eventUTC, char* eventCD, char* eventCT);

char* createCalendarWrapper(char* filename, char* calJSON,char* eventJSON,char* eventsummary,char* eventSD,char* eventST,char* eventUTC, char* eventCD, char* eventCT);
void addPropToEvent(Event* event, Property* toBeAdded);

char* propToJSON(const Property* prop);
char* propListToJSON(const List* propList);
char* propObjectToJSON(char* filename, int eventNum);

char* alarmObjectToJSON(char* filename, int alarmNum);
char* alarmListToJSON(const List* alarmList);
char* alarmToJSON(const Alarm* alarm);
char* eventObjectToJSON(char* filename);
char* eventListWithFilenameToJSON(const List* eventList, char* filename);
char* calendarWithFilenameToJSON(const Calendar* cal, char* filename);
char* objectToJSON(char* filename);
ICalErrorCode validateEvent(const Event* obj);
ICalErrorCode validateAlarm(const Alarm* obj);
Alarm* AlarmParse(char** charTable, char** clDescription, int i, int numLines);
void dateTimeParse(char* clDescription, char (*tempDate)[9], char (*tempTime)[7], bool* tempUTC);
Event* EventParse(char** charTable, char** clDescription, int i, int numLines);
void CalParse(Calendar** obj, char** charTable, int numLines, char** clDescription);
void LineArrayParmCalc(char* fileName, int* maxLen, int* numLines);
void ClfrTrimmer(char*** charTable,int numberOfLines, int maxNumberofChars);
void getName(char* charTable, char (*clName)[200]);
void getPropDescr(char *charTable, char** clPropDescr);
void getPropVal(char *charTable, char** clPropDescr);
bool dtValidator(char **clDescription);
bool fileCheck(char *fileName);
ICalErrorCode validArray(char** charTable, int numLines, char** clDescription);
int ignoreCaseCompare(char *a, char *b);
int getIndex(char clName[200], char **charTable, int numLines);

#endif
