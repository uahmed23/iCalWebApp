/*
Name: Umer Ahmed
ID: 1010091
Date: Febuary 6th 2019
Assignment 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>


#include "HelperFunctions.h"
#include "CalendarParser.h"
#include "LinkedListAPI.h"

//ADD-EVENT BACKEND*******************************************************************************************************************************
char* addEventWrapper(char* filename,char* eventJSON,char* eventsummary,char* eventSD,char* eventST,char* eventUTC, char* eventCD, char* eventCT)
{
  Calendar* c =NULL;
  Event *e;
  Property *p;
  char* error = malloc(100);
  char* directory = malloc(200);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);


  ICalErrorCode err = createCalendar(directory, &c);

  if(err != OK)
  {
    return "{\"error\":\"Unable to add event.\"}";
  }

  e = JSONtoEvent(eventJSON);
  if(e==NULL)
  {
    return "{\"error\":\"Invalid Event Properties: could not add event.\"}";
  }
  else
  {
    // printf("%s\n",e->UID);
    strcpy(e->startDateTime.date,eventSD);
    strcpy(e->startDateTime.time,eventST);
    if(strcmp(eventUTC,"true") == 0)
    {
      e->startDateTime.UTC = true;
    }
    else
    {
      e->startDateTime.UTC = false;
    }

    strcpy(e->creationDateTime.date,eventCD);
    strcpy(e->creationDateTime.time,eventCT);
    e->creationDateTime.UTC = true;
    addEvent(c, e);
  }

  if(strlen(eventsummary) != 0)
  {
    p = malloc(sizeof(Property) + sizeof(eventsummary));
    strcpy(p->propName,"SUMMARY");
    strcpy(p->propDescr,eventsummary);
    addPropToEvent(e,p);
  }

  err =  writeCalendar(directory, c);
  if(err != OK)
  {
    sprintf(error,"{\"error\":\"%s\"}",printError(err));
    return error;
  }

  free(directory);
  free(error);

  return "{\"error\":\"Successfully Added Event...refresh to load file.\"}";
}

//CREATE-CALENDAR BACKEND*******************************************************************************************************************************
char* createCalendarWrapper(char* filename, char* calJSON,char* eventJSON,char* eventsummary,char* eventSD,char* eventST,char* eventUTC, char* eventCD, char* eventCT)
{
  Calendar *c;
  Event *e;
  Property *p;
  char* directory = malloc(200);
  char* error = malloc(100);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);


  c = JSONtoCalendar(calJSON);
  if(c==NULL)
  {
    return "{\"error\":\"Invalid Calendar Properties: could not create calendar.\"}";
  }
  else
  {
    // printf("%s,%lf\n",c->prodID,c->version);
  }

  e = JSONtoEvent(eventJSON);
  if(e==NULL)
  {
    return "{\"error\":\"Invalid Event Properties: could not create calendar.\"}";
  }
  else
  {
    // printf("%s\n",e->UID);
    strcpy(e->startDateTime.date,eventSD);
    strcpy(e->startDateTime.time,eventST);
    if(strcmp(eventUTC,"true") == 0)
    {
      e->startDateTime.UTC = true;
    }
    else
    {
      e->startDateTime.UTC = false;
    }

    strcpy(e->creationDateTime.date,eventCD);
    strcpy(e->creationDateTime.time,eventCT);
    e->creationDateTime.UTC = true;
    addEvent(c, e);
  }

  if(strlen(eventsummary) != 0)
  {
    p = malloc(sizeof(Property) + sizeof(eventsummary));
    strcpy(p->propName,"SUMMARY");
    strcpy(p->propDescr,eventsummary);
    addPropToEvent(e,p);
  }

  // ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
  ICalErrorCode err =  writeCalendar(directory, c);
  if(err != OK)
  {
    sprintf(error,"{\"error\":\"%s\"}",printError(err));
    return error;
  }


  // printf("%s %s %s %s %s %s %s\n",filename,calJSON,eventJSON,eventsummary,eventSD,eventST,eventUTC);
  free(directory);
  free(error);
  
  return "{\"error\":\"Successfully Created Calendar...refresh to load file.\"}";
}

void addPropToEvent(Event* event, Property* toBeAdded){

  if(event != NULL && toBeAdded != NULL)
  {
    insertBack( event->properties,(void*)toBeAdded);
  }
}

//PROPERTY BACKEND*******************************************************************************************************************************

char* propObjectToJSON(char* filename, int eventNum)
{
  char* directory = malloc(200);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);

  Calendar* c =NULL;

  ICalErrorCode err = createCalendar(directory, &c);

  if(err != OK)
  {
    return "[]";
  }

  err = validateCalendar(c);
  if(err != OK)
  {
    return "[]";
  }

  free(directory);

  void* elem;
  int i;

  i=1;
  ListIterator iter = createIterator((List*)(c->events));
  while ((elem = nextElement(&iter)) != NULL)
  {
    if(i==eventNum)
    {
      Event* tmpEvent = (Event*)elem;

      return propListToJSON(tmpEvent->properties);

    }
    i++;
  }

   return "[]";
}


char* propListToJSON(const List* propList){

  void* elem;
  char *tmpStr;
  int len;

  len = 50;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if (propList == NULL  )
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }
  if( propList->length ==0)
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }

  sprintf(tmpStr,"[");

  ListIterator iter = createIterator((List*)propList);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    char* strE = propToJSON(tmpProp);
    len = strlen(strE) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s,",strE);
    free(strE);
  }

  tmpStr[strlen(tmpStr) -1 ] = '\0';

  sprintf(tmpStr + strlen(tmpStr),"]");
  // printf("%s\n",tmpStr);
  return tmpStr;
}

char* propToJSON(const Property* prop){

  char* tmpStr;

  if(prop == NULL)
  {
    tmpStr = malloc(10);
    // strcpy(tmpStr,"\0");
    sprintf(tmpStr,"{}");
    return tmpStr;
  }
  tmpStr = (char*)malloc(sizeof(char)* strlen(prop->propDescr) +1000 );
  sprintf(tmpStr,"{\"propName\":\"%s\",\"propDescr\":\"%s\"}",prop->propName,prop->propDescr);

  return tmpStr;
}
//ALARM BACKEND**********************************************************************************************************************************


char* alarmObjectToJSON(char* filename, int eventNum)
{
  char* directory = malloc(200);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);

  Calendar* c =NULL;

  ICalErrorCode err = createCalendar(directory, &c);

  if(err != OK)
  {
    return "[]";
  }

  err = validateCalendar(c);
  if(err != OK)
  {
    return "[]";
  }

  free(directory);

  void* elem;
  int i;

  i=1;
  ListIterator iter = createIterator((List*)(c->events));
  while ((elem = nextElement(&iter)) != NULL)
  {
    if(i==eventNum)
    {
      Event* tmpEvent = (Event*)elem;

      return alarmListToJSON(tmpEvent->alarms);

    }
    i++;
  }

   return "[]";
}


char* alarmListToJSON(const List* alarmList){

  void* elem;
  char *tmpStr;
  int len;

  len = 50;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if (alarmList == NULL  )
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }
  if( alarmList->length ==0)
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }

  sprintf(tmpStr,"[");

  ListIterator iter = createIterator((List*)alarmList);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Alarm* tmpAlarm = (Alarm*)elem;
    char* strE = alarmToJSON(tmpAlarm);
    len = strlen(strE) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s,",strE);
    free(strE);
  }

  tmpStr[strlen(tmpStr) -1 ] = '\0';

  sprintf(tmpStr + strlen(tmpStr),"]");
  // printf("%s\n",tmpStr);
  return tmpStr;
}

char* alarmToJSON(const Alarm* alarm){

  char* tmpStr;

  if(alarm == NULL)
  {
    tmpStr = malloc(10);
    // strcpy(tmpStr,"\0");
    sprintf(tmpStr,"{}");
    return tmpStr;
  }
  tmpStr = (char*)malloc(sizeof(char)* strlen(alarm->trigger) +1000 );
  sprintf(tmpStr,"{\"action\":\"%s\",\"trigger\":\"%s\",\"numProps\":%d}",alarm->action,alarm->trigger,alarm->properties->length+2);

  return tmpStr;
}
//*****************************************************************************************************************************************
char* eventListWithFilenameToJSON(const List* eventList, char* filename){

  void* elem;
  char *tmpStr;
  int len;

  len = 50;
  tmpStr = (char*)malloc(sizeof(char)*len);

  if(eventList == NULL  )
  {
    sprintf(tmpStr,"[]");
    return tmpStr;

  }
  if(eventList->length ==0)
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }

  sprintf(tmpStr,"[{\"filename\":\"%s\"},",filename);

  ListIterator iter = createIterator((List*)eventList);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Event* tmpEvent = (Event*)elem;
    char* strE = eventToJSON(tmpEvent);
    len = strlen(strE) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s,",strE);
    free(strE);
  }

  tmpStr[strlen(tmpStr) -1 ] = '\0';

  sprintf(tmpStr + strlen(tmpStr),"]");
  // printf("%s\n",tmpStr);
  return tmpStr;
}

char* calendarWithFilenameToJSON(const Calendar* cal, char* filename){

  char *tmpStr;
  int len;


  len = 2000;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if (cal == NULL)
  {
    sprintf(tmpStr,"{}");
    return tmpStr;
  }

  sprintf(tmpStr,"{\"filename\":\"%s\",\"version\":%.0f,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",filename,cal->version,cal->prodID,cal->properties->length+2,cal->events->length );

  return tmpStr;
}


char* objectToJSON(char* filename)
{
  char* directory = malloc(200);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);

  Calendar* c =NULL;

  ICalErrorCode err = createCalendar(directory, &c);

  if(err != OK)
  {
    return "{}";
  }

  err = validateCalendar(c);
  if(err != OK)
  {
    return "{}";
  }

  free(directory);

  return calendarWithFilenameToJSON(c,filename);
}

char* eventObjectToJSON(char* filename)
{
  char* directory = malloc(200);

  strcpy(directory,"./uploads/");

  strcat(directory, filename);

  Calendar* c =NULL;

  ICalErrorCode err = createCalendar(directory, &c);
  if(err != OK)
  {
    return "[]";
  }

  err = validateCalendar(c);
  if(err != OK)
  {
    return "[]";
  }


  free(directory);

  return eventListWithFilenameToJSON(c->events,filename);
}


ICalErrorCode validateEvent(const Event* tmpEvent){
  void* elem;
  ICalErrorCode Err;
  // //event optional must not occur > 1
  bool class = false;
  bool created = false;
  bool description = false;
  bool geo = false;
  bool lastmod = false;
  bool location = false;
  bool organizer = false;
  bool priority = false;
  bool sequence = false;
  bool status = false;
  bool summary = false;
  bool transp = false;
  bool url = false;
  bool recurid = false;

  // //event optional
  // bool rrule = false;
  bool dtend = false;
  bool duration = false;
  // bool attach = false;
  // bool attendee = false;
  // bool categories = false;
  // bool comment = false;
  // bool contact = false;
  // bool exdate = false;
  // bool rstatus = false;
  // bool related = false;
  // bool resources = false;
  // bool rdate = false;

  if(strlen(tmpEvent->UID) <= 0 || strlen(tmpEvent->UID) >= 1000 )
  {
    return INV_EVENT;
  }
  if(strlen(tmpEvent->creationDateTime.date)!= 8 || strlen(tmpEvent->creationDateTime.time)!= 6 )
  {
    return INV_EVENT;
  }
  if(strlen(tmpEvent->startDateTime.date)!= 8 || strlen(tmpEvent->startDateTime.time)!= 6 )
  {
    return INV_EVENT;
  }
  if(tmpEvent->alarms == NULL )
  {
    return INV_EVENT;
  }
  if(tmpEvent->properties == NULL)
  {
    return INV_EVENT;
  }

  ListIterator iter = createIterator(tmpEvent->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    // printf("\n%s-%s\n",tmpProp->propName,tmpProp->propDescr );
    if(ignoreCaseCompare(tmpProp->propName,"ATTACH") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // attach = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"CATEGORIES") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // categories = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"CLASS") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(class == true)
      {
        return INV_EVENT;
      }
      class = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"COMMENT") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // comment = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"DESCRIPTION") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(description == true)
      {
        return INV_EVENT;
      }
      description = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"GEO") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(geo == true)
      {
        return INV_EVENT;
      }
      geo = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"LOCATION") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(location == true)
      {
        return INV_EVENT;
      }
      location = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"PRIORITY") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(priority == true)
      {
        return INV_EVENT;
      }
      priority = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"RESOURCES") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // resources = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"STATUS") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(status == true)
      {
        return INV_EVENT;
      }
      status = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"SUMMARY") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(summary == true)
      {
        return INV_EVENT;
      }
      summary = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"DTEND") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(duration == true)
      {
        return INV_EVENT;
      }
      dtend = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"DURATION") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(dtend == true)
      {
        return INV_EVENT;
      }
      duration = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"TRANSP") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(transp == true)
      {
        return INV_EVENT;
      }
      transp = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"ATTENDEE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // attendee = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"CONTACT") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // contact = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"ORGANIZER") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(organizer == true)
      {
        return INV_EVENT;
      }
      organizer = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"RELATED-TO") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // related = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"URL") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(url == true)
      {
        return INV_EVENT;
      }
      url = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"EXDATE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // exdate = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"RDATE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // rdate = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"RRULE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      // rrule = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"CREATED") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(created == true)
      {
        return INV_EVENT;
      }
      created = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"LAST-MODIFIED") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(lastmod == true)
      {
        return INV_EVENT;
      }
      lastmod = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"SEQUENCE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(sequence == true)
      {
        return INV_EVENT;
      }
      sequence = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"RECURRENCE-ID") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(recurid == true)
      {
        return INV_EVENT;
      }
      recurid = true;
    }
    else
    {
      return INV_EVENT;
    }
  }

  iter = createIterator(tmpEvent->alarms);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Alarm* tmpAlarm = (Alarm*)elem;

    Err = validateAlarm(tmpAlarm);

    if(Err != OK)
    {
      return Err;
    }

  }

  return OK;
}


ICalErrorCode validateAlarm(const Alarm* tmpAlarm){

  void* elem;
  bool repeat = false;
  bool duration = false;
  bool attach = false;

  if(tmpAlarm->trigger == NULL )
  {
    return INV_ALARM;
  }
  if(strlen(tmpAlarm->trigger) == 0)
  {
    return INV_ALARM;
  }
  if(strlen(tmpAlarm->action) >= 200 )
  {
    return INV_ALARM;
  }
  if(strlen(tmpAlarm->action) <= 0 )
  {
    return INV_ALARM;
  }
  if(tmpAlarm->properties == NULL)
  {
    return INV_ALARM;
  }

  ListIterator iter = createIterator(tmpAlarm->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    // printf("\n%s-%s\n",tmpProp->propName,tmpProp->propDescr );

    if(ignoreCaseCompare(tmpProp->propName,"ATTACH") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(attach == true)
      {
        return INV_ALARM;
      }
      attach = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"DURATION") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(duration == true)
      {
        return INV_ALARM;
      }
      duration = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"REPEAT") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(repeat == true)
      {
        return INV_ALARM;
      }
      repeat = true;
    }
    else
    {
      return INV_ALARM;
    }
  }

  if((repeat == true && duration == false) || (repeat == false && duration == true))
  {
    return INV_ALARM;
  }
  return OK;
}

/*****************************************************************************************/

