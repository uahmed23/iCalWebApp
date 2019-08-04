/*
Name: Umer Ahmed
ID: 1010091
Date: Febuary 4th 2019
Assignment 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <stddef.h>

#include "HelperFunctions.h"
#include "CalendarParser.h"
#include "LinkedListAPI.h"


/** Function to converting a JSON string into a Calendar struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Calendar struct
 *@param str - a pointer to a string
 **/
Calendar* JSONtoCalendar(const char* str){

  Calendar *obj = NULL ;
  char tmpStr[2000];
  int i,j;
  float version;
  char prodID[1000];

  if(str == NULL)
  {
    return NULL;
  }
  if(strlen(str) == 0 || strlen(str) >= 2000 || str[0] != '{' ||  str[strlen(str)-1] != '}' )
  {
    return NULL;
  }
// {"version":verVal,"prodID":"prodIDVal"}
  // tmpStr[0]= str[11];
  // tmpStr[1]= '\0';
  i=11;
  j=0;
  while(str[i]!= ',' && str[i]!='}')
  {
    tmpStr[j]=str[i];
    tmpStr[j+1] = '\0';
    j++;
    i++;
  }
  version = atof(tmpStr);

  // printf("%s %lf\n","version", version );

  if(version == 0)
  {
    return NULL;
  }
  i=i+11;
  j=0;
  while(str[i]!= '\"' && str[i]!='}')
  {
    tmpStr[j]=str[i];
    tmpStr[j+1] = '\0';
    j++;
    i++;
  }
  if(str[i]== '}')
  {
    return NULL;
  }
  strcpy(prodID,tmpStr);
  // printf("%s\n",prodID );


  if( (obj = malloc(sizeof(Calendar))) == NULL )
  {
    return NULL;
  }
  obj->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
  obj->events = initializeList(&printEvent,&deleteEvent,&compareEvents);

  obj->version = version;
  strcpy(obj->prodID,prodID);


  return obj;


}

/** Function to converting a JSON string into an Event struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Event struct
 *@param str - a pointer to a string
 **/
Event* JSONtoEvent(const char* str){

    Event *obj = NULL ;
    char tmpStr[2000];
    int i,j;
    char UID[1000];

    if(str == NULL)
    {
      return NULL;
    }
    if(strlen(str) == 0 || strlen(str) >= 2000 || str[0] != '{' ||  str[strlen(str)-1] != '}' )
    {
      return NULL;
    }

    i=8;
    j=0;
    while(str[i]!= '\"' && str[i]!='}')
    {
      tmpStr[j]=str[i];
      tmpStr[j+1] = '\0';
      j++;
      i++;
    }
    if(str[i]== '}')
    {
      return NULL;
    }
    strcpy(UID,tmpStr);

    if( (obj = malloc(sizeof(Event))) == NULL )
    {
      return NULL;
    }
    obj->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
    obj->alarms = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);

    strcpy(obj->UID,UID);


    return obj;

}

/** Function to adding an Event struct to an ixisting Calendar struct
 *@pre arguments are not NULL
 *@post The new event has been added to the calendar's events list
 *@return N/A
 *@param cal - a Calendar struct
 *@param toBeAdded - an Event struct
 **/

void addEvent(Calendar* cal, Event* toBeAdded){

  if(cal != NULL && toBeAdded != NULL)
  {
    insertBack( cal->events,(void*)toBeAdded);
  }
}

/** Function to converting a Calendar into a JSON string
 *@pre Calendar is not NULL
 *@post Calendar has not been modified in any way
 *@return A string in JSON format
 *@param cal - a pointer to a Calendar struct
 **/
char* calendarToJSON(const Calendar* cal){

  char *tmpStr;
  int len;


  len = 2000;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if (cal == NULL)
  {
    sprintf(tmpStr,"{}");
    return tmpStr;
  }

  sprintf(tmpStr,"{\"version\":%.0f,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",cal->version,cal->prodID,cal->properties->length+2,cal->events->length );

  return tmpStr;
}


/** Function to converting an Event list into a JSON string
 *@pre Event list is not NULL
 *@post Event list has not been modified in any way
 *@return A string in JSON format
 *@param eventList - a pointer to an Event list
 **/
