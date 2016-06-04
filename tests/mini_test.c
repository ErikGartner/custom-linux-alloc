#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

/*
Get a string of size `size`
*/
static void getString(char* str, size_t size)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
	if (size) {
		--size;
		for (size_t n = 0; n < size; n++) {
			str[n] = charset[n % 40];
		}
		str[size] = '\0';
	}
}

/*
Call malloc and fill memory with string
*/
static void* xmalloc(size_t size)
{
	void* p = malloc(size);
	assert(p != NULL);

	// Get a string to fill the alloted memory with
	char str[size];
	getString(str, size);
	strcpy(p, str);

	// Print memory contents and pointer address
	printf("%s at %p\n", (char*)p, p);

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
	void* p = realloc(p0, size);
	assert(p != NULL);
	return p;
}

int main()
{
	char *str;

	str = (char *) xmalloc(150);
	free(str);

	str = (char *) xmalloc(15);
	str = (char *) xmalloc(640);
	free(str);

	str = (char *) xmalloc(640);

	char *str2 = (char *) xmalloc(15);
	char *str3 = (char *) xrealloc(str2, 150);
	str2 = (char *) xcalloc(1, 15);

	return (0);
}
