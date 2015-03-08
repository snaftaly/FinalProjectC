#ifndef ERRHANDLE_H_
#define ERRHANDLE_H_

extern int isError;
extern int isQuit;

void perrorPrint(char * command);
void sdlErrorPrint(char * sdlProblem);

#endif /* ERRHANDLE_H_ */

