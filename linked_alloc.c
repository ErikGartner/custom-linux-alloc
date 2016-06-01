#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>


typedef struct list_t list_t;
struct list_t {
	size_t size;
	int in_use;												/* if the block is used or not */
	list_t* next;												 /* next available block. */
};

list_t* allocate_space(list_t*, size_t);
list_t* find_block(list_t**, size_t);
static list_t* base = NULL;

void *malloc(size_t size)
{
	if (size <= 0) {
		return NULL;
	}

	size_t real_size = size + sizeof(list_t);

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

list_t* allocate_space(list_t* last, size_t size)
{

	list_t* meta = sbrk(0);
	void* data = sbrk(size + sizeof(list_t));
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

list_t* find_block(list_t** last, size_t size)
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
	list_t* block = ((list_t*) ptr) - 1;
	block->in_use = 0;
}

void *calloc(size_t nbr_elements, size_t element_size) {
	size_t size = nbr_elements * element_size;
	void* ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void *realloc(void *ptr, size_t size) {

	if (!ptr) {
		return malloc(size);
	}

	list_t* block = ((list_t*) ptr) - 1;
	if (block->size == size) {
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
