#ifndef ERRHANDLE_H_
#define ERRHANDLE_H_

#include <stdio.h>
#include <string.h>

extern int isError;
extern int isQuit;

void perrorPrint(char * command);
void sdlErrorPrint(char * sdlProblem);

#endif /* ERRHANDLE_H_ */

