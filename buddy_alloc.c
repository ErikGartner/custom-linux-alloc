#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void *malloc(size_t size)
{
 fprintf(stderr, "My malloc called!\n");
 return NULL;
}
