#ifndef _TEST_H_
#define _TEST_H_ 1

#include "surt.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Item {
    int32_t      id;
    char        *data;
    struct Item *next;
} Item;

#define DECL_DATA( _item, _id, _dat, _nxt ) \
    do {                                    \
        ( _item ).id   = ( _id );           \
        ( _item ).data = ( _dat );          \
        ( _item ).next = ( _nxt );          \
    } while ( 0 )

#define SHOW_ARRAY( _msg, _arr, _len )                                        \
    do {                                                                      \
        printf( "%s:\n", _msg );                                              \
        for ( int32_t i = 0; i < ( _len ); i++ ) {                            \
            printf( "\tItem(%d) : {%d, %s}\n", i, _arr[i].id, _arr[i].data ); \
        }                                                                     \
    } while ( 0 )

inline static int64_t item_cmp( const void *item1, const void *item2 )
{
#ifdef DEBUG_MODE
    int64_t res = ( (Item *) item1 )->id - ( (Item *) item2 )->id;
    return res;
#else
    return ( ( (Item *) item1 )->id - ( (Item *) item2 )->id );
#endif
}

static void itme_travel( void *arr, uint32_t len, const char *msg )
{
    printf( "%s:\t", msg );
    for ( int32_t i = 0; i < len; i++ ) {
        printf( "[%d]={%d,%s} ",
                i,
                ( (Item *) arr )[i].id,
                ( (Item *) arr )[i].data );
    }
    printf( "\n" );
}

#endif /* _TEST_H_ */
