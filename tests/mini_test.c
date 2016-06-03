#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  char *str;

  /* Initial memory allocation */
 // str = (char *) malloc(15);
  str = (char *) malloc(64);
  str = (char *) malloc(56);
  str = (char *) malloc(6400);
  printf("Address = %u\n", str);
  strcpy(str, "tutorialspoint");
  printf("String = %s,  Address = %u\n", str, str);
  free(str);

  str = (char *) malloc(15);
  printf("Address = %u\n", str);
  strcpy(str, "tutorialspoin2");
  printf("String = %s,  Address = %u\n", str, str);

  char *str2 = (char *) malloc(15);
  printf("Address = %u\n", str);
  strcpy(str2, "tutorialspoin2");
  printf("String = %s,  Address = %u\n", str2, str2);
  //
  // str = (char *) realloc(str, 15);
  // printf("Address = %u\n", str);
  // strcpy(str, "tutorialspoint");
  // printf("String = %s,  Address = %u\n", str, str);
  //
  // str = (char *) calloc(1, 15);
  // printf("Address = %u\n", str);
  // strcpy(str, "tutorialspoint");
  // printf("String = %s,  Address = %u\n", str, str);
  //
  // printf("Address = %u\n", str2);
  // printf("String = %s,  Address = %u\n", str2, str2);
  return (0);
}
