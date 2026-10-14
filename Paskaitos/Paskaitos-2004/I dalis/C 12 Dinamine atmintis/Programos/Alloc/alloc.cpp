
#include <stdio.h>
#include <stdlib.h>
int main (void)
{
int * memptr1;
int dim; 
char * memptr2;
  memptr1 =new int;
  delete memptr1;
  
  printf("Kiek atminties reikia eilutei?\n");
  scanf("%d",&dim);
  if (0 == (memptr2 = new char[dim] )) { 
    printf("Neliko laisvos atminties\n"); 
    exit(1); 
  }
  delete memptr2;

  return 0;
}