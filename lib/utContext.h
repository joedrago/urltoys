#ifndef UTCONTEXT_H
#define UTCONTEXT_H

#include "utArray.h"
#include "utList.h"

struct utAction;
struct utContext;

// --------------------------------------------------------------------------------------
// User issued commands, typically from a parsed command line

typedef struct utCommand
{
    utString line;
    utString name;
    utStringArray args;
} utCommand;

utCommand *utCommandCreate();
void utCommandDestroy(utCommand *command);
int utCommandParse(utCommand *command, const char *text);

// --------------------------------------------------------------------------------------
// Actions are responses to commands that are registered with a context

typedef int (*utActionFunc)(struct utAction *action, utCommand *command, struct utContext *context, struct utList *list);

typedef struct utAction
{
    utString name;
    utActionFunc func;
} utAction;

utAction * utActionCreate(const char *name, utActionFunc func);
void utActionDestroy(utAction *action);

ARRAY_DECLARE(utAction);

// --------------------------------------------------------------------------------------
// One "instance" of URLToys

typedef void (*utOutputCB)(const char *line);

typedef struct utContext
{
    utList *current;
    utOutputCB outputCB;
    utOutputCB errorCB;
    utActionArray actions;
} utContext;

utContext *utContextCreate();
void utContextDestroy(utContext *context);
void utContextRegister(utContext *context, const char *name, utActionFunc func);
int utContextExec(utContext *context, utCommand *command);
int utContextParseExec(utContext *context, const char *text);

#endif
