#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

#define N (8 * 1024*1024 / sizeof(size_t))

typedef struct list_t list_t;
struct list_t {
  size_t size; /* size including list_t */
  list_t* next; /* next available block. */
  char data[]; /* C99 flexible array . */
};

static size_t pool[N] = { N * sizeof pool[0] };
static list_t avail = { .next = (list_t*)pool };

void *malloc(size_t size)
{
  size_t real_size = size + sizeof(list_t);
  list_t* p = &avail;
  list_t* q = NULL;

  if(p->size > 0) {
    q = p->next;
  }

  list_t* r = NULL;

  // find a gap in the list
  while(q != NULL){
    size_t free_block_size = q - (p + p->size);
    if(free_block_size >= real_size)
      r = (p + p->size + sizeof(size_t));

    p = q;
    q = p->next;
  }


  // if no gap found, end of list
  if(r == NULL) {
    size_t end_of_list = p + p->size;
    if (end_of_list + real_size > &pool + sizeof(pool)) {
      // HERE WE SHOULD INCREASE LIST SIZE WITH SBRK.
    } else {
      r = end_of_list + sizeof(size_t);
    }
  }

  size_t* n = (char*)r - sizeof(size_t);
  *n = size;
  printf("%zu %zu %zu %zu %zu\n", r - sizeof(size_t), n, *n, pool);

  r->size = size;
  r->next = q;
  p->next = r;

  return r;
}

void free(void *ptr) {



}
