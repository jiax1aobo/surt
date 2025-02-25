#include "surt.h"
#include "test.h"

#include <stdint.h>
#include <stdio.h>

int main(void) {
  int32_t rc = 0;
  Item tmp_item;
  Item array[5];
  DECL_DATA(array[0], 1, "ABC", &array[4]);
  DECL_DATA(array[1], 5, "JXX", NULL);
  DECL_DATA(array[2], 2, "BBC", NULL);
  DECL_DATA(array[3], 4, "MAGA", NULL);
  DECL_DATA(array[4], 1, "DEF", NULL);

  SHOW_ARRAY("Before Sort", array, 5);

  rc = surt_select(array, 5, sizeof(Item), item_cmp, &tmp_item, SOPT_DEFAULT);
  if (rc == -1) {
    printf("Sort Function Call Failed\n");
  }

  SHOW_ARRAY("After Sort", array, 5);

  return 0;
}