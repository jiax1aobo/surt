#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "surt.h"

#define CMP(_arr, _idx1, _idx2)                                                \
  fn_cmp(_arr + elem_sz * _idx1, _arr + elem_sz * _idx2)

typedef struct Heap {
  bool flag; /**< true for min heap, false for max heap */
  uint32_t size;
  uint32_t max_sz;
  uint32_t elem_sz;
  char *payload;
  cmp_fn_p fn_cmp;
  char *tmp_buff;
} Heap;

#define LCHILD_IDX(_idx) (2 * (_idx) + 1)
#define RCHILD_IDX(_idx) (2 * (_idx) + 2)
#define PARENT_IDX(_idx) (((_idx)-1) / 2)
#define HEAP_EMPTY(_h) ((_h)->size == 0 ? true : false)

// inline char *peek(Heap *h) { return h->payload; }

// void sift_bottom_up(Heap *h, int32_t idx) {
//   int32_t i = idx;
//   int32_t p;
//   char *arr = h->payload;
//   if (h->flag == true) {
//     while (true) {
//       p = PARENT_IDX(i);
//       if ((p < 0) ||
//           (h->fn_cmp(arr + h->elem_sz * p, arr + h->elem_sz * i) <= 0)) {
//         break;
//       }
//       surt_swap(h->elem_sz, arr + h->elem_sz * p, arr + h->elem_sz * i,
//                 h->tmp_buff);
//       i = p;
//     }
//   } else {
//     while (true) {
//       p = PARENT_IDX(i);
//       if ((p < 0) ||
//           (h->fn_cmp(arr + h->elem_sz * p, arr + h->elem_sz * i) >= 0)) {
//         break;
//       }
//       surt_swap(h->elem_sz, arr + h->elem_sz * p, arr + h->elem_sz * i,
//                 h->tmp_buff);
//       i = p;
//     }
//   }
// }

// int32_t push(Heap *h, char *elem) {
//   if (h->size == h->max_sz) {
//     return -1;
//   }
//   memcpy(h->payload + h->elem_sz * h->size, elem, h->elem_sz);
//   h->size++;
//   sift_bottom_up(h, h->size - 1);
//   return 0;
// }

void sift_top_down(Heap *h, int32_t idx) {
  int32_t i = idx, l, r, min = idx, max = idx;
  char *arr = h->payload;

  if (h->flag == true) {
    /* min heap */
    while (true) {
      l = LCHILD_IDX(i);
      r = RCHILD_IDX(i);
      if ((l < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * l, arr + h->elem_sz * min) < 0)) {
        min = l;
      }
      if ((r < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * r, arr + h->elem_sz * min) < 0)) {
        min = r;
      }
      if (min == i) {
        break;
      }

      surt_swap(h->elem_sz, arr + h->elem_sz * i, arr + h->elem_sz * min,
                h->tmp_buff);

      i = min;
    }
  } else {
    /* max heap */
    while (true) {
      l = LCHILD_IDX(i);
      r = RCHILD_IDX(i);
      if ((l < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * max, arr + h->elem_sz * l) < 0)) {
        max = l;
      }
      if ((r < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * max, arr + h->elem_sz * r) < 0)) {
        max = r;
      }
      if (max == i) {
        break;
      }
      surt_swap(h->elem_sz, arr + h->elem_sz * i, arr + h->elem_sz * max,
                h->tmp_buff);
      i = max;
    }
  }
}

int32_t pop(Heap *h, void **top) {
  if (HEAP_EMPTY(h) == true) {
    return -1;
  }
  surt_swap(h->elem_sz, h->payload, h->payload + h->elem_sz * (h->size - 1),
            h->tmp_buff);
  if (top != NULL) {
    *top = h->payload + h->elem_sz * (h->size - 1);
  }
  h->size--;
  sift_top_down(h, 0);
  return 0;
}

bool surt_build_heap(Heap **h, void *arr, uint32_t len, uint32_t elem_sz,
                     cmp_fn_p fn_cmp, char *tmp_buff, bool flag) {
  Heap *addr = NULL;

  addr = (Heap *)malloc(sizeof(Heap));
  if (addr == NULL) {
    goto fatal_malloc;
  }

  *h = addr;
  (*h)->flag = flag;
  (*h)->size = len;
  (*h)->max_sz = len;
  (*h)->elem_sz = elem_sz;
  (*h)->payload = arr;
  (*h)->fn_cmp = fn_cmp;
  (*h)->tmp_buff = tmp_buff;

  for (int32_t i = PARENT_IDX(len - 1); i >= 0; i--) {
    sift_top_down(*h, i);
  }

  return true;

fatal_malloc:

  return false;
}

void surt_destroy_heap(Heap *h) { free(h); }

int32_t surt_heap(void *arr_ptr, uint32_t arr_len, uint32_t elem_sz,
                  cmp_fn_p fn_cmp, trv_fn_p fn_trv, void *tmp_buff,
                  uint16_t option) {
  char *ans = arr_ptr, *tmp_elem = tmp_buff, *top = NULL;
  bool alloc_flag = false;
  Heap *heap = NULL;

  if (tmp_elem == NULL) {
    tmp_elem = (char *)malloc(elem_sz);
    if (tmp_elem == NULL) {
      goto fatal_malloc_error;
    }
    alloc_flag = true;
  }

  if ((option & SOPT_ORDER_MASK) == SOPT_INVERSE_ORDER) {
    if (surt_build_heap(&heap, arr_ptr, arr_len, elem_sz, fn_cmp, tmp_elem,
                        false) == false) {
      goto failure_build_heap;
    }
    for (int32_t i = 0; i < arr_len; i++) {
#ifdef DEBUG_MODE
      if (pop(heap, NULL) != 0) {
        goto fatal_call_error;
      };
#else  /* DEBUG_MODE */
      pop(heap, NULL);
#endif /* DEBUG_MODE */
    }
  } else {
    if (surt_build_heap(&heap, arr_ptr, arr_len, elem_sz, fn_cmp, tmp_elem,
                        true) == false) {
      goto failure_build_heap;
    }
    for (int32_t i = 0; i < arr_len; i++) {
#ifdef DEBUG_MODE
      if (pop(heap, NULL) != 0) {
        goto fatal_call_error;
      }
#else  /* DEBUG_MODE */
      pop(heap, NULL);
#endif /* DEBUG_MODE */
    }
  }

  surt_destroy_heap(heap);

failure_build_heap:

  if (alloc_flag == true) {
    free(tmp_elem);
  }

  return 0;

#ifdef DEBUG_MODE
fatal_call_error:
  surt_destroy_heap(heap);
  if (alloc_flag == true) {
    free(tmp_elem);
  }
#endif /* DEBUG_MODE */

fatal_malloc_error:

  return -1;
}