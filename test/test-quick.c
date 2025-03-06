#include "test.h"

#include <stdint.h>
#include <stdio.h>

int main( void )
{
    int32_t rc = 0, arr_len = 5;
    Item    tmp_item;
    Item    array[5];
    DECL_DATA( array[0], 1, "ABC", &array[4] );
    DECL_DATA( array[1], 5, "JXX", NULL );
    DECL_DATA( array[2], 2, "BBC", NULL );
    DECL_DATA( array[3], 4, "MAGA", NULL );
    DECL_DATA( array[4], 1, "DEF", NULL );

    itme_travel( array, arr_len, "Before Sort" );

    rc = surt_quick_r( array,
                       arr_len,
                       sizeof( Item ),
                       item_cmp,
                       itme_travel,
                       NULL,
                       SOPT_DEFAULT );
    if ( rc == -1 ) {
        printf( "Sort Function Call Failed\n" );
    }

    itme_travel( array, arr_len, "After Sort" );

    return 0;
}
