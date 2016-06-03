#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

#define DEBUG 1
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
#define ORDER_0 8
// Size of the node metadata
#define META_SIZE (ALIGN(sizeof(list_t)))

static list_t* find_block(size_t);
static size_t get_order(size_t);
static list_t* split(list_t*, size_t);

/* Array of pointers to first block of order k at free_list[k] */
static list_t* freelist[K_MAX + 1];
static void* start = NULL;

void *malloc(size_t requested_size)
{
	debug_print("*******************MY MALLOC(%zu)\n", requested_size);

	if (requested_size <= 0) {
		return NULL;
	}

	if (!start) {
		// First allocation ever, grab memory and root the tree
		start = sbrk(K_MAX_SIZE);
                assert(start != -1);

		list_t* top = start;
		top->order = K_MAX;
		top->in_use = 0;
		top->succ = NULL;
		top->pred = NULL;
		freelist[K_MAX] = top;
	}

	/* E.g. if requested size is 56 bytes, k = 6 (2^6=64)*/
	size_t k = get_order(requested_size + META_SIZE);
        debug_print("Order: %d\n", k);

	list_t* r = find_block(k);

	if (r) {
                r->in_use = 1;
                return (r + 1);
        }else{
                return NULL;
        }
}

/* Find the smallest power of 2 larger than k */
static size_t get_order(size_t v)
{
	/* http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2 */
	// v--;
	// v |= v >> 1;
	// v |= v >> 2;
	// v |= v >> 4;
	// v |= v >> 8;
	// v |= v >> 16;
	// v++;
        int k = ORDER_0;
        while (1 << k < v)
                k++;
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

static list_t* split(list_t* src, size_t new_order)
{

	while (src->order > new_order) {

                /* src becomes left buddy */
                src->order--;
                if (src->pred)
                        src->pred->succ = src->succ;
                if (src->succ)
                        src->succ->pred = src->pred;
		src->pred = NULL;
		src->succ = NULL;

                size_t size = 1 << src->order;

		list_t* right = ((void*) src + size);
		right->order = src->order;
		right->in_use = 0;
		right->pred = src;
		right->succ = freelist[src->order];
                src->succ = right;

		if (freelist[src->order])
			freelist[src->order]->pred = right;
		freelist[src->order] = src;

	}

	return src;
}

static void merge(size_t k)
{
	return;
}


void free(void *ptr) {
	debug_print("***************************Free\n");
	if (!ptr)
		return;
	return;
}

void *calloc(size_t nbr_elements, size_t element_size) {
	debug_print((stderr, "***************************CALLOC(%zu, %zu)\n", nbr_elements, element_size));

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
	if (2 << block->order >= size + META_SIZE) {
		return ptr;
	}

	void* new_ptr = malloc(size);
	if (!new_ptr) {
		return NULL;
	}

	memcpy(new_ptr, ptr, (2 << block->order) - META_SIZE);
	free(ptr);
	return new_ptr;
}
