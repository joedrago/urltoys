#ifndef UTCONTEXT_H
#define UTCONTEXT_H

#include "utList.h"

typedef struct utCommand
{
    utString line;
    utString name;
    utStringArray args;
} utCommand;

utCommand *utCommandCreate();
void utCommandDestroy(utCommand *command);
int utCommandParse(utCommand *context, char *text);

typedef void (*utOutputCB)(const char *line);

typedef struct utContext
{
    utList *current;
    utOutputCB outputCB;
} utContext;

utContext *utContextCreate();
void utContextDestroy(utContext *context);
int utContextParse(utContext *context, char *text);

#endif