void dateTimeParse(char* clDescription, char (*tempDate)[9], char (*tempTime)[7], bool* tempUTC)
{
  int i=0, j=0;
  //20151002T100000Z
  if(clDescription[strlen(clDescription)-1] == 'Z')
  {
    *tempUTC = true;
  }

  for(i=0;i<8;i++)
  {
    (*tempDate)[i] = clDescription[i];
    (*tempDate)[i+1]='\0';
  }

  j=0;
  for(i=9; i<15; i++)
  {
    (*tempTime)[j] = clDescription[i];
    (*tempTime)[j+1]='\0';
    j++;
  }
}
/***************************************************************/
Alarm* AlarmParse(char** charTable, char** clDescription, int i, int numLines)
{
  char tmpStr[200];
  Property* tempProp;
  Alarm* tempAlarm;

  tempAlarm = (Alarm*)malloc(sizeof(Alarm));
  tempAlarm->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);

  i = i+1;
  while(ignoreCaseCompare( charTable[i],"END:VALARM") != 0)
  {
      getName(charTable[i], &tmpStr);
      if(ignoreCaseCompare(tmpStr,"ACTION") == 0)
      {
        getPropVal(charTable[i],clDescription);
        strcpy(tempAlarm->action,*clDescription);
      }
      else if(ignoreCaseCompare(tmpStr,"TRIGGER")== 0)
      {
        getPropDescr(charTable[i], clDescription);
        tempAlarm->trigger = (char*)malloc(strlen(*clDescription)+100);
        strcpy(tempAlarm->trigger,*clDescription);
      }
      else if(ignoreCaseCompare(tmpStr,"COMMENT")!=0)
      {
        getPropDescr(charTable[i], clDescription);
        tempProp = (Property*)malloc(sizeof(Property)+strlen(*clDescription)+200);
        strcpy(tempProp->propName,tmpStr);
        strcpy(tempProp->propDescr, *clDescription);

        insertBack(tempAlarm->properties, (void*)tempProp);
      }
    i++;
  }
  return tempAlarm;

}