char* eventListToJSON(const List* eventList){

  void* elem;
  char *tmpStr;
  int len;

  len = 50;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if (eventList == NULL || eventList->length ==0 )
  {
    sprintf(tmpStr,"[]");
    return tmpStr;
  }

  sprintf(tmpStr,"[");

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
  printf("%s\n",tmpStr);
  return tmpStr;
}

/** Function to converting an Event into a JSON string
 *@pre Event is not NULL
 *@post Event has not been modified in any way
 *@return A string in JSON format
 *@param event - a pointer to an Event struct
 **/
char* eventToJSON(const Event* event){
  char* tmpStr;
  char* jsonDt;
  char* summaryStr;
  bool summary = false;
  int len;
  void* elem;


  if (event == NULL)
  {
    tmpStr = malloc(10);
    // strcpy(tmpStr,"\0");
    sprintf(tmpStr,"{}");
    return tmpStr;
  }

  ListIterator iter = createIterator(event->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    if(ignoreCaseCompare(tmpProp->propName,"SUMMARY") == 0  )
    {
      summaryStr = (char*)malloc(sizeof(char)* strlen(tmpProp->propDescr) +11 );

      strcpy(summaryStr,tmpProp->propDescr);
      summary = true;
    }

  }
  if(summary == false)
  {
    summaryStr = (char*)malloc(sizeof(char)*10);
    strcpy(summaryStr,"");
  }

  len = strlen(summaryStr)+1000;
  tmpStr = (char*)malloc(sizeof(char)*len);

  // obj->events->length == 0
  // {"startDT":%s,"numProps":4,"numAlarms":2,"summary":"Do taxes"}
  // {"startDT":{"date":"19540203","time":"123012","isUTC":true},"numProps":6,"numAlarms":0,"summary":""}

  jsonDt = dtToJSON(event->startDateTime);


  sprintf(tmpStr,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\"}",jsonDt,event->properties->length+3,event->alarms->length,summaryStr );

  free(summaryStr);
  free(jsonDt);
  return tmpStr;
}

/** Function to converting a DateTime into a JSON string
 *@pre N/A
 *@post DateTime has not been modified in any way
 *@return A string in JSON format
 *@param prop - a DateTime struct
 **/
char* dtToJSON(DateTime prop){

  	char* jsonStr;
    int len;

  	len = strlen(prop.date)+strlen(prop.time)+48;
  	jsonStr = (char*)malloc(sizeof(char)*len);

    if(prop.UTC == true)
    {
  	   sprintf(jsonStr,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":true}", prop.date, prop.time);
    }
    else
    {
      sprintf(jsonStr,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":false}", prop.date, prop.time);
    }
  	return jsonStr;
}


