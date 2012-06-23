#include "utContext.h"

#include <stdio.h>

utCommand *utCommandCreate()
{
    utCommand *command = (utCommand *)calloc(1, sizeof(utCommand));
    return command;
}

void utCommandDestroy(utCommand *command)
{
    utStringClear(&command->line);
    utStringClear(&command->name);
    utStringArrayClear(&command->args);
    free(command);
}

int utCommandParse(utCommand *command, const char *line)
{
    utStringArray *pieces;

    utStringSet(&command->line, line);
    pieces = utStringSplitQuoted(&command->line);
    if(pieces)
    {
        int i;
        for(i = 0; i < pieces->count; i++)
        {
            utString *s = pieces->data[i];
            printf("arg%d: '%s'\n", i, utStringSafe(s));
        }
        if(pieces->count)
        {
            utStringCopy(&command->name, pieces->data[0]);
        }
        utStringArrayDestroy(pieces);
        return 1;
    }
    return 0;
}

static void defaultOutputCB(const char *line)
{
    printf("* %s\n", line);
}

utContext *utContextCreate()
{
    utContext *context = (utContext *)calloc(1, sizeof(utContext));
    context->current = utListCreate();
    context->outputCB = defaultOutputCB;
    return context;
}

void utContextDestroy(utContext *context)
{
    if(context->current)
        utListDestroy(context->current);
    free(context);
}

static void walkPrint(utList *list, int i, void *userData)
{
    utContext *context = (utContext *)userData;
    context->outputCB(utStringSafe(&list->lines.data[i]->text));
}

int utContextParse(utContext *context, const char *text)
{
    int ret = 1;
    utCommand *command = utCommandCreate();

    if(utCommandParse(command, text))
    {
        if(!strcmp(utStringSafe(&command->name), "show"))
        {
            utListWalk(context->current, LS_ALL, walkPrint, context);
        }
        else
        {
            if(command->line.length)
                utListPush(context->current, command->line.buffer);
            else
                ret = 0;
        }
    }

    utCommandDestroy(command);
    return ret;
}
