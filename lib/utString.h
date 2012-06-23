#ifndef UTSTRING_H
#define UTSTRING_H

#include <stdarg.h>

#include "utArray.h"

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

ARRAY_DECLARE(utString);

utString * utStringCreate();
utString * utStringCreateFrom(const char *text);
utString * utStringCreateSubstr(utString *s, int start, int length);
void utStringReserve(utString *s, int newSize);
void utStringDestroy(utString *s);

void utStringClear(utString *s);
void utStringCopy(utString *s, utString *t);
void utStringSet(utString *s, const char *text);
void utStringSetK(utString *s, const char *text); // keeps text pointer!
void utStringConcat(utString *s, const char *text);
void utStringPrintf(utString *s, const char *format, ...);
void utStringConcatv(utString *s, const char *format, va_list args);
void utStringConcatf(utString *s, const char *format, ...);
utStringArray * utStringSplitQuoted(utString *s);

const char *utStringSafe(utString *s);

#endif
