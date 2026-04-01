
#include <stdio.h>
int main (void)
{
int i; const int ci=2;
int * pi1=&i, * const cpi1=&i;  	/* OK */
int * pi2=&ci, * const cpi2=&ci; 	/* C:Warning,C++:Error */
const int * pci1=&ci, * const cpci1=&ci; 	/* OK */
const int * pci2=&i, * const cpci2=&i;  	/* OK */


/*
int *pi1=&i;
int *pi2=&ci; //C - Suspicious, C++ - Error

const int * pci1=&ci;
const int * pci2=&i; //!

int * const cpi1=&i;
int * const cpi2=&ci; //C - Suspicious, C++ - Error

const int * const cpci1=&i;
const int * const cpci2=&ci;
*/

pci1=pi1;
/* 
C - OK 
C++ - OK
*/

pi1 = pci1; 
/* 
C - Warning: Suspicious pointer conversion
C++ - Error: Cannont convert 'const int *' to 'int *'
*/

/*
*pi1++;
*(pi1++);
(*pi1)++;

*pci1++;
*(pci1++);
(*pci1)++; //Error

*cpi1++;   //Error
*(cpi1++); //Error
(*cpi1)++;

*cpci1++;   //Error
*(cpci1++); //Error
(*cpci1)++; //Error
*/

return 0;
}