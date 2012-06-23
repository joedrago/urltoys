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
        utStringArrayDestroy(pieces);
    }
    return 0;
}

static void defaultOutputCB(const char *line)
{
    printf("%s\n", line);
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

int utContextParse(utContext *context, const char *text)
{
    int ret = 1;
    utCommand *command = utCommandCreate();

    if(utCommandParse(command, text))
    {
        if(!strcmp(utStringSafe(&command->name), "show"))
        {
            int i;
            for(i = 0; i < context->current->lines.count; i++)
            {
                utLine *line = context->current->lines.data[i];
                context->outputCB(utStringSafe(&line->text));
            }
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
