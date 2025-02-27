#ifndef _SURT_H_
#define _SURT_H_ 1

#include <stdint.h>
#include <string.h>

static const uint16_t SOPT_DEFAULT = 0x0000;

static const uint16_t SOPT_INVERSE_ORDER = 0x8000;
static const uint16_t SOPT_ORDER_MASK = 0x8000;

typedef int64_t (*cmp_fn_p)(const void *a, const void *b);
typedef void (*trv_fn_p)(void *arr, uint32_t len, const char *msg);

#define surt_swap(_sz, _a, _b, _tmp)                                           \
  do {                                                                         \
    memcpy(_tmp, _b, _sz);                                                     \
    memcpy(_b, _a, _sz);                                                       \
    memcpy(_a, _tmp, _sz);                                                     \
  } while (0)

#define surt_move_insert(m_sz, mv, sz, b, tmp)                                 \
  do {                                                                         \
    memcpy(tmp, b, sz);                                                        \
    memcpy(mv + sz, mv, m_sz);                                                 \
    memcpy(mv, tmp, sz);                                                       \
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

int32_t surt_merge_r(void *arr_ptr, uint32_t arr_len, uint32_t elem_sz,
                     cmp_fn_p fn_cmp, trv_fn_p fn_trv, void *tmp_buff,
                     uint16_t option);

/*****************
 * Unstable Sort
 *****************/

//! @todo quick

//! @todo heap

#endif /* _SURT_H_ */