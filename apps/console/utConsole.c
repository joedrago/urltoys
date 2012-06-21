#include "utString.h"

#include <stdio.h>

void test()
{
    utString *s = utStringCreate(0);
    utStringSet(s, "herp derp");
    utStringSet(s, "lulz");
    utStringSet(s, "herp derp skerp");
    utStringConcatf(s, " %s %s", "werp", "nerp");
    printf("string: '%s'\n", utStringSafe(s));
    utStringDestroy(s);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
