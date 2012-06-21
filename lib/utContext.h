#ifndef UTCONTEXT_H
#define UTCONTEXT_H

#include "utArray.h"

typedef struct utHerp
{
    int a;
} utHerp;

ARRAY_DECLARE(utHerp);

typedef struct utContext
{
    utHerpArray herps;
} utContext;

utContext *utContextCreate();
void utContextDestroy(utContext *context);

#endif
