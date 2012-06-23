#include "utContext.h"

#include <stdio.h>
#include <readline/readline.h>

void test()
{
    utContext *context = utContextCreate();
    utString *prompt = utStringCreate();
    char *rawLine;

    while(1)
    {
        utStringPrintf(prompt, "U[%d]> ", context->current->lines.count);
        rawLine = readline(utStringContents(prompt));

        if(rawLine && utContextParse(context, rawLine))
        {
            add_history(rawLine);
        }

        free(rawLine);
    }

    utContextDestroy(context);
}

void test2()
{
    utContext *context = utContextCreate();
    const char *rawLine = "\"\\\\\"";
    utContextParse(context, rawLine);
    utContextDestroy(context);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
