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
	unsigned	in_use: 1; 	/* if the block is used or not */
	size_t		order; 		/* current order of block (2^order) */
	list_t*	    	succ;		/* right child block in tree */
	list_t*		pred;		/* left child block in tree */
};

#define K_MAX 22
#define K_MAX_SIZE (1 << K_MAX)
#define ORDER_0 4
// Size of the node metadata
#define META_SIZE (ALIGN(sizeof(list_t)))

static list_t* find_block(size_t);
static size_t get_order(size_t);
static list_t* split(list_t*, size_t);

/* Array of pointers to first block of order k at free_list[k] */
static list_t* freelist[K_MAX + 1];
static void* start = NULL;

static void print_freelist()
{
	debug_print("Freelist: [");
	for (int i = ORDER_0; i <= K_MAX; i++) {
		int f = 0;
		int j = 0;
		list_t* current = freelist[i];
		while (current) {
			if (!current->in_use) {
				f++;
			}
			j++;
			debug_print("%p -> ", current);
			current = current->succ;
		}

		debug_print("%d/%d, ", f, j);
	}
	debug_print("]\n");
}

void *malloc(size_t requested_size)
{
	debug_print("*******************MY MALLOC(%zu)\n", requested_size);
	print_freelist();

	if (requested_size <= 0) {
		return NULL;
	}

	if (!start) {
		// First allocation ever, grab memory and root the tree
		start = sbrk(K_MAX_SIZE);
		list_t* top = start;
		top->order = K_MAX;
		top->in_use = 0;
		top->succ = NULL;
		top->pred = NULL;
		freelist[K_MAX] = top;
	}

	/* E.g. if requested size is 56 bytes, k = 6 (2^6=64)*/
	size_t k = get_order(ALIGN(requested_size + META_SIZE));
	debug_print("Order: %zu\n", k);

	list_t* r = find_block(k);

	if (r) {
		r->in_use = 1;
		print_freelist();
		debug_print("****** Malloc returned %p\n", (r + 1));
		return (r + 1);
	} else {
		return NULL;
	}
}

/* Find the smallest power of 2 larger than k */
static size_t get_order(size_t v)
{

	int k = ORDER_0;
	while ((1 << k) < v) {
		k++;
	}
	return k;
}


// finds a suitable block of order k. if not found return null
static list_t* find_block(size_t k)
{

	if (k > K_MAX)
		return NULL;


	list_t* current = freelist[k];

	while (current) {

		if (!current->in_use)
			return current;

		current = current->succ;
	}

	list_t* big_block = find_block(k + 1);
	if (big_block) {
		current = split(big_block, k);
	}

	return current;
}

static void remove_from_freelist(list_t* item) {
	size_t k = item->order;

	if (freelist[k] == item)
		freelist[k] = item->succ;

	if (item->pred)
		item->pred->succ = item->succ;

	if (item->succ)
		item->succ->pred = item->pred;

	item->pred = NULL;
	item->succ = NULL;

}

static void add_to_freelist(list_t* item) {

	size_t k = item->order;

	if (!freelist[k]) {
		freelist[k] = item;
		item->succ = NULL;
		item->pred = NULL;
		return;
	}

	item->pred = NULL;
	item->succ = freelist[k];
	freelist[k]->pred = item;
	freelist[k] = item;
}

static list_t* split(list_t* src, size_t new_order)
{

	while (src->order > new_order) {

		/* src becomes left buddy */
		remove_from_freelist(src);

		// set new order
		src->order = src->order - 1;
		// calculate half size of old block, aka size of new order.
		size_t size = 1 << src->order;

		list_t* right = ((void*) src) + size;
		right->order = src->order;
		right->in_use = 0;

		add_to_freelist(right);
		add_to_freelist(src);

	}

	return src;
}

static void merge(list_t* block)
{

	if (block->in_use || block->order == K_MAX)
		return;

	list_t* buddy = start + ((((void*)block) - start) ^ (1 << block->order));

	if (buddy->in_use || buddy->order != block->order)
		return;

	list_t* left = block;
	list_t* right = buddy;
	if (block > buddy) {
		left = buddy;
		right = block;
	}

	remove_from_freelist(right);
	remove_from_freelist(left);
	left->order++;
	add_to_freelist(left);
	merge(left);
}


void free(void *ptr) {
	debug_print("***************************Free (%p)\n", ptr);
	print_freelist();

	if (!ptr)
		return;

	list_t* block = (((list_t*)ptr) - 1);
	assert(block->in_use);
	block->in_use = 0;
	merge(block);
	debug_print("***************************Free -- exit (%p)\n", ptr);
	print_freelist();
}

void *calloc(size_t nbr_elements, size_t element_size) {
	debug_print( "***************************CALLOC(%zu, %zu)\n", nbr_elements, element_size);

	size_t size = nbr_elements * element_size;
	void* ptr = malloc(size);
	if (ptr == NULL)
		return NULL;
	memset(ptr, 0, size);
	return ptr;
}

void *realloc(void *ptr, size_t size) {
	debug_print("***************************REALLOC(%p, %zu)\n", ptr, size);

	if (!ptr) {
		return malloc(size);
	}

	list_t* block = (((list_t*) ptr) - 1);
	if ((1 << block->order) - META_SIZE >= size) {
		return ptr;
	}

	void* new_ptr = malloc(size);
	if (!new_ptr) {
		return NULL;
	}

	memcpy(new_ptr, ptr, (1 << block->order) - META_SIZE);
	free(ptr);
	return new_ptr;
}
