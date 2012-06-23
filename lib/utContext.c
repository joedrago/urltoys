#include "utContext.h"
#include "utActionsCore.h"

#include <stdio.h>

ARRAY_IMPLEMENT(utAction, utActionDestroy);

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
        if(pieces->count && pieces->data[0]->length)
        {
            int i;
            utStringCopy(&command->name, pieces->data[0]);
            for(i = 1; i < pieces->count; i++)
            {
                utString *s = pieces->data[i];
                utStringArrayPush(&command->args, s);
                pieces->data[i] = NULL;
            }
            utStringArraySquash(pieces);
        }
        utStringArrayDestroy(pieces);
        return 1;
    }
    return 0;
}

utAction * utActionCreate(const char *name, utActionFunc func)
{
    utAction *action = (utAction *)calloc(1, sizeof(utAction));
    utStringSet(&action->name, name);
    action->func = func;
    return action;
}

void utActionDestroy(utAction *action)
{
    utStringClear(&action->name);
    free(action);
}

static void defaultOutputCB(const char *line)
{
    printf("%s\n", line);
}

static void defaultErrorCB(const char *line)
{
    printf("ERROR: %s\n", line);
}

utContext *utContextCreate()
{
    utContext *context = (utContext *)calloc(1, sizeof(utContext));
    context->current = utListCreate();
    context->outputCB = defaultOutputCB;
    context->errorCB = defaultErrorCB;
    utActionsRegisterCore(context);
    return context;
}

void utContextDestroy(utContext *context)
{
    if(context->current)
        utListDestroy(context->current);
    utActionArrayClear(&context->actions);
    free(context);
}

void utContextRegister(utContext *context, const char *name, utActionFunc func)
{
    utAction *action = utActionCreate(name, func);
    utActionArrayPush(&context->actions, action);
}

int utContextExec(utContext *context, utCommand *command)
{
    int i;
    for(i = 0; i < context->actions.count; i++)
    {
        utAction *action = context->actions.data[i];
        if(!utStringCmp(&action->name, &command->name))
        {
            return action->func(action, command, context, context->current);
        }
    }
    return 0;
}

int utContextParse(utContext *context, const char *text)
{
    int ret = 0;
    utCommand *command = utCommandCreate();

    if(utCommandParse(command, text))
    {
        ret = utContextExec(context, command);
    }

    utCommandDestroy(command);
    return ret;
}
