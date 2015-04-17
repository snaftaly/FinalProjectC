#ifndef ERRHANDLE_H_
#define ERRHANDLE_H_

#include <stdio.h>
#include <string.h>

extern int isError;

void perrorPrint(char * command);
void sdlErrorPrint(char * sdlProblem);
void generalErrorprint(char * errorStr);

#endif /* ERRHANDLE_H_ */

