#include "utString.h"

#include <stdlib.h>
#include <string.h>

utString * utStringCreate(int initialSize)
{
    utString *s = (utString *)calloc(1, sizeof(utString));
    utStringReserve(s, initialSize);
    return s;
}

void utStringReserve(utString *s, int newSize)
{
    char *originalBuffer = s->buffer;
    if(newSize <= s->capacity)
        return;

    s->buffer = malloc(sizeof(char) * (newSize + 1));
    if(s->length)
        memcpy(s->buffer, originalBuffer, s->length + 1);
    else
        s->buffer[0] = 0;
    if(!(s->flags & USF_CONST))
        free(originalBuffer);

    s->capacity = newSize;
}

void utStringDestroy(utString *s)
{
    utStringClear(s);
    free(s);
}

void utStringClear(utString *s)
{
    if(s->buffer && !(s->flags & USF_CONST))
        free(s->buffer);
    s->buffer = NULL;
    s->length = 0;
}

void utStringSet(utString *s, const char *text)
{
    int len = (int)strlen(text);
    utStringReserve(s, len);
    strcpy(s->buffer, text);
    s->length = len;
}

void utStringSetK(utString *s, const char *text)
{
    utStringClear(s);
    s->buffer = (char *)text;
    s->length = (int)strlen(text);
    s->flags = USF_CONST;
}

void utStringConcat(utString *s, const char *text)
{
    int textLen = (int)strlen(text);
    int newLen = textLen + s->length;
    utStringReserve(s, newLen);
    memcpy(s->buffer + s->length, text, textLen+1);
    s->length = newLen;
}

void utStringPrintf(utString *s, const char *format, ...)
{
    va_list args;
    s->length = 0;
    va_start(args, format);
    utStringConcatv(s, format, args);
    va_end(args);
}

void utStringConcatf(utString *s, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    utStringConcatv(s, format, args);
    va_end(args);
}

void utStringConcatv(utString *s, const char *format, va_list args)
{
    int textLen;
    int newLen;
    va_list argsCopy;
    va_copy(argsCopy, args);

    if(textLen == 0)
        return;

    textLen = vsnprintf(NULL, 0, format, argsCopy);
    newLen = textLen + s->length;

    utStringReserve(s, newLen);
    vsnprintf(s->buffer + s->length, newLen + 1, format, args);
    s->length = newLen;
}

const char *utStringSafe(utString *s)
{
    if(!s->buffer)
        return "";

    return s->buffer;
}