/*************************************************************************/
Event* EventParse(char** charTable, char** clDescription, int i, int numLines)
{

  char tmpStr[200];
  bool isAlarm = false;
  Event* tempEvent;
  char tempDate[9];
  char tempTime[7];
  bool tempUTC;
  Property* tempProp;
  Alarm* tempAlarm;



  tempEvent = (Event*)malloc(sizeof(Event));
/*
  strcpy(tempEvent->UID,"");
  strcpy(tempEvent->creationDateTime.date,"\0");
  strcpy(tempEvent->startDateTime.date,"\0");
  strcpy(tempEvent->creationDateTime.time,"\0");
  strcpy(tempEvent->startDateTime.time,"\0");
  tempEvent->startDateTime.UTC = false;
*/
  tempEvent->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  tempEvent->alarms = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);

  i = i+1;
  while(ignoreCaseCompare( charTable[i],"END:VEVENT") != 0)
  {


    if(ignoreCaseCompare( charTable[i],"BEGIN:VALARM") == 0)
    {
      //tempEvent = EventParse(charTable, clDescription, i);
      tempAlarm = AlarmParse(charTable, clDescription, i, numLines);
      insertBack(tempEvent->alarms,(void*)tempAlarm);
      isAlarm = true;
    }

    if(isAlarm==false)
    {

      getName(charTable[i], &tmpStr);
      if(ignoreCaseCompare(tmpStr,"UID") == 0)
      {
        getPropVal(charTable[i],clDescription);
        strcpy(tempEvent->UID,*clDescription);
      }
      else if(ignoreCaseCompare(tmpStr,"DTSTAMP")== 0)
      {
        //tempEvent->creationDateTime = (malloc(sizeof(DateTime)));
        getPropVal(charTable[i],clDescription);
        dateTimeParse( *clDescription, &tempDate, &tempTime, &tempUTC);
        strcpy((*tempEvent).creationDateTime.date,tempDate);
        strcpy((*tempEvent).creationDateTime.time,tempTime);
        (*tempEvent).creationDateTime.UTC = tempUTC;

      }
      else if(ignoreCaseCompare(tmpStr,"DTSTART")== 0)
      {
        //tempEvent->creationDateTime = (malloc(sizeof(DateTime)));
        getPropVal(charTable[i],clDescription);
        dateTimeParse( *clDescription, &tempDate, &tempTime, &tempUTC);
        strcpy((*tempEvent).startDateTime.date,tempDate);
        strcpy((*tempEvent).startDateTime.time,tempTime);
        (*tempEvent).startDateTime.UTC = tempUTC;

      }
      else if(ignoreCaseCompare(tmpStr,"COMMENT")!=0)
      {
        getPropDescr(charTable[i], clDescription);

        tempProp = (Property*)malloc(sizeof(Property)+strlen(*clDescription)+200);
        strcpy(tempProp->propName,tmpStr);
        strcpy(tempProp->propDescr, *clDescription);

        insertBack(tempEvent->properties, (void*)tempProp);
      }
    }
    if(ignoreCaseCompare( charTable[i],"END:VALARM") == 0)
    {
      isAlarm = false;
    }
    i++;
  }

  return tempEvent;

}


