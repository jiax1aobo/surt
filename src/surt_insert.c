#include "surt.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int32_t _insert_sort_move_insert_cnt;

int32_t surt_insert( void    *arr_ptr,
                     int32_t  arr_len,
                     int32_t  elem_sz,
                     cmp_fn_p fn_cmp,
                     void    *tmp_buff,
                     uint16_t option )
{
    void *tmp_elem = tmp_buff;
    char *cmp_elem = NULL, *swap_elem = NULL, *idx_elem;
    bool  alloc_flag = false;

    if ( tmp_elem == NULL ) {
        tmp_elem = malloc( elem_sz );
        if ( tmp_elem == NULL ) {
            goto fatal_malloc;
        }
        alloc_flag = true;
    }

    if ( ( option & SOPT_ORDER_MASK ) == SOPT_INVERSE_ORDER ) {
        /** @todo */
    } else {
        for ( int32_t i = 1; i < arr_len; i++ ) {
            idx_elem = (char *) arr_ptr + i * elem_sz;
            for ( int32_t j = i - 1; j >= 0; j-- ) {
                cmp_elem  = (char *) arr_ptr + j * elem_sz;
                swap_elem = cmp_elem + elem_sz;
                if ( fn_cmp( cmp_elem, idx_elem ) <= 0 &&
                     swap_elem < idx_elem ) {
                    surt_move_insert( elem_sz * ( i - j - 1 ),
                                      swap_elem,
                                      elem_sz,
                                      idx_elem,
                                      tmp_elem );
#ifdef DEBUG_MODE
                    _insert_sort_move_insert_cnt++;
#endif
                    break;
                } else {
                    continue;
                }
            }
        }
    }

    if ( alloc_flag == true ) {
        free( tmp_elem );
    }

    return 0;

fatal_malloc:

    return -1; /* call failed */
}
