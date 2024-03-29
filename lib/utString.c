#include "utString.h"

#include <stdlib.h>
#include <string.h>

static const char EMPTY_STRING[] = "";

ARRAY_IMPLEMENT(utString, utStringDestroy);

utString * utStringCreate()
{
    utString *s = (utString *)calloc(1, sizeof(utString));
    return s;
}

utString * utStringCreateFrom(const char *text)
{
    utString *s = utStringCreate();
    utStringSet(s, text);
    return s;
}

utString * utStringCreateSubstr(utString *orig, int start, int length)
{
    utString *s = utStringCreate();
    if((start >= 0) && (start < orig->length))
    {
        int maxLength = orig->length - start;
        if(length > maxLength)
            length = maxLength;
        utStringReserve(s, length);
        memcpy(s->s, orig->s + start, length);
        s->s[length] = 0;
        s->length = length;
    }
    return s;
}

void utStringReserve(utString *s, int newSize)
{
    char *originalBuffer = s->s;
    if(newSize <= s->capacity)
        return;

    s->s = malloc(sizeof(char) * (newSize + 1));
    if(s->length)
        memcpy(s->s, originalBuffer, s->length + 1);
    else
        s->s[0] = 0;
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
    if(s->s && !(s->flags & USF_CONST))
        free(s->s);
    s->s = (char *)EMPTY_STRING;
    s->flags = USF_CONST;
    s->length = 0;
}

void utStringCopy(utString *s, utString *t)
{
    utStringSet(s, utStringContents(t));
}

int utStringCmp(utString *a, utString *b)
{
    return strcmp(a->s, b->s);
}

void utStringSet(utString *s, const char *text)
{
    int len = (int)strlen(text);
    if(len)
    {
        utStringReserve(s, len);
        strcpy(s->s, text);
    }
    s->length = len;
}

void utStringSetK(utString *s, const char *text)
{
    utStringClear(s);
    s->s = (char *)text;
    s->length = (int)strlen(text);
    s->flags = USF_CONST;
}

void utStringConcat(utString *s, const char *text)
{
    int textLen = (int)strlen(text);
    int newLen = textLen + s->length;
    utStringReserve(s, newLen);
    memcpy(s->s + s->length, text, textLen+1);
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

    textLen = vsnprintf(NULL, 0, format, argsCopy);
    if(textLen == 0)
        return;
    newLen = textLen + s->length;

    utStringReserve(s, newLen);
    vsnprintf(s->s + s->length, newLen + 1, format, args);
    s->length = newLen;
}

static void utStringUnescapeQuotes(utString *s)
{
    int dropped = 0;
    int head = 0;
    int tail = 0;
    while(tail < s->length)
    {
        if((s->s[tail] == '\\') && !dropped)
        {
            dropped = 1;
        }
        else
        {
            s->s[head] = s->s[tail];
            head++;
            dropped = 0;
        }
        tail++;
    }
    s->s[head] = 0;
    s->length = head;
}

utStringArray * utStringSplitQuoted(utString *s)
{
    utStringArray *array = NULL;
    if(s->length)
    {
        int i;
        int front = 0;
        int quoted = 0;
        int escapeCount = 0;
        int onlySpaces = 1;

        for(i = 0; i < s->length; i++)
        {
            if(s->s[i] != ' ')
            {
                onlySpaces = 0;
                break;
            }
        }

        if(onlySpaces)
        {
            return NULL;
        }

        array = utStringArrayCreate();
        for(i = 0; i < s->length; i++)
        {
            if(quoted)
            {
                if((s->s[i] == '"') && ((escapeCount % 2) == 0))
                {
                    utString *newString = utStringCreateSubstr(s, front, i - front);
                    utStringUnescapeQuotes(newString);
                    utStringArrayPush(array, newString);
                    front = i + 1;
                    quoted = 0;
                }
            }
            else
            {
                if((s->s[i] == '"') && ((escapeCount % 2) == 0))
                {
                    front = i + 1;
                    quoted = 1;
                }
                else if(s->s[i] == ' ')
                {
                    if((i - front) > 0)
                        utStringArrayPush(array, utStringCreateSubstr(s, front, i - front));
                    front = i + 1;
                }
            }

            if(s->s[i] == '\\')
            {
                escapeCount++;
            }
            else
            {
                escapeCount = 0;
            }
        }
        if(quoted) // || ((escapeCount % 2) != 0))
        {
            // unbalanced quotes, bail out
            utStringArrayDestroy(array);
            array = NULL;
        }
        else
        {
            if((i - front) > 0)
            {
                // get the leftovers
                utStringArrayPush(array, utStringCreateSubstr(s, front, i - front));
            }
        }
    }

    if(array)
    {
        int i;
        for(i = 0; i < array->count; i++)
        {
            //utStringUnescapeQuotes(array->data[i]);
        }
    }
    return array;
}

