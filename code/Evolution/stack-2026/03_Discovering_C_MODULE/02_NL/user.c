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
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\02_nl_b>gcc stack.c user.c -o use_stack
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccy2a8T4.o:user.c:(.bss+0x0): multiple definition of `stack'; C:\Users\vikto\AppData\Local\Temp\ccvCaIXh.o:stack.c:(.bss+0x0): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccy2a8T4.o:user.c:(.bss+0x64): multiple definition of `top'; C:\Users\vikto\AppData\Local\Temp\ccvCaIXh.o:stack.c:(.bss+0x64): first defined here
collect2.exe: error: ld returned 1 exit status
*/

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\src\evolution\stack-2026\03_header_mistakes\02_nl_b>clang stack.c user.c -o use_stack
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-da01f4.o:user.c:(.bss+0x0): multiple definition of `stack'; C:/Users/vikto/AppData/Local/Temp/stack-b6886b.o:stack.c:(.bss+0x10): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-da01f4.o:user.c:(.bss+0x64): multiple definition of `top'; C:/Users/vikto/AppData/Local/Temp/stack-b6886b.o:stack.c:(.bss+0x0): first defined here
clang: error: linker command failed with exit code 1 (use -v to see invocation)
*/
