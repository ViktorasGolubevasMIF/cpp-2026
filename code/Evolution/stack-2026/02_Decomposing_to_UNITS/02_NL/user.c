#include <stdio.h>
#include "stack.c"

int main(void) {
    char c;

    init();

    /* Patobuliname: Tik jei stekas neužpildytas, skaitome simbolius iki '\n' */
    while ( (!isFull()) && ('\n' != (c = getchar())) ) push(c);

    while (!isEmpty()) putchar(pop());

    putchar('\n');
    return 0;
}

/*
C:\Repos\Github\ViktorasGolubevas\cpp2026-test\code\evolution\stack-2026\02_separating_Files\02_NL>gcc stack.c user.c -o app
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.text+0x0): multiple definition of `init'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.text+0x0): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.text+0x11): multiple definition of `isEmpty'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.text+0x11): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.text+0x25): multiple definition of `isFull'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.text+0x25): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.text+0x3a): multiple definition of `push'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.text+0x3a): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.text+0x76): multiple definition of `pop'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.text+0x76): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.bss+0x0): multiple definition of `stack'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.bss+0x0): first defined here
C:/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\vikto\AppData\Local\Temp\ccHdaJW6.o:user.c:(.bss+0x8): multiple definition of `top'; C:\Users\vikto\AppData\Local\Temp\ccIlwfrX.o:stack.c:(.bss+0x8): first defined here
collect2.exe: error: ld returned 1 exit status
*/

/*
clang stack.c user.c -o app
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.text+0x0): multiple definition of `init'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.text+0x0): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.bss+0x0): multiple definition of `top'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.bss+0x0): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.text+0x10): multiple definition of `isEmpty'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.text+0x10): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.text+0x30): multiple definition of `isFull'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.text+0x30): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.text+0x50): multiple definition of `push'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.text+0x50): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.bss+0x4): multiple definition of `stack'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.bss+0x4): first defined here
C:/mingw64/bin/ld: C:/Users/vikto/AppData/Local/Temp/user-58106f.o:user.c:(.text+0x90): multiple definition of `pop'; C:/Users/vikto/AppData/Local/Temp/stack-bb4c10.o:stack.c:(.text+0x90): first defined here
clang: error: linker command failed with exit code 1 (use -v to see invocation)

*/