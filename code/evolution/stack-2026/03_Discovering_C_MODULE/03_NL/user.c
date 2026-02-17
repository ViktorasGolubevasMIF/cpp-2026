#include <stdio.h>
#include "stack.h"

int main(void) {
    char c;

    init();
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);
    while (!isEmpty()) putchar(pop());

    putchar('\n');
    return 0;
}

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\03_nl_c>gcc stack.c user.c -o use_stack
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\cc51uPmo.o:stack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\cc51uPmo.o:stack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
collect2.exe: error: ld returned 1 exit status
*/

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\03_nl_c>clang stack.c user.c -o use_stack
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/stack-5551ba.o:stack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/stack-5551ba.o:stack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
clang: error: linker command failed with exit code 1 (use -v to see invocation)
*/