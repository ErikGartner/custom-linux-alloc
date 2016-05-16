#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  char *str;

  /* Initial memory allocation */
  str = (char *) malloc(15);
  printf("Address = %u\n", str);
  strcpy(str, "tutorialspoint");
  printf("String = %s,  Address = %u\n", str, str);
  //free(str);
  return (0);
}