/*****************************************************************************/
void CalParse(Calendar** obj, char** charTable, int numLines, char** clDescription)
{
  Property* tempProp;
  Event* tempEvent;
  char tmpStr[200];
  int i;
  bool isEvent = false;

  (*obj)->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  (*obj)->events = initializeList(&printEvent,&deleteEvent,&compareEvents);

  //for(i=1; i<numLines-1; i++)
  i = 1;
  while(ignoreCaseCompare( charTable[i],"END:VCALENDAR") != 0)
  {
    if(ignoreCaseCompare( charTable[i],"BEGIN:VEVENT") == 0)
    {
      tempEvent = EventParse(charTable, clDescription, i, numLines);
      insertBack((*obj)->events,(void*)tempEvent);
      isEvent = true;
    }

    if(isEvent==false)
    {
      getName(charTable[i], &tmpStr);
      if(ignoreCaseCompare( tmpStr,"VERSION") !=0 && ignoreCaseCompare( tmpStr,"PRODID") !=0 && ignoreCaseCompare(tmpStr, "COMMENT") != 0)
      {
        getPropDescr(charTable[i], clDescription);

        tempProp = (Property*)malloc(sizeof(Property)+strlen(*clDescription)+200);
        strcpy(tempProp->propName,tmpStr);
        strcpy(tempProp->propDescr, *clDescription);

        insertBack((*obj)->properties, (void*)tempProp);
      }
    }
    if(ignoreCaseCompare( charTable[i],"END:VEVENT") == 0)
    {
      isEvent = false;
    }
    i++;

  }


  getPropVal(charTable[getIndex("VERSION", charTable, numLines)],clDescription);
  (*obj)->version = atof(*clDescription);
  getPropVal(charTable[getIndex("PRODID", charTable, numLines)],clDescription);
  strcpy((*obj)->prodID, *clDescription);


}

