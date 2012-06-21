#ifndef UTLIST_H
#define UTLIST_H

#include "utArray.h"
#include "utString.h"

typedef struct utLine
{
    utString text;
} utLine;

ARRAY_DECLARE(utLine);

utLine *utLineCreate(const char *text);
void utLineDestroy(utLine *line);

typedef struct utList
{
    utLineArray lines;
} utList;

utList *utListCreate();
void utListDestroy(utList *list);
void utListPush(utList *list, const char *text);

#endif
