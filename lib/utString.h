#ifndef UTSTRING_H
#define UTSTRING_H

#include <stdarg.h>

enum utStringFlag
{
    USF_CONST = (1 << 0)     // utString does not own buffer
};

typedef struct utString
{
    char *buffer;
    int length;
    int capacity;
    int flags;
} utString;

utString * utStringCreate(int initialSize);
void utStringReserve(utString *s, int newSize);
void utStringDestroy(utString *s);

void utStringClear(utString *s);
void utStringSet(utString *s, const char *text);
void utStringSetK(utString *s, const char *text); // keeps text pointer!
void utStringConcat(utString *s, const char *text);
void utStringPrintf(utString *s, const char *format, ...);
void utStringConcatv(utString *s, const char *format, va_list args);
void utStringConcatf(utString *s, const char *format, ...);

const char *utStringSafe(utString *s);

#endif
