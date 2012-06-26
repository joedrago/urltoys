#include "utList.h"

#include <pcre.h>

const utListSubset LS_ALL[] = { { LST_ALL, 0 } };

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

utListSubset *utListSubsetCreate(int type)
{
    utListSubset *subset = (utListSubset *)calloc(1, sizeof(utListSubset));
    subset->type = type;
    return subset;
}

void utListSubsetDestroy(utListSubset *subset)
{
    utIntArrayClear(&subset->indices);
    free(subset);
}

void utListSubsetPushIndex(utListSubset *subset, int i)
{
    utIntArrayPush(&subset->indices, i);
}

utList *utListCreate()
{
    utList *list = (utList *)calloc(1, sizeof(utList));
    return list;
}

void utListDestroy(utList *list)
{
    utListClear(list);
    free(list);
}

void utListClear(utList *list)
{
    utLineArrayClear(&list->lines);
}

void utListPush(utList *list, const char *text)
{
    utLineArrayPush(&list->lines, utLineCreate(text));
}

void utListWalk(utList *list, const utListSubset *subset, utListWalkCB walkCB, void *userData)
{
    switch(subset->type)
    {
        default:
        case LST_NONE:
            return;

        case LST_ALL:
            {
                int i;
                for(i = 0; i < list->lines.count; i++)
                {
                    walkCB(list, i, userData);
                }
                break;
            }

        case LST_SELECTED:
            {
                int i;
                for(i = 0; i < list->lines.count; i++)
                {
                    if(list->lines.data[i]->flags & LF_SELECTED)
                        walkCB(list, i, userData);
                }
                break;
            }

        case LST_INDICES:
            {
                int i;
                for(i = 0; i < subset->indices.count; i++)
                {
                    walkCB(list, subset->indices.data[i], userData);
                }
                break;
            }
    };
}

static void walkDelete(utList *list, int i, void *userData)
{
    utLineDestroy(list->lines.data[i]);
    list->lines.data[i] = NULL;
}

void utListDelete(utList *list, const utListSubset *subset)
{
    utListWalk(list, subset, walkDelete, NULL);
    utLineArraySquash(&list->lines);
}

struct FilterData
{
    const char *text;
    pcre *regex;
    utListSubset *subset;
};

static void walkFilterSubstr(utList *list, int i, void *userData)
{
    struct FilterData *data = (struct FilterData *)userData;
    utLine *line = list->lines.data[i];
    if(strstr(line->text.s, data->text))
    {
        utListSubsetPushIndex(data->subset, i);
    }
}

utListSubset * utListFilterSubstr(utList *list, const utListSubset *toFilter, const char *text)
{
    struct FilterData data;
    data.text = text;
    data.subset = utListSubsetCreate(LST_INDICES);
    utListWalk(list, toFilter, walkFilterSubstr, &data);
    return data.subset;
}

static void walkFilterRegex(utList *list, int i, void *userData)
{
    struct FilterData *data = (struct FilterData *)userData;
    utLine *line = list->lines.data[i];
    if(pcre_exec(data->regex, NULL, line->text.s, line->text.length, 0, 0, NULL, 0) >= 0)
    {
        utListSubsetPushIndex(data->subset, i);
    }
}

utListSubset * utListFilterRegex(utList *list, const utListSubset *toFilter, const char *text, utString *returnError)
{
    struct FilterData data;
    const char *error;
    int erroffset;
    data.text = text;
    data.regex = pcre_compile(data.text, 0, &error, &erroffset, NULL);
    if(data.regex)
    {
        data.subset = utListSubsetCreate(LST_INDICES);
        utListWalk(list, toFilter, walkFilterRegex, &data);
        pcre_free(data.regex);
    }
    else
    {
        if(error && returnError)
        {
            utStringSet(returnError, error);
        }
    }
    return data.subset;
}
