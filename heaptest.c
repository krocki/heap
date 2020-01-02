#include <stdio.h>
#include "heap.h"

#define randf() (rand() / (RAND_MAX + 1.))

heap h;

void makeheap(unsigned n) {

  // insert n random values into the heap
  for (unsigned i=0; i<n; i++) {
    int k = i;
    float v = randf();
    kv e = {.key=k, .val=v};
    heap_insert(&h, &e);
  }
}

void popheap(unsigned n) {

  // retrieve the lowest element until empty
  while (h.len > 0) {
    kv *top = heap_peek(&h);
    unsigned k = top->key;
    float v = top->val;
    heap_pop(&h);
    printf("h.len=%u, key=%u, val=%f\n", h.len, k, v);
  }

}
int main(int argc, char **argv) {

  int n = argc > 1 ? strtol(argv[1], NULL, 10) : 10;

  makeheap(n);
  popheap(n);

  heap_release(&h);

  return 0;
}
