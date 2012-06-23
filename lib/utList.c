#include "utList.h"

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
