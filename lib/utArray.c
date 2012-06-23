#include "utArray.h"

int utIntArrayPush(utIntArray *p, int v)
{
    if(p->count == p->capacity)
    {
        int newSize = (p->capacity) ? p->capacity * 2 : 2;
        p->capacity = newSize;
        p->data = realloc(p->data, p->capacity * sizeof(int));
    }
    p->data[p->count++] = v;
    return (p->count - 1);
}

int utIntArrayPop(utIntArray *p)
{
    if(p->count == 0)
        return -1;

    return p->data[--p->count];
}

void utIntArrayClear(utIntArray *p)
{
    p->count = 0;
    if(p->data)
        free(p->data);
    p->data = NULL;
    p->capacity = 0;
}

