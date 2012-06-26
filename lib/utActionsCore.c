#include "utActionsCore.h"

#include <stdio.h>

static void walkPrint(utList *list, int i, void *userData)
{
    utContext *context = (utContext *)userData;
    context->outputCB(utStringContents(&list->lines.data[i]->text));
}

static int utActionShow(struct utAction *action, utCommand *command, struct utContext *context, struct utList *list)
{
    utListSubset *subset = NULL;
    utString *error = utStringCreate();
    if(command->args.count)
    {
        subset = utListFilterRegex(list, LS_ALL, command->args.data[0]->s, error);
    }
    if(error->length)
    {
        context->errorCB(error->s);
    }
    else
    {
        utListWalk(context->current, subset ? subset : LS_ALL, walkPrint, context);
    }
    if(subset)
        utListSubsetDestroy(subset);
    utStringDestroy(error);
    return 1;
}

static int utActionAdd(struct utAction *action, utCommand *command, struct utContext *context, struct utList *list)
{
    if(command->args.count)
    {
        int i;
        for(i = 0; i < command->args.count; i++)
        {
            utString *s = command->args.data[i];
            utListPush(context->current, s->s);
        }
    }
    else
    {
        context->errorCB("add requires arguments");
    }
    return 1;
}

static int utActionClear(struct utAction *action, utCommand *command, struct utContext *context, struct utList *list)
{
    utListClear(context->current);
    return 1;
}

void utActionsRegisterCore(utContext *context)
{
    utContextRegister(context, "show", utActionShow);
    utContextRegister(context, "add", utActionAdd);
    utContextRegister(context, "clear", utActionClear);
}
