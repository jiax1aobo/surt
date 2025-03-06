#include "surt.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_MODE
    #include <stdio.h>
static int32_t  msg_len;
static char     _merge_sort_msg_buff[128];
static uint32_t _merge_sort_divide_cnt;
static uint32_t _merge_sort_conquer_cnt;
#endif /* DEBUG_MODE */

typedef struct MergeInfo {
    char    *surt_obj;
    uint32_t obj_sz;
    uint32_t obj_len;
    uint32_t abs_begin;
    uint32_t abs_middle;
    uint32_t abs_end;
    uint16_t option;
    cmp_fn_p fn_cmp;
    trv_fn_p fn_trv;
    char    *tmp_buff;
} MergeInfo;

#define SET_SURT_INFO( _info,               \
                       _obj,                \
                       _sz,                 \
                       _len,                \
                       _begin,              \
                       _mid,                \
                       _end,                \
                       _opt,                \
                       _fcmp,               \
                       _ftrv,               \
                       _buf )               \
    do {                                    \
        ( _info )->surt_obj   = ( _obj );   \
        ( _info )->obj_sz     = ( _sz );    \
        ( _info )->obj_len    = ( _len );   \
        ( _info )->abs_begin  = ( _begin ); \
        ( _info )->abs_middle = ( _mid );   \
        ( _info )->abs_end    = ( _end );   \
        ( _info )->option     = ( _opt );   \
        ( _info )->fn_cmp     = ( _fcmp );  \
        ( _info )->fn_trv     = ( _ftrv );  \
        ( _info )->tmp_buff   = ( _buf );   \
    } while ( 0 )

/**
 * @brief 归并两个子序列
 *
 * @param[in] info 归并信息
 * @note 由于 info->tmp_buff 是原序列的一半大小，
 * <BR>  因此在归并时，对 info->tmp_buff 的使用一定要用相对的索引
 */
static void surt_merge_conquer( MergeInfo *info )
{
    uint32_t idx1 = info->abs_begin, idx2 = info->abs_middle + 1,
             idx3 = info->abs_begin, offset = info->abs_begin;
    char *ans = info->surt_obj;

#ifdef DEBUG_MODE
    _merge_sort_conquer_cnt++;
#endif /* DEBUG_MODE */

    memcpy( info->tmp_buff,
            info->surt_obj + idx1 * info->obj_sz,
            info->obj_sz * ( info->abs_middle - info->abs_begin + 1 ) );

    if ( ( info->option & SOPT_ORDER_MASK ) == SOPT_INVERSE_ORDER ) {
        /** @todo */
    } else {
        while ( idx1 <= info->abs_middle && idx2 <= info->abs_end ) {
            if ( info->fn_cmp( info->tmp_buff +
                                   ( idx1 - offset ) * info->obj_sz,
                               info->surt_obj + idx2 * info->obj_sz ) <= 0 ) {
                memcpy( ans + idx3 * info->obj_sz,
                        info->tmp_buff + ( idx1 - offset ) * info->obj_sz,
                        info->obj_sz );
                idx3++;
                idx1++;
            } else {
                memcpy( ans + idx3 * info->obj_sz,
                        info->surt_obj + idx2 * info->obj_sz,
                        info->obj_sz );
                idx3++;
                idx2++;
            }
        }

        if ( idx1 <= info->abs_middle ) {
            memcpy( ans + idx3 * info->obj_sz,
                    info->tmp_buff + ( idx1 - offset ) * info->obj_sz,
                    info->obj_sz * ( info->abs_middle - idx1 + 1 ) );
            idx3 += info->abs_middle - idx1 + 1;
        }

        if ( idx2 <= info->abs_end ) {
            memcpy( ans + idx3 * info->obj_sz,
                    info->surt_obj + idx2 * info->obj_sz,
                    info->obj_sz * ( info->abs_end - idx2 + 1 ) );
            idx3 += info->abs_end - idx2 + 1;
        }
    }
}

static void surt_merge_divide( MergeInfo *info )
{
    uint32_t low = info->abs_begin, mid = info->abs_middle,
             high = info->abs_end;
    MergeInfo tmp_info;

#ifdef DEBUG_MODE
    _merge_sort_divide_cnt++;
#endif /* DEBUG_MODE */

    if ( low < high ) {
        SET_SURT_INFO( &tmp_info,
                       info->surt_obj,
                       info->obj_sz,
                       info->obj_len,
                       low,
                       low + ( mid - low ) / 2,
                       mid,
                       info->option,
                       info->fn_cmp,
                       info->fn_trv,
                       info->tmp_buff );
        surt_merge_divide( &tmp_info );

        SET_SURT_INFO( &tmp_info,
                       info->surt_obj,
                       info->obj_sz,
                       info->obj_len,
                       mid + 1,
                       mid + 1 + ( high - mid - 1 ) / 2,
                       high,
                       info->option,
                       info->fn_cmp,
                       info->fn_trv,
                       info->tmp_buff );
        surt_merge_divide( &tmp_info );

        surt_merge_conquer( info );

#ifdef DEBUG_MODE
        if ( info->fn_trv != NULL ) {
            msg_len                       = snprintf( _merge_sort_msg_buff,
                                128,
                                "Conquer %3d",
                                _merge_sort_conquer_cnt );
            _merge_sort_msg_buff[msg_len] = '\0';
            info->fn_trv( info->surt_obj, info->obj_len, _merge_sort_msg_buff );
        }
#endif /* DEBUG_MODE */
    }
}

int32_t surt_merge_r( void    *arr_ptr,
                      uint32_t arr_len,
                      uint32_t elem_sz,
                      cmp_fn_p fn_cmp,
                      trv_fn_p fn_trv,
                      void    *tmp_buff,
                      uint16_t option )
{
    uint32_t  low = 0, mid = arr_len / 2, high = arr_len - 1;
    char     *tmp_elems  = tmp_buff;
    bool      alloc_flag = false;
    MergeInfo info;

#ifdef DEBUG_MODE
    _merge_sort_divide_cnt  = 0;
    _merge_sort_conquer_cnt = 0;
#endif /* DEBUG_MODE */

    if ( arr_len > 1 ) {
        if ( tmp_elems == NULL ) {
            tmp_elems = (char *) malloc( elem_sz * ( arr_len / 2 + 1 ) );
            if ( tmp_elems == NULL ) {
                goto fatal_malloc;
            }
            alloc_flag = true;
        }

        SET_SURT_INFO( &info,
                       arr_ptr,
                       elem_sz,
                       arr_len,
                       low,
                       mid,
                       high,
                       option,
                       fn_cmp,
                       fn_trv,
                       tmp_elems );

        surt_merge_divide( &info );
    }

    if ( alloc_flag == true ) {
        free( tmp_elems );
    }

    return 0;

fatal_malloc:

    return -1;
}