/** Function to writing a Calendar object into a file in iCalendar format.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a file representing the
        Calendar contents in iCalendar format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{
  FILE *fp = NULL;
  // char *temp;
  void* elem;

  if(obj == NULL)
  {
    return WRITE_ERROR;
  }
  if(fileCheck(fileName)==false)
  {
    return WRITE_ERROR;
  }

  fp = fopen(fileName, "w");
  if (fp==NULL)
  {
    return WRITE_ERROR;
  }
  //sprintf(buffer,"BEGIN:VCALENDAR\nPRODID:%s\nVERSION:%.1f\n",obj->prodID, obj->version);
  fprintf(fp,"BEGIN:VCALENDAR\r\nPRODID:%s\r\nVERSION:%.1f\r\n",obj->prodID, obj->version);
  ListIterator iter = createIterator(obj->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    char* strP = obj->properties->printData(tmpProp);
    fprintf(fp,"%s\r\n",strP);
    free(strP);
  }

  iter = createIterator(obj->events);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Event* tmpEvent = (Event*)elem;
    char* strE = obj->events->printData(tmpEvent);
    fprintf(fp,"%s\r\n",strE);
    free(strE);
  }
  //

  fprintf(fp,"END:VCALENDAR\r\n");

  fclose(fp);

  return OK;
}


/** Function to validating an existing a Calendar object
 *@pre Calendar object exists and is not null
 *@post Calendar has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode validateCalendar(const Calendar* obj)
{
  void* elem;
  bool calscale = false;
  bool method = false;
  ICalErrorCode Err;

  /******************CALENDAR VALIDATION************************/
  if(obj == NULL)
  {
    return INV_CAL;
  }
  if(obj->version == 0)
  {
    return INV_CAL;
  }
  if(strlen(obj->prodID) <= 0 || strlen(obj->prodID) >= 1000)
  {
    return INV_CAL;
  }
  if(obj->events == NULL )
  {
    return INV_CAL;
  }
  if(obj->events->length == 0 )
  {
    return INV_CAL;
  }
  if(obj->properties == NULL)
  {
    return INV_CAL;
  }

  ListIterator iter = createIterator(obj->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    //printf("%s-%s\n",tmpProp->propName,tmpProp->propDescr );
    if(ignoreCaseCompare(tmpProp->propName,"CALSCALE") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(calscale == true)
      {
        return INV_CAL;
      }
      calscale = true;
    }
    else if(ignoreCaseCompare(tmpProp->propName,"METHOD") == 0 && strlen(tmpProp->propDescr) != 0 )
    {
      if(method == true)
      {
        return INV_CAL;
      }
      method = true;
    }
    else
    {
      return INV_CAL;
    }
  }

  /*********************EVENT VALIDATION************************/

  iter = createIterator(obj->events);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Event* tmpEvent = (Event*)elem;

    Err = validateEvent(tmpEvent);

    if(Err != OK)
    {
      return Err;
    }

  }


  return OK;
}




/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string contaning a humanly readable representation of the error code by indexing into
          the descr array using rhe error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err)
{
  char* tempChar;
  tempChar = malloc(1000);

  if(err== INV_FILE)
  {
    strcpy(tempChar, "File is invalid.");
    return tempChar;
  }
  else if(err == INV_CAL)
  {
    strcpy(tempChar,"Calendar is invalid. (no: opening & closing tags/proID/version/events)");
    return tempChar;
  }
  else if(err == INV_VER)
  {
    strcpy(tempChar,"Invalid Calendar Version. (calendar version property is present, but malformed (i.e. property value is missing or is not a number))  ");
    return tempChar;
  }
  else if (err == DUP_VER)
  {
      strcpy(tempChar,"Duplicate Calendar Version property.");
      return tempChar;
  }
  else if(err == INV_PRODID)
  {
    strcpy(tempChar,"Invalid Calendar Product ID. (calendar version property is present, but malformed (i.e. property value is missing))  ");
    return tempChar;
  }
  else if (err == DUP_PRODID)
  {
      strcpy(tempChar,"Duplicate Calendar Product ID property.");
      return tempChar;
  }
  else if (err == INV_EVENT)
  {
    strcpy(tempChar,"Invalid Event component. (missing closing tag/ missing required properties/ UID is present, but malformed (missing value)");
    return tempChar;
  }
  else if (err == INV_DT)
  {
    strcpy(tempChar,"Invalid DateTime Property.(do not conform to the  FORM #1 or  FORM #2 in Section 3.3.5 of the RFC5545)");
    return tempChar;
  }
  else if(err == INV_ALARM)
  {
    strcpy(tempChar,"Invalid Alarm Component.(i.e. missing or malformed required properties / missing closing tag  )");
    return tempChar;
  }
  else if(err == OTHER_ERROR)
  {
    strcpy(tempChar,"non-calendar relate error. (e.g. malloc returns NULL)");
    return tempChar;
  }
  else if(err == WRITE_ERROR)
  {
    strcpy(tempChar,"Write Error.");
    return tempChar;
  }

  strcpy(tempChar, "File is OK.");
  return tempChar;


}


