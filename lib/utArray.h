#ifndef UTARRAY_H
#define UTARRAY_H

#include <stdlib.h>
#include <string.h>

typedef struct utIntArray
{
    int count;
    int capacity;
    int *data;
} utIntArray;

int utIntArrayPush(utIntArray *p, int v);
int utIntArrayPop(utIntArray *p);
void utIntArrayClear(utIntArray *p);

#define ARRAY_DECLARE(TYPE)                                                                \
                                                                                           \
typedef struct TYPE ## Array                                                               \
{                                                                                          \
    int count;                                                                             \
    int capacity;                                                                          \
    TYPE **data;                                                                           \
} TYPE ## Array;                                                                           \
                                                                                           \
TYPE ## Array * TYPE ## ArrayCreate();                                                     \
void TYPE ## ArrayDestroy(TYPE ## Array *p);                                               \
void TYPE ## ArrayClear(TYPE ## Array *p);                                                 \
int TYPE ## ArrayPush(TYPE ## Array *p, TYPE *v);                                          \
TYPE *TYPE ## ArrayPop(TYPE ## Array *p);                                                  \
TYPE *TYPE ## ArrayTop(TYPE ## Array *p);                                                  \
int TYPE ## ArrayCount(TYPE ## Array *p);                                                  \
void TYPE ## ArraySquash(TYPE ## Array *p);                                                \
void TYPE ## ArrayUnshift(TYPE ## Array *p, TYPE *v);                                      \
void TYPE ## ArrayInject(TYPE ## Array *p, TYPE *v, int n);                                \
void TYPE ## ArrayShrink(TYPE ## Array *p, int n);

#define ARRAY_IMPLEMENT(TYPE, DESTROY)                                                     \
                                                                                           \
TYPE ## Array * TYPE ## ArrayCreate()                                                      \
{                                                                                          \
    TYPE ## Array *array = ((TYPE ## Array*)calloc(1, sizeof(TYPE ## Array)));             \
    return array;                                                                          \
}                                                                                          \
                                                                                           \
void TYPE ## ArraySquash(TYPE ## Array *p)                                                 \
{                                                                                          \
    int head = 0;                                                                          \
    int tail = 0;                                                                          \
    while(tail < p->count)                                                                 \
    {                                                                                      \
        if(p->data[tail] != NULL)                                                          \
        {                                                                                  \
            p->data[head] = p->data[tail];                                                 \
            head++;                                                                        \
        }                                                                                  \
        tail++;                                                                            \
    }                                                                                      \
    p->count = head;                                                                       \
}                                                                                          \
                                                                                           \
void TYPE ## ArrayInject(TYPE ## Array *p, TYPE *v, int n)                                 \
{                                                                                          \
    int index = TYPE ## ArrayPush(p, v);                                                   \
    int injectIndex = index - n;                                                           \
    if(injectIndex < 0)                                                                    \
    {                                                                                      \
        injectIndex = 0;                                                                   \
    }                                                                                      \
    while(index > injectIndex)                                                             \
    {                                                                                      \
        p->data[index] = p->data[index - 1];                                               \
        index--;                                                                           \
    }                                                                                      \
    p->data[index] = v;                                                                    \
}                                                                                          \
                                                                                           \
void TYPE ## ArrayUnshift(TYPE ## Array *p, TYPE *v)                                       \
{                                                                                          \
    TYPE ## ArrayPush(p, NULL);                                                            \
    memmove(&p->data[1], &p->data[0], sizeof(void*) * (p->count - 1));                     \
    p->data[0] = v;                                                                        \
}                                                                                          \
                                                                                           \
void TYPE ## ArrayShrink(TYPE ## Array *p, int n)                                          \
{                                                                                          \
    while(p->count > n)                                                                    \
    {                                                                                      \
        DESTROY(p->data[p->count-1]);                                                      \
        p->count--;                                                                        \
    }                                                                                      \
}                                                                                          \
                                                                                           \
int TYPE ## ArrayPush(TYPE ## Array *p, TYPE *v)                                           \
{                                                                                          \
    if(p->count == p->capacity)                                                            \
    {                                                                                      \
        int newSize = (p->capacity) ? p->capacity * 2 : 100;                               \
        p->capacity = newSize;                                                             \
        p->data = realloc(p->data, p->capacity * sizeof(char **));                         \
    }                                                                                      \
    p->data[p->count++] = v;                                                               \
    return (int)(p->count - 1);                                                            \
}                                                                                          \
                                                                                           \
TYPE *TYPE ## ArrayPop(TYPE ## Array *p)                                                   \
{                                                                                          \
    if(!p->count)                                                                          \
        return NULL;                                                                       \
    return p->data[--p->count];                                                            \
}                                                                                          \
                                                                                           \
TYPE *TYPE ## ArrayTop(TYPE ## Array *p)                                                   \
{                                                                                          \
    if(!p->count)                                                                          \
        return NULL;                                                                       \
    return p->data[p->count - 1];                                                          \
}                                                                                          \
                                                                                           \
int TYPE ## ArrayCount(TYPE ## Array *p)                                                   \
{                                                                                          \
    return p->count;                                                                       \
}                                                                                          \
                                                                                           \
void TYPE ## ArrayClear(TYPE ## Array *p)                                                  \
{                                                                                          \
    int i;                                                                                 \
    for(i = 0; i < p->count; i++)                                                          \
        DESTROY(p->data[i]);                                                               \
    p->count = 0;                                                                          \
    if(p->data)                                                                            \
        free(p->data);                                                                     \
    p->data = NULL;                                                                        \
    p->capacity = 0;                                                                       \
}                                                                                          \
                                                                                           \
void TYPE ## ArrayDestroy(TYPE ## Array *p)                                                \
{                                                                                          \
    TYPE ## ArrayClear(p);                                                                 \
    free(p);                                                                               \
}

#endif