/*****************************************************************************/
int ignoreCaseCompare(char *a, char *b)
{
  int lenA=0;
  int lenB=0;
  int i;

  lenA = strlen(a);
  lenB= strlen(b);

  if(lenA != lenB)
  {
    return 1;
  }

  for (i =0; i<lenA; i++)
  {
    if(tolower(a[i]) != tolower(b[i]))
    {
      return 1;
    }
  }
  return 0;
}

/*****************************************************************************/

ICalErrorCode validArray(char** charTable, int numLines, char** clDescription)
{
  char clName[200];
  int i;
  bool version = false;
  bool prodID= false;
  bool dtStamp = false;
  bool uid = false;
  bool dtStart = false;
  bool trigger = false;
  bool action = false;
  bool eventBeginEnd = true;
  bool isEvent =false;
  bool calEnd = false;
  bool alarmBeginEnd = true;
  bool isAlarm =false;
  int eventCount = 0;

  if( ignoreCaseCompare(charTable[0], "BEGIN:VCALENDAR") !=0  )
  {
    return INV_CAL;
  }


  for(i=0;i<numLines;i++)
  {
    getName(charTable[i], &clName);
/*
     if(ignoreCaseCompare(clName,"INVALID") == 0)
     {
       return INV_CAL;
     }
     */
     if(i>0 && ignoreCaseCompare( charTable[i],"BEGIN:VCALENDAR") == 0)
     {

       return INV_CAL;

     }
     if( ignoreCaseCompare( clName,"PRODID") == 0 && isEvent==false )
     {
       if(prodID == true)
       {
         return DUP_PRODID;
       }
       getPropDescr(charTable[i],clDescription);
       if(strlen(*clDescription) == 0)
       {
         return INV_PRODID;
       }
       prodID=true;
     }
     if( ignoreCaseCompare( clName,"VERSION") == 0 && isEvent==false  )
     {
       if(version == true)
       {
         return DUP_VER;
       }
       getPropDescr(charTable[i],clDescription);
       if(atof(*clDescription)==0)
       {
         return INV_VER;
       }
       version=true;
     }
     if(ignoreCaseCompare(charTable[i],"BEGIN:VEVENT")==0 && isEvent ==false)
     {
       eventBeginEnd = false;
       isEvent = true;
       eventCount++;
     }
     else if(ignoreCaseCompare(charTable[i],"BEGIN:VEVENT")==0 && isEvent ==true)
     {
       return INV_EVENT;
     }

     if(ignoreCaseCompare(charTable[i],"END:VEVENT")==0 && isEvent==true)
     {
       eventBeginEnd = true;
       isEvent = false;
     }
     if(ignoreCaseCompare(charTable[i],"BEGIN:VALARM")==0 && isEvent ==true && isAlarm ==false)
     {
       alarmBeginEnd = false;
       isAlarm = true;
     }
     else if(ignoreCaseCompare(charTable[i],"BEGIN:VALARM")==0 && isEvent ==true && isAlarm ==true)
     {
       return INV_ALARM;
     }
     if(ignoreCaseCompare(charTable[i],"END:VALARM")==0 && isEvent==true && isAlarm==true)
     {
       alarmBeginEnd = true;
       isAlarm = false;
     }
     if(strcmp(clName,"INVALID")==0)
     {
       if(isAlarm==true)
       {

         return INV_ALARM;
       }
       else if(isEvent==true)
       {
         return INV_EVENT;
       }
       else
       {

         return INV_CAL;

       }
     }
     if(ignoreCaseCompare(charTable[i],"END:VCALENDAR")==0)
     {
       calEnd = true;
     }
  }
  if(alarmBeginEnd == false)
  {
    return INV_ALARM;
  }
  if(eventBeginEnd == false)
  {
    return INV_EVENT;
  }
  if(version == false || prodID == false || eventCount == 0 || calEnd==false)
  {
    return INV_CAL;

  }

  if(isAlarm == true && alarmBeginEnd == false)
  {
      return INV_FILE;
  }

 isAlarm = false;

  if(eventBeginEnd == true)
  {
    for(i=0;i<numLines;i++)
    {
      getName(charTable[i], &clName);

       if( ignoreCaseCompare(charTable[i],"BEGIN:VEVENT")==0 )
       {
         isEvent= true;
       }

       if(isEvent == true && ignoreCaseCompare( clName,"UID") == 0 )
       {
         if(uid == true)
         {
           return INV_EVENT;
         }
         getPropDescr(charTable[i],clDescription);
         if(strlen(*clDescription) == 0)
         {
           return INV_EVENT;
         }
         uid=true;
       }
       else if(isEvent == true && ignoreCaseCompare( clName,"DTSTAMP") == 0 )
       {
         if(dtStamp == true)
         {
           return INV_EVENT;
         }
         getPropDescr(charTable[i],clDescription);
         if(dtValidator(clDescription) == false)
         {
           return INV_DT;
         }
         dtStamp=true;
       }
       else if(isEvent == true && ignoreCaseCompare( clName,"DTSTART") == 0 )
       {
         if(dtStart == true)
         {
           return INV_EVENT;
         }
         getPropDescr(charTable[i],clDescription);
         if(dtValidator(clDescription) == false)
         {
           return INV_DT;
         }
         dtStart=true;
       }
       else if(isEvent == true && isAlarm == false)
       {
         getPropDescr(charTable[i],clDescription);
         if(strlen(clName) == 0 || strlen(*clDescription)== 0)
         {
           return INV_EVENT;
         }
       }

       if(ignoreCaseCompare(charTable[i],"END:VEVENT")==0)
       {
         isEvent = false;
         if(dtStart == true && dtStamp == true && uid == true)
         {
           dtStamp = false;
           dtStart = false;
           uid = false;
         }
         else
         {
           return INV_EVENT;
         }
       }
        if(ignoreCaseCompare(charTable[i],"BEGIN:VALARM")==0 && isEvent ==true && isAlarm ==false)
        {
            isAlarm = true;
        }


       if(isAlarm == true && ignoreCaseCompare( clName,"TRIGGER") == 0 )
       {
         if(trigger == true)
         {

           return INV_ALARM;
         }
         getPropDescr(charTable[i],clDescription);
         if(strlen(*clDescription) == 0)
         {

           return INV_ALARM;
         }
         trigger=true;
       }
       else if(isAlarm == true && ignoreCaseCompare( clName,"ACTION") == 0 )
       {
         if(action == true)
         {

           return INV_ALARM;
         }
         getPropDescr(charTable[i],clDescription);
         if(strlen(*clDescription) == 0)
         {

           return INV_ALARM;
         }
         action=true;
       }
       else if(isAlarm == true)
       {
         getPropDescr(charTable[i],clDescription);
         if(strlen(clName) == 0 || strlen(*clDescription)== 0)
         {
           return INV_ALARM;
         }
       }

       if(ignoreCaseCompare(charTable[i],"END:VALARM")==0 && isEvent==true && isAlarm==true)
       {
            isAlarm = false;
            if(trigger == true && action == true)
            {
                trigger = false;
                action = false;
            }
            else
            {
                return INV_ALARM;
            }
        }
     }
  }

  return OK;
}


