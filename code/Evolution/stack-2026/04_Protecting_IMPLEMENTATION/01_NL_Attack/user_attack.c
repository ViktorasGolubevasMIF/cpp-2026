#include <stdio.h>
#include "stack.h"

extern char stack[];
extern int top;
extern void reset(void);

int main(void) {
    char c;

    init();
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);

    stack[top++] = '!'; // attack non-hiiden data  // +

    while (!isEmpty()) putchar(pop());
    putchar('\n');

    reset();
    return 0;
}

/*
\04_hiding_Implementation\01_nl_z>gcc stack.c user_attack.c -o user_attack

C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccuqV69P.o:user_attack.c:(.text+0x7d): undefined reference to `reset'
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccuqV69P.o:user_attack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccuqV69P.o:user_attack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
collect2.exe: error: ld returned 1 exit status
*/

/*
\04_hiding_Implementation\01_nl_z>clang stack.c user_attack.c -o user_attack
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user_attack-4b596d.o:user_attack.c:(.text+0xc1): undefined reference to `reset'
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user_attack-4b596d.o:user_attack.c:(.rdata$.refptr.stack[.refptr.stack]+0x0): undefined reference to `stack'
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user_attack-4b596d.o:user_attack.c:(.rdata$.refptr.top[.refptr.top]+0x0): undefined reference to `top'
clang: error: linker command failed with exit code 1 (use -v to see invocation)
*/