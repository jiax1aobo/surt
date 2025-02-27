#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "surt.h"

#ifdef DEBUG_MODE
static uint32_t _bubble_sort_pass_cnt;
static uint32_t _bubble_sort_swap_cnt;
#endif /* DEBUG_MODE */

int32_t surt_bubble(void *arr_ptr, int32_t arr_len, int32_t elem_sz,
                    cmp_fn_p fn_cmp, void *tmp_buff, uint16_t option) {
  void *tmp_elem = tmp_buff;
  char *first = NULL, *second = NULL;
  bool swap_flag = true, alloc_flag = false;

#ifdef DEBUG_MODE
  _bubble_sort_pass_cnt = 0;
  _bubble_sort_swap_cnt = 0;
#endif /* DEBUG_MODE */

  if (tmp_elem == NULL) {
    tmp_elem = malloc(elem_sz);
    if (tmp_elem == NULL) {
      goto fatal_malloc;
    }
    alloc_flag = true;
  }

  if ((option & SOPT_ORDER_MASK) == SOPT_INVERSE_ORDER) {
    /** @todo */
  } else {
    for (int32_t i = 0; i < arr_len - 1 && swap_flag == true; i++) {
      swap_flag = false;
      for (int32_t j = 0; j < arr_len - i - 1; j++) {
        first = (char *)arr_ptr + j * elem_sz;
        second = first + elem_sz;
        if (fn_cmp(first, second) > 0) {
          swap_flag = true;
          surt_swap(elem_sz, first, second, tmp_elem);
#ifdef DEBUG_MODE
          _bubble_sort_swap_cnt++;
#endif /* DEBUG_MODE */
        }
      }
#ifdef DEBUG_MODE
      _bubble_sort_pass_cnt++;
#endif /* DEBUG_MODE */
    }
  }

  if (alloc_flag == true) {
    free(tmp_elem);
  }

  return 0;

fatal_malloc:

  return -1; /* call failed */
}
