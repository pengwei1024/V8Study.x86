//
// Created by Peng,Wei(BAMRD) on 2021/4/5.
//

#include <execinfo.h>
#include <cstdio>
#include <cstdlib>

/**
 * 打印调用堆栈
 */
void print_stacktrace()
{
    int size = 16;
    void * array[16];
    int stack_num = backtrace(array, size);
    char ** stacktrace = backtrace_symbols(array, stack_num);
    for (int i = 0; i < stack_num; ++i)
    {
        printf("%s\n", stacktrace[i]);
    }
    free(stacktrace);
}

