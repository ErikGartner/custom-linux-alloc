#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

#define DEBUG 0
#define debug_print(...) \
            do { if (DEBUG) fprintf(stderr, ##__VA_ARGS__); } while (0)

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

typedef struct list_t list_t;
struct list_t {
	size_t 		size;
	unsigned 	in_use:1;	/* if the block is used or not */
	list_t* 	next;		/* next available block. */
};

#define META_SIZE (ALIGN(sizeof(list_t)))
static list_t* allocate_space(list_t*, size_t);
static list_t* find_block(list_t**, size_t);
list_t* base = NULL;

void *malloc(size_t size)
{
	debug_print("*******************MY MALLOC(%zu)\n", size);


	if (size <= 0) {
		return NULL;
	}

	list_t* last = base;
	list_t* r = find_block(&last, size);

	if (r == NULL) {
		r = allocate_space(last, size);
		if (r == NULL)
			return NULL;

		if (base == NULL)
			base = r;
	} else {
		r->in_use = 1;
	}

	return (r + 1);

}

static list_t* allocate_space(list_t* last, size_t size)
{

	list_t* meta = sbrk(0);
	void* data = sbrk(ALIGN(size + META_SIZE));
	if (data == (void*) - 1) {
		return NULL;
	}

	meta->size = size;
	meta->next = NULL;
	meta->in_use = 1;
	if (last) {
		last->next = data;
	}

	return meta;
}

static list_t* find_block(list_t** last, size_t size)
{

	list_t* current = base;
	while (current) {
		if (!current->in_use && current->size >= size)
			return current;

		*last = current;
		current = current->next;
	}
	return current;
}



void free(void *ptr) {
	debug_print("***************************Free\n");
	if (!ptr)
		return;
	list_t* block = ((list_t*) ptr) - 1;
	assert(block->in_use == 1);
	block->in_use = 0;
}

void *calloc(size_t nbr_elements, size_t element_size) {
	//debug_print((stderr, "***************************CALLOC(%zu, %zu)\n", nbr_elements, element_size));

	size_t size = nbr_elements * element_size;
	void* ptr = malloc(size);
	if (ptr == NULL)
		return NULL;
	memset(ptr, 0, size);
	return ptr;
}

void *realloc(void *ptr, size_t size) {
	debug_print("***************************REALLOC\n");

	if (!ptr) {
		return malloc(size);
	}

	list_t* block = ((list_t*) ptr) - 1;
	if (block->size >= size) {
		return ptr;
	}

	void* new_ptr = malloc(size);
	if (!new_ptr) {
		return NULL;
	}

	memcpy(new_ptr, ptr, block->size);
	free(ptr);
	return new_ptr;
}
