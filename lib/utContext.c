#include "utContext.h"

#include <stdio.h>

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

int utContextParse(utContext *context, char *text)
{
    // TODO: not this

    if(!strcmp(text, "show"))
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
        if(strlen(text))
            utListPush(context->current, text);
        else
            return 0;
    }

    return 1;
}
