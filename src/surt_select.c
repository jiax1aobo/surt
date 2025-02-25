#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "surt.h"

#ifdef DEBUG_MODE
static int32_t _select_sort_swap_cnt;
#endif /* DEBUG_MODE */

int32_t surt_select(void *arr_ptr, int32_t arr_len, int32_t elem_sz,
                    cmp_fn_p fn_cmp, void *tmp_buff, uint16_t option) {
  void *tmp_elem = tmp_buff;
  char *cmp_elem = NULL, *cur_elem = NULL, *idx_elem;
  bool alloc_flag = false;

#ifdef DEBUG_MODE
  _select_sort_swap_cnt = 0;
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
    for (int32_t i = 0; i < arr_len; i++) {
      idx_elem = (char *)arr_ptr + i * elem_sz;
      cur_elem = idx_elem;
      for (int32_t j = i + 1; j < arr_len; j++) {
        cmp_elem = (char *)arr_ptr + j * elem_sz;
        if (fn_cmp(cur_elem, cmp_elem) > 0) {
          cur_elem = cmp_elem;
        }
      }
      if (idx_elem != cur_elem) {
        surt_swap(elem_sz, idx_elem, cur_elem, tmp_elem);
#ifdef DEBUG_MODE
        _select_sort_swap_cnt++;
#endif
      }
    }
  }

  if (alloc_flag == true) {
    free(tmp_elem);
  }

  return 0;

fatal_malloc:

  return -1; /* call failed */
}
