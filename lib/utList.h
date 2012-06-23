#ifndef UTLIST_H
#define UTLIST_H

#include "utArray.h"
#include "utString.h"

typedef enum utLineFlag
{
    LF_SELECTED = (1 << 0)
} utLineFlag;

typedef struct utLine
{
    utString text;
    int flags;
} utLine;

ARRAY_DECLARE(utLine);

utLine *utLineCreate(const char *text);
void utLineDestroy(utLine *line);

typedef enum utListSubsetType
{
    LST_NONE = 0,
    LST_ALL,
    LST_SELECTED,
    LST_INDICES,

    LST_COUNT
} utListSubsetType;

typedef struct utListSubset
{
    int type; // LST_*
    utIntArray indices;
} utListSubset;

extern const utListSubset LS_ALL[];

utListSubset *utListSubsetCreate(int type);
void utListSubsetDestroy(utListSubset *subset);

typedef struct utList
{
    utLineArray lines;
} utList;

typedef void (*utListWalkCB)(utList *list, int i, void *userData);

utList *utListCreate();
void utListDestroy(utList *list);
void utListPush(utList *list, const char *text);
void utListWalk(utList *list, const utListSubset *subset, utListWalkCB walkCB, void *userData);
void utListDelete(utList *list, const utListSubset *subset);

#endif
