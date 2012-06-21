#ifndef UTCONTEXT_H
#define UTCONTEXT_H

#include "utList.h"

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
