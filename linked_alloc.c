#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

typedef struct list_t list_t;
struct list_t {
  size_t size; /* size including list_t */
  int in_use; /* if the block is used or not */
  list_t* next; /* next available block. */
};

static list_t* base = NULL;

void *malloc(size_t size)
{

  list_t* r = base;

  // find empty block

  // null none was find
  if (r == NULL) {

     list_t* meta = sbrk(0);
     void* data =

  }






}

list_t* allocate_space(list_t* last, size_t size) {

  list_t* meta = sbrk(0);
  void* data = sbrk(size + sizeof(list_t));
  if (data == (void*) -1) {
    return NULL;
  }

  meta->size = size;
  meta->next = NULL;
  meta->in_use = 1;
  if(last != NULL) {
    last->next = data;
  }

  return meta;
}

list_t* find_block(list_t* start, )



void free(void *ptr) {



}
