#ifndef _TEST_H_
#define _TEST_H_ 1

#include <stdint.h>

typedef struct Item {
  int32_t id;
  char *data;
  struct Item *next;
} Item;

#define DECL_DATA(_item, _id, _dat, _nxt)                                      \
  do {                                                                         \
    (_item).id = (_id);                                                        \
    (_item).data = (_dat);                                                     \
    (_item).next = (_nxt);                                                     \
  } while (0)

#define SHOW_ARRAY(_msg, _arr, _len)                                           \
  do {                                                                         \
    printf("%s:\n", _msg);                                                     \
    for (int32_t i = 0; i < (_len); i++) {                                     \
      printf("\tItem(%d) : {%d, %s}\n", i, _arr[i].id, _arr[i].data);          \
    }                                                                          \
  } while (0)

static inline int64_t item_cmp(const void *item1, const void *item2) {
#ifdef DEBUG_MODE
  int64_t res = ((Item *)item1)->id - ((Item *)item2)->id;
  return res;
#else
  return (((Item *)item1)->id - ((Item *)item2)->id);
#endif
}

#endif /* _TEST_H_ */