/******************************************************************************/

bool dtValidator(char **clDescription)
{
  int i = 0;

  if( strlen(*clDescription) != 15 && strlen(*clDescription) != 16 )
  {
    return false;
  }
  if(strlen(*clDescription) == 16 )
  {
    if((*clDescription)[15]!= 'Z' && (*clDescription)[15]!= 'z' )
    {
      return false;
    }
  }
  if ((*clDescription)[8] != 'T' && (*clDescription)[8] != 't')
  {
    return false;
  }

   for(i=0;i<8;i++)
  {
    if((*clDescription)[i] !='0' && (*clDescription)[i] !='1' && (*clDescription)[i] !='2' && (*clDescription)[i] !='3' && (*clDescription)[i] !='4'&& (*clDescription)[i] !='5'&& (*clDescription)[i] !='6' && (*clDescription)[i] !='7' && (*clDescription)[i] !='8'&&(*clDescription)[i] !='9')
    {
      return false;
    }
  }
  for(i=9; i<15; i++)
  {
    if((*clDescription)[i] !='0' && (*clDescription)[i] !='1' && (*clDescription)[i] !='2' && (*clDescription)[i] !='3' && (*clDescription)[i] !='4'&& (*clDescription)[i] !='5'&& (*clDescription)[i] !='6' && (*clDescription)[i] !='7' && (*clDescription)[i] !='8'&&(*clDescription)[i] !='9')
    {

      return false;
    }
  }

  return true;

}
/*****************************************************************************/
int getIndex(char clName[200], char **charTable, int numLines)
{
  int i;
  char tempName[200];

  for(i=0;i<numLines;i++)
  {
    getName(charTable[i], &tempName);
     if( ignoreCaseCompare( tempName,clName) == 0 )
     {
       return i;
     }

  }

  return -1;
}

