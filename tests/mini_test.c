#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{

	char *str;

	str = (char *) malloc(15);
	strcpy(str, "tutorialspoin1");
	printf("String = %s,  Address = %p\n", str, (void*)str);
	free(str);

	str = (char *) malloc(15);
	printf("Address = %p\n", (void*)str);
	strcpy(str, "tutorialspoin2");
	str = (char *) malloc(6400);
	printf("Address = %p\n", (void*)str);
	free(str);

	str = (char *) malloc(6400);
	printf("Address = %p\n", (void*)str);
	strcpy(str, "tutorialspoin2");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	char *str2 = (char *) malloc(15);
	printf("Address = %p\n", str);
	strcpy(str2, "tutorialspoin2");
	printf("String = %s,  Address = %p\n", str2, (void*)str2);

	str = (char *) realloc(str, 15);
	printf("Address = %p\n", str);
	strcpy(str, "tutorialspoint");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	str = (char *) calloc(1, 15);
	printf("Address = %p\n", str);
	strcpy(str, "tutorialspoint");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	printf("Address = %p\n", str2);
	printf("String = %s,  Address = %p\n", str2, (void*)str2);
	return (0);

}