ICalErrorCode createCalendar(char* fileName, Calendar** obj){


  FILE *fp = NULL;
  // FILE *fTemp =NULL;
  char character;
  int i,j;
  char** charTable =NULL;
  //char clName[200];
  //int len
  char *clDescription;
  bool firstChar= false;
  int numberOfLines = 0, maxNumberofChars =0;
  ICalErrorCode tempError;

  *obj = NULL;

  if(fileCheck(fileName)==false)
  {
    return INV_FILE;
  }

  fp = fopen(fileName, "r");
  // fTemp = fopen(fileName, "r");

  if (fp==NULL)
  {
    return INV_FILE;
  }

  LineArrayParmCalc(fileName,&maxNumberofChars,&numberOfLines);

  if(numberOfLines < 5)
  {
    fclose(fp);
    return INV_CAL;
  }
  //
  // printf("****************************************************\n" );
  // printf("number of total lines = %d, most characters in a line= %d \n",numberOfLines,maxNumberofChars);
  // printf("****************************************************\n" );
  // printf("print straight from file\n" );
  // printf("****************************************************\n" );
  // do
  // {
  //   character = fgetc(fTemp);
  //   if( feof(fTemp) )
  //   {
  //      break ;
  //   }
  //
  //   printf("%c", character);
  //
  // } while(1);
  // printf("****************************************************\n" );


  /***********ALLOCATING MEMORY FOR 2D ARRAY AND STORING THE FILE INTO IT******/

 if( (clDescription = malloc(maxNumberofChars +1 ) ) == NULL)
 {
   fclose(fp);
   return OTHER_ERROR;
 }
 for(j=0;j<maxNumberofChars;j++)
 {
   clDescription[j]='\0';
 }

  if (( charTable = malloc( numberOfLines*sizeof( char* ) +1) ) == NULL )
  {
    fclose(fp);
    free(clDescription);
    return OTHER_ERROR;
  }
  else
  {
    for(i=0; i< numberOfLines; i++)
    {
      charTable[i]=NULL;
    }
  }

  for ( i = 0; i < numberOfLines; i++ )
  {
    if (( charTable[i] = malloc( maxNumberofChars)) == NULL )
    {
      fclose(fp);
      free(clDescription);
      free(charTable);
      return OTHER_ERROR;
    }
    for(j=0;j<maxNumberofChars;j++)
    {
      charTable[i][j]='\0';
    }

    if(firstChar == true)
    {
      charTable[i][0]= character;
      j=1;
      firstChar =false;
    }
    else
    {
      j=0;
    }

    while( !feof(fp))//(character = fgetc(fp)) != EOF)
    {
      character = fgetc(fp);
      if( (character == '\r')  )
      {
        if((character = fgetc(fp)) == '\n' )
        {
          character = fgetc(fp);
          if(character==' ' || character=='\t')
          {


          }
          else
          {
            charTable[i][j]='\r';
            charTable[i][j+1]='\n';
            firstChar = true;
            //charTable[i+1][0]=character;
            //j=j+3;
            break;
          }
        }
        else
        {
          charTable[i][j]='\r';
          charTable[i][j+1]= character;
          j=j+2;
        }
      }
      else
      {
        charTable[i][j] = character;
        j++;
      }
    }
  }

  //fclose(fTemp);
  fclose(fp);
  ClfrTrimmer(&charTable,numberOfLines, maxNumberofChars);
//
//   printf("****************************************************\n" );
//   printf("print from array\n" );
//   printf("****************************************************\n" );
//
//   for ( i = 0; i < numberOfLines; i++ )
//   {
//       printf("%s\n",charTable[i] );
//       // getName(charTable[i], &clName);
//       // len = strlen(charTable[i]);
//       // printf("Name = %s\nlength = %d\n", clName, len );
//   }
//
//
// printf("****************************************************\n" );

tempError = validArray(charTable, numberOfLines, &clDescription);
if(tempError != OK)
{

  free(clDescription);
  for ( i = 0; i < numberOfLines; i++ )
  {
    free(charTable[i]);
  }
  free(charTable);

  return tempError;
}

/******************************************************************************/
//MALLOC Calendar Obj
/******************************************************************************/

  if( (*obj = malloc(sizeof(Calendar))) == NULL )
  {
    return OTHER_ERROR;
  }

  CalParse(obj,charTable, numberOfLines, &clDescription);

  /***************************FREEING 2D ARRAY********************************/
  free(clDescription);
  for ( i = 0; i < numberOfLines; i++ )
  {
    free(charTable[i]);
  }
  free(charTable);
/*****************************************************************************/
  return OK;
}

