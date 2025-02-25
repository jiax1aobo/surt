#ifndef _SURT_H_
#define _SURT_H_ 1

#include <stdint.h>
#include <string.h>

static const uint16_t SOPT_DEFAULT = 0x0000;

static const uint16_t SOPT_INVERSE_ORDER = 0x8000;
static const uint16_t SOPT_ORDER_MASK = 0x8000;

typedef int64_t (*cmp_fn_p)(const void *a, const void *b);

#define surt_swap(_sz, _a, _b, _tmp)                                           \
  do {                                                                         \
    memcpy(_tmp, _b, _sz);                                                     \
    memcpy(_b, _a, _sz);                                                       \
    memcpy(_a, _tmp, _sz);                                                     \
  } while (0)

#define surt_move_insert(m_sz, m, sz, b, tmp)                                  \
  do {                                                                         \
    memcpy(tmp, b, sz);                                                        \
    memcpy(m + sz, m, m_sz);                                                   \
    memcpy(m, tmp, sz);                                                        \
  } while (0)
/**********************
 * Assistant Function
 **********************/

/***************
 * Stable Sort
 ***************/

int32_t surt_bubble(void *arr_ptr, int32_t arr_len, int32_t elem_sz,
                    cmp_fn_p fn_cmp, void *tmp_buff, uint16_t option);

int32_t surt_select(void *arr_ptr, int32_t arr_len, int32_t elem_sz,
                    cmp_fn_p fn_cmp, void *tmp_buff, uint16_t option);

int32_t surt_insert(void *arr_ptr, int32_t arr_len, int32_t elem_sz,
                    cmp_fn_p fn_cmp, void *tmp_buff, uint16_t option);

//! @todo merge

//! @todo merge recursive

/*****************
 * Unstable Sort
 *****************/

//! @todo quick

//! @todo heap

#endif /* _SURT_H_ */