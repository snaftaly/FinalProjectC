#ifndef ERRHANDLE_H_
#define ERRHANDLE_H_

/*** includes ***/
#include <stdio.h>
#include <string.h>

/*** externs ***/
extern int isError; /* used by all project to indicate an error */

/*** functions declerations ***/
void perrorPrint(char * command);
void sdlErrorPrint(char * sdlProblem);
void generalErrorprint(char * errorStr);

#endif /* ERRHANDLE_H_ */