void deleteCalendar(Calendar* obj)
{
  if(obj == NULL)
  {
    return;
  }
  freeList(obj->properties);
  freeList(obj->events);
  free(obj);
}

char* printCalendar(const Calendar* obj)
{
  char *buffer;
  int len;
  void* elem;

  if(obj == NULL)
  {
    return NULL;
  }

  len = strlen(obj->prodID)+500;
	buffer = (char*)malloc(sizeof(char)*len);
  sprintf(buffer,"BEGIN:VCALENDAR\r\nPRODID:%s\r\nVERSION:%.1f\r\n",obj->prodID, obj->version);


  ListIterator iter = createIterator(obj->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    char* strP = obj->properties->printData(tmpProp);
    len = strlen(strP) + len;
    buffer = realloc(buffer, len);
    sprintf(buffer + strlen(buffer),"%s\r\n",strP);
    free(strP);
  }

  // char* listDescr = toString(obj->properties);
	// printf("After removal, the list is %s\n", listDescr);
	// free(listDescr);

  iter = createIterator(obj->events);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Event* tmpEvent = (Event*)elem;
    char* strE = obj->events->printData(tmpEvent);
    len = strlen(strE) + len;
    buffer = realloc(buffer, len);
    sprintf(buffer + strlen(buffer),"%s\r\n",strE);
    free(strE);
  }

  sprintf(buffer + strlen(buffer),"END:VCALENDAR\r\n");

  return buffer;

}

// ************* List helper functions - MUST be implemented ***************
void deleteProperty(void* toBeDeleted)
{
  	Property* tmpProp;

  	if (toBeDeleted == NULL){
  		return;
  	}

  	tmpProp = (Property*)toBeDeleted;

  	free(tmpProp);
}

int compareProperties(const void* first, const void* second){

  Property* tmpProp1;
  Property* tmpProp2;

  if (first == NULL || second == NULL){
    return 0;
  }

  tmpProp1 = (Property*)first;
  tmpProp2 = (Property*)second;

  return strcmp((char*)tmpProp1->propName, (char*)tmpProp2->propName);
}

char* printProperty(void* toBePrinted){

	char* tmpStr;
  Property* tmpProp;
  int len;

	if (toBePrinted == NULL){
		return NULL;
	}

	tmpProp = (Property*)toBePrinted;

	len = strlen(tmpProp->propName)+strlen(tmpProp->propDescr)+10;
	tmpStr = (char*)malloc(sizeof(char)*len);

	sprintf(tmpStr,"%s:%s", tmpProp->propName, tmpProp->propDescr);

	return tmpStr;
}

// **************************************************************************

void deleteEvent(void* toBeDeleted)
{
  	Event* tempEvent;

  	if (toBeDeleted == NULL){
  		return;
  	}

  	tempEvent = (Event*)toBeDeleted;

    freeList(tempEvent->properties);
    freeList(tempEvent->alarms);

  	free(tempEvent);
}

int compareEvents(const void* first, const void* second){

  Event* tmpEvent1;
  Event* tmpEvent2;

  if (first == NULL || second == NULL){
    return 0;
  }

  tmpEvent1 = (Event*)first;
  tmpEvent2 = (Event*)second;

  return strcmp((char*)tmpEvent1->UID, (char*)tmpEvent2->UID);
}

char* printEvent(void* toBePrinted){

	char* tmpStr;
	//Name* tmpName;
  Event* tmpEvent;
  int len;
  void* elem;

	if (toBePrinted == NULL){
		return NULL;
	}

	tmpEvent = (Event*)toBePrinted;

	len = strlen(tmpEvent->UID)+500;
	tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr,"BEGIN:VEVENT\r\nUID:%s\r\n",tmpEvent->UID );