/*****************************************************************************/
//you could error check to see if the line contains a :
void getPropVal(char *charTable, char** clPropDescr)
{
  bool flip = false;
  int i,j;

  j=0;
  for(i=0;i<strlen(charTable);i++)
  {
    if(flip == true)
    {
      (*clPropDescr)[j] = charTable[i];
      j++;
    }
    else
    {
      if(charTable[i] ==':')
      {
        flip= true;
      }
    }
  }
  (*clPropDescr)[j] = '\0';


}
/*****************************************************************************/

void getPropDescr(char *charTable, char** clPropDescr)
{
  bool flip = false;
  int i,j;

  j=0;
  for(i=0;i<strlen(charTable);i++)
  {
    if(flip == true)
    {
      (*clPropDescr)[j] = charTable[i];
      j++;
    }
    else
    {
      if(charTable[i] ==':' || charTable[i] == ';')
      {
        flip= true;
      }
    }
  }
  (*clPropDescr)[j] = '\0';

}
/*****************************************************************************/
void getName(char* charTable, char(*clName)[200])
{
  int j,k;
  int stopLoop= 1;

    j=0;

    if(charTable[0]==';')
    {
      strcpy((*clName), "COMMENT");
      return;
    }

      stopLoop = 1;
      while (stopLoop)
      {
        if( charTable[j]==';' || charTable[j]==':'  )
        {
          for(k=0;k<j;k++)
          {
            (*clName)[k] = charTable[k];
          }
          (*clName)[j]='\0';
          stopLoop = 0;

        }
        else if(j== (strlen(charTable)-1))
        {
          strcpy((*clName), "INVALID");
          stopLoop = 0;
        }
        j++;
      }

}


