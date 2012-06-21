#include "utList.h"

ARRAY_IMPLEMENT(utLine, utLineDestroy);

utLine *utLineCreate(const char *text)
{
    utLine *line = (utLine *)calloc(1, sizeof(utLine));
    utStringSet(&line->text, text);
    return line;
}

void utLineDestroy(utLine *line)
{
    utStringClear(&line->text);
    free(line);
}

utList *utListCreate()
{
    utList *list = (utList *)calloc(1, sizeof(utList));
    return list;
}

void utListDestroy(utList *list)
{
    utLineArrayClear(&list->lines);
    free(list);
}

void utListPush(utList *list, const char *text)
{
    utLineArrayPush(&list->lines, utLineCreate(text));
}
