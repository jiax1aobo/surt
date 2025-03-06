#include "surt.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CMP( _arr, _idx1, _idx2 ) \
    fn_cmp( _arr + elem_sz * _idx1, _arr + elem_sz * _idx2 )

int32_t surt_quick_r( void    *arr_ptr,
                      uint32_t arr_len,
                      uint32_t elem_sz,
                      cmp_fn_p fn_cmp,
                      trv_fn_p fn_trv,
                      void    *tmp_buff,
                      uint16_t option )
{
    uint32_t low = 0, mid = arr_len / 2, high = arr_len - 1;
    uint32_t i = low, j = high, k;
    char    *tmp_elem   = tmp_buff;
    bool     alloc_flag = false;

    /* pick up pivot */
    if ( CMP( arr_ptr, low, high ) < 0 ) {
        if ( CMP( arr_ptr, high, mid ) < 0 ) {
            k = high;
        } else if ( CMP( arr_ptr, low, mid ) < 0 ) {
            k = mid;
        } else {
            k = low;
        }
    } else {
        if ( CMP( arr_ptr, low, mid ) < 0 ) {
            k = low;
        } else if ( CMP( arr_ptr, high, mid ) < 0 ) {
            k = mid;
        } else {
            k = high;
        }
    }

    if ( tmp_elem == NULL ) {
        tmp_elem = (char *) malloc( elem_sz );
        if ( tmp_elem == NULL ) {
            goto fatal_malloc_error;
        }
        alloc_flag = true;
    }

    if ( ( option & SOPT_ORDER_MASK ) == SOPT_INVERSE_ORDER ) {
        /** @todo */
    } else {
        while ( i <= j ) {
            while ( CMP( arr_ptr, i, k ) < 0 ) {
                i++;
            }
            while ( CMP( arr_ptr, k, j ) < 0 ) {
                j--;
            }
            if ( i <= j ) {
                surt_swap( elem_sz,
                           arr_ptr + elem_sz * i,
                           arr_ptr + elem_sz * j,
                           tmp_elem );
                i++;
                j--;
            }
        }
    }

    if ( low < j ) {
#ifdef DEBUG_MODE
        if ( surt_quick_r( arr_ptr + elem_sz * low,
                           j - low + 1,
                           elem_sz,
                           fn_cmp,
                           fn_trv,
                           tmp_elem,
                           option ) != 0 ) {
            goto fatal_call_error;
        }
#else  /* DEBUG_MODE */
        surt_quick_r( arr_ptr + elem_sz * low,
                      j - low + 1,
                      elem_sz,
                      fn_cmp,
                      fn_trv,
                      tmp_elem,
                      option );
#endif /* DEBUG_MODE */
    }

    if ( i < high ) {
#ifdef DEBUG_MODE
        if ( surt_quick_r( arr_ptr + elem_sz * i,
                           high - i + 1,
                           elem_sz,
                           fn_cmp,
                           fn_trv,
                           tmp_elem,
                           option ) != 0 ) {
            goto fatal_call_error;
        }
#else  /* DEBUG_MODE */
        surt_quick_r( arr_ptr + elem_sz * i,
                      high - i + 1,
                      elem_sz,
                      fn_cmp,
                      fn_trv,
                      tmp_elem,
                      option );
#endif /* DEBUG_MODE */
    }

    if ( alloc_flag == true ) {
        free( tmp_elem );
    }

    return 0;

fatal_call_error:
    if ( alloc_flag == true ) {
        free( tmp_elem );
    }

fatal_malloc_error:

    return -1;
}