/*****************************************************************************/
void ClfrTrimmer(char*** charTable,int numberOfLines, int maxNumberofChars)
{
  int i,j;

  for ( i = 0; i < numberOfLines; i++ )
  {
    for(j=0; j < maxNumberofChars; j++)
    {
      if( (*charTable)[i][j]=='\r')
      {
        if((*charTable)[i][j+1] == '\n')
        {
          (*charTable)[i][j] = '\0';
        }
      }
    }
  }
}


/*****************************************************************************/
void LineArrayParmCalc(char* fileName, int* maxLen, int* numLines)
{
  FILE *fVal = NULL;
  char character;
  int  numberOfChars = 0;

  fVal = fopen(fileName, "r");


  while(!feof(fVal))
  {
    character = fgetc(fVal);
     numberOfChars++;
     if( (character == '\r')  )
     {
       numberOfChars++;
       if((character = fgetc(fVal)) == '\n' )
       {
         numberOfChars++;
         character=fgetc(fVal);
         if(character != ' ' && character != '\t' )
         {
           *numLines = *numLines +1;

           if(*maxLen < numberOfChars)
           {
             *maxLen = numberOfChars+1;
           }
           numberOfChars=0;
         }
        }
      }

   }
   fclose(fVal);
 }

 /*****************************************************************************/

 bool fileCheck(char *fileName)
 {
   int len;

   if(fileName == NULL)
   {
     return false;
   }

   len = strlen(fileName);

   if(len == 0 || len< 5)
   {

     return false;
   }

   if(fileName[len-4] != '.' || fileName[len-3] != 'i' ||fileName[len-2] != 'c' || fileName[len-1] != 's' )
   {

     return false;
   }


   return true;
 }
