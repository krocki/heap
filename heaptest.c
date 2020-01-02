#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define randf() (rand() / (RAND_MAX + 1.))
/* todo: make generic */

typedef struct kv {
  unsigned key;
  float val;
} kv;

float kv_compare(const void *a, const void *b) {
  kv *e0 = (kv*)a;
  kv *e1 = (kv*)b;
  return e0->val - e1->val;
}

void swap(kv *a, kv *b) {
  kv c;
  memcpy(&c,  a, sizeof(kv));
  memcpy( a,  b, sizeof(kv));
  memcpy( b, &c, sizeof(kv));
}

typedef struct heap {
  kv *arr;
  unsigned len;
  unsigned cap;
} heap;

void heap_init(heap *h, unsigned cap) {
  h->arr = calloc(cap, sizeof(kv));
}

void heap_resize(heap *h, unsigned cap) {
  h->arr = realloc(h->arr, cap * sizeof(kv));
  h->cap=cap;
}

kv *heap_peek(heap *h) {
  if (h->len > 0) return &h->arr[0];
  return NULL;
}

void heapify_down(heap *h, unsigned idx_parent) {

  unsigned l = idx_parent * 2 + 1;
  unsigned r = idx_parent * 2 + 2;
  kv *ep, *el, *er;

  if (h->len > idx_parent) {
    ep = &h->arr[idx_parent];
  } else return;

  float diff = 0, el_diff, er_diff;
  unsigned swap_with = idx_parent;

  if (l < h->len) {
    el = &h->arr[l];
    el_diff = kv_compare(el, ep);
    if (diff > el_diff) {
      diff = el_diff, swap_with = l;
    }
  }

  if (r < h->len) {
    er = &h->arr[r];
    er_diff = kv_compare(er, ep);
    if (diff > er_diff) {
      diff = er_diff, swap_with = r;
    }
  }

  if (diff != 0) {
    kv *swp = &h->arr[swap_with];
    swap(swp, ep);
    heapify_down(h, swap_with);
  }
}

kv *heap_pop(heap *h) {
  kv *e = &h->arr[h->len-1];
  swap(&h->arr[0], &h->arr[h->len-1]);
  h->len--;
  heapify_down(h, 0);
  return e;
}

void heap_push_back(heap *h, kv *e) {
  if ((h->len+1)>=h->cap)
    heap_resize(h, h->cap==0?1:h->cap*2);
  memcpy(&h->arr[h->len++], e, sizeof(kv));
}

void heapify_up(heap *h, unsigned idx_e0) {

  if (idx_e0 > 0) {
    unsigned idx_e1 = (idx_e0-1)/2;
    kv *e0 = &(h->arr[idx_e0]);
    kv *e1 = &(h->arr[idx_e1]);
    int res = kv_compare(e0, e1) < 0;
    if (res > 0)
      swap(e0, e1);
    heapify_up(h, idx_e1);
  }
}

void heap_insert(heap *h, kv *e) {
  heap_push_back(h, e);
  unsigned idx_e0 = h->len-1;
  heapify_up(h, idx_e0);
}

void heap_print(heap *h) {

  printf("heap: %p\n", (void*)h);
  if (NULL == h) return;

  printf("len: %u, cap: %u\n",
    h->len, h->len);
  for (unsigned i=0; i<h->len; i++)
    printf("[%d] k:%d, v:%f\n",
      i, h->arr[i].key, h->arr[i].val);
}

void heap_release(heap *h) {
  free(h->arr); h->arr = NULL;
  h->cap = 0; h->len = 0;
}

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
