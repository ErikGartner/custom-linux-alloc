#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*
Check some simple stuff in malloc: not-null-pointer, etc
*/
static void* xmalloc(size_t size)
{
	void* p = malloc(size);
	assert(p != NULL);
	return p;
}

static void* xcalloc(size_t elems, size_t size)
{
	void* p = calloc(elems, size);
	assert(p != NULL);
	return p;
}

static void* xrealloc(void* p0, size_t size)
{
	void* p = realloc(void* p0, size);
	assert(p != NULL);
	return p;
}

int main()
{

	char *str;

	str = (char *) xmalloc(15);
	assert(str != NULL);
	strcpy(str, "tutorialspoin1");
	printf("String = %s,  Address = %p\n", str, (void*)str);
	free(str);

	str = (char *) xmalloc(15);
	assert(str != NULL);
	printf("Address = %p\n", (void*)str);
	strcpy(str, "tutorialspoin2");
	str = (char *) xmalloc(6400);
	assert(str != NULL);
	printf("Address = %p\n", (void*)str);
	free(str);

	str = (char *) xmalloc(6400);
	assert(str != NULL);
	printf("Address = %p\n", (void*)str);
	strcpy(str, "tutorialspoin2");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	char *str2 = (char *) xmalloc(15);
	assert(str != NULL);
	printf("Address = %p\n", str);
	strcpy(str2, "tutorialspoin2");
	printf("String = %s,  Address = %p\n", str2, (void*)str2);

	str = (char *) xrealloc(str, 15);
	assert(str != NULL);
	printf("Address = %p\n", str);
	strcpy(str, "tutorialspoint");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	str = (char *) xcalloc(1, 15);
	printf("Address = %p\n", str);
	strcpy(str, "tutorialspoint");
	printf("String = %s,  Address = %p\n", str, (void*)str);

	printf("Address = %p\n", str2);
	printf("String = %s,  Address = %p\n", str2, (void*)str2);
	return (0);

}