//FIIXIIXIXIIT
  char* strDt = printDate(&(tmpEvent->creationDateTime));
  len = strlen(strDt) + len;
  tmpStr = realloc(tmpStr, len);
  sprintf(tmpStr + strlen(tmpStr), "DTSTAMP:%s\r\n",strDt);
  free(strDt);

  char* strDt2 = printDate(&(tmpEvent->startDateTime));
  len = strlen(strDt2) + len;
  tmpStr = realloc(tmpStr, len);
  sprintf(tmpStr + strlen(tmpStr), "DTSTART:%s\r\n",strDt2);
  free(strDt2);

  ListIterator iter = createIterator(tmpEvent->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    char* strP = tmpEvent->properties->printData(tmpProp);
    len = strlen(strP) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s\r\n",strP);
    free(strP);
  }

  iter = createIterator(tmpEvent->alarms);
  while((elem = nextElement(&iter)) != NULL)
  {
    Alarm* tmpAlarm = (Alarm*)elem;
    char* strA= tmpEvent->alarms->printData(tmpAlarm);
    len = strlen(strA) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s\r\n",strA);
    free(strA);
  }

  sprintf(tmpStr + strlen(tmpStr), "END:VEVENT");

  return tmpStr;
}

// **************************************************************************
void deleteDate(void* toBeDeleted)
{
  	DateTime* tmpDt;

  	if (toBeDeleted == NULL){
  		return;
  	}

  	tmpDt = (DateTime*)toBeDeleted;

  	free(tmpDt);
}

int compareDates(const void* first, const void* second){

  DateTime* tmpDt1;
  DateTime* tmpDt2;

  if (first == NULL || second == NULL){
    return 0;
  }

  tmpDt1 = (DateTime*)first;
  tmpDt2 = (DateTime*)second;

  return strcmp((char*)tmpDt1->date, (char*)tmpDt2->date);
}

char* printDate(void* toBePrinted){

	char* tmpStr;
	//Name* tmpName;
  DateTime* tmpDt;
  int len;

	if (toBePrinted == NULL){
		return NULL;
	}

	tmpDt = (DateTime*)toBePrinted;

	len = strlen(tmpDt->date)+strlen(tmpDt->time)+10;
	tmpStr = (char*)malloc(sizeof(char)*len);

  if(tmpDt->UTC == true)
  {
	   sprintf(tmpStr,"%sT%sZ", tmpDt->date, tmpDt->time);
  }
  else
  {
    sprintf(tmpStr,"%sT%s", tmpDt->date, tmpDt->time);
  }
	return tmpStr;
}

void deleteAlarm(void* toBeDeleted)
{
  Alarm* tmpAlarm;

  if (toBeDeleted == NULL){
    return;
  }

  tmpAlarm = (Alarm*)toBeDeleted;

  freeList(tmpAlarm->properties);
  free(tmpAlarm->trigger);
  free(tmpAlarm);
}
int compareAlarms(const void* first, const void* second)
{
  Alarm* tmpAlarm1;
  Alarm* tmpAlarm2;

  if (first == NULL || second == NULL){
    return 0;
  }

  tmpAlarm1 = (Alarm*)first;
  tmpAlarm2 = (Alarm*)second;

  return strcmp((char*)tmpAlarm1->action, (char*)tmpAlarm2->action);
}
char* printAlarm(void* toBePrinted)
{
  char* tmpStr;
  Alarm* tmpAlarm;
  int len;
  void* elem;


  if(toBePrinted == NULL){
    return NULL;
  }

  tmpAlarm = (Alarm*)toBePrinted;

  len = strlen(tmpAlarm->action)+strlen(tmpAlarm->trigger) + 500;
  tmpStr = (char*)malloc(sizeof(char)*len);

  sprintf(tmpStr,"BEGIN:VALARM\r\nACTION:%s\r\nTRIGGER:%s\r\n",tmpAlarm->action,tmpAlarm->trigger);

  ListIterator iter = createIterator(tmpAlarm->properties);
  while ((elem = nextElement(&iter)) != NULL)
  {
    Property* tmpProp = (Property*)elem;
    char* str = tmpAlarm->properties->printData(tmpProp);
    len = strlen(str) + len;
    tmpStr = realloc(tmpStr, len);
    sprintf(tmpStr + strlen(tmpStr),"%s\r\n",str);
    free(str);
  }

  sprintf(tmpStr + strlen(tmpStr),"END:VALARM");

  return tmpStr;

}
