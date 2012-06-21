#include "utContext.h"

ARRAY_IMPLEMENT(utHerp, free);

utContext *utContextCreate()
{
    utContext *context = (utContext *)calloc(1, sizeof(utContext));
    return context;
}

void utContextDestroy(utContext *context)
{
    free(context);
}

