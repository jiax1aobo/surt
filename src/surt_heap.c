#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "surt.h"

/**
 * @brief Heap
 */
typedef struct Heap {
  bool flag;        /**< true for min heap, false for max heap */
  uint32_t size;    /**< heap current size */
  uint32_t max_sz;  /**< heap max size */
  uint32_t elem_sz; /**< element size in byte */
  char *payload;    /**< heap element array */
  cmp_fn_p fn_cmp;  /**< heap element compare function */
  char *tmp_buff;   /**< swap buffer */
} Heap;

#define LCHILD_IDX(_idx) (2 * (_idx) + 1)
#define RCHILD_IDX(_idx) (2 * (_idx) + 2)
#define PARENT_IDX(_idx) (((_idx)-1) / 2)
#define HEAP_EMPTY(_h) ((_h)->size == 0 ? true : false)

// inline char *peek(Heap *h) { return h->payload; }

// void sift_bottom_up(Heap *h, int32_t idx) {
//   int32_t i = idx;
//   int32_t p;
//   char *arr = h->payload;
//   if (h->flag == true) {
//     while (true) {
//       p = PARENT_IDX(i);
//       if ((p < 0) ||
//           (h->fn_cmp(arr + h->elem_sz * p, arr + h->elem_sz * i) <= 0)) {
//         break;
//       }
//       surt_swap(h->elem_sz, arr + h->elem_sz * p, arr + h->elem_sz * i,
//                 h->tmp_buff);
//       i = p;
//     }
//   } else {
//     while (true) {
//       p = PARENT_IDX(i);
//       if ((p < 0) ||
//           (h->fn_cmp(arr + h->elem_sz * p, arr + h->elem_sz * i) >= 0)) {
//         break;
//       }
//       surt_swap(h->elem_sz, arr + h->elem_sz * p, arr + h->elem_sz * i,
//                 h->tmp_buff);
//       i = p;
//     }
//   }
// }

// int32_t push(Heap *h, char *elem) {
//   if (h->size == h->max_sz) {
//     return -1;
//   }
//   memcpy(h->payload + h->elem_sz * h->size, elem, h->elem_sz);
//   h->size++;
//   sift_bottom_up(h, h->size - 1);
//   return 0;
// }

/**
 * @brief Top-Down Heapify
 * <BR>   这是局部子树的堆化，分为大堆或小堆的堆化两种
 * <BR>   @b 小堆的堆化 ：首先假设当前堆顶是堆中的最小元素
 * <BR>         1. 接着在左右子节点（存在的情况下）和堆顶之间
 *                 进行比较获取真正最小的元素
 * <BR>         2. 如果真正最小的元素就是一开始假设堆顶，
 *                 那么说明当前子堆已经完成了堆化了，可以退出了
 *                 （不用考虑子节点以下的那些子堆，因为 @a top-down-heapify
 *                 就是从子节点以下的子堆往上做的，所以那些子堆早已堆化了）
 * <BR>         3. 否则，就把真正最小的元素（左或右子节点）和堆顶元素交换
 *                 然后把交换后的原堆顶假设为新的最小元素再次从步骤 1 开始执行
 * <BR>   @b 大堆的堆化 ：和小堆的堆化类似的，只是以假设堆顶为最大元素为起始条件
 *
 * @param[in] h     堆
 * @param[in] idx   要执行堆化的子堆的堆顶位置
 */
void sift_top_down(Heap *h, int32_t idx) {
  int32_t i = idx, l, r, min = idx, max = idx;
  char *arr = h->payload;

  if (h->flag == true) {
    /* min heap */
    while (true) {
      l = LCHILD_IDX(i);
      r = RCHILD_IDX(i);
      if ((l < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * l, arr + h->elem_sz * min) < 0)) {
        min = l;
      }
      if ((r < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * r, arr + h->elem_sz * min) < 0)) {
        min = r;
      }
      if (min == i) {
        break;
      }

      surt_swap(h->elem_sz, arr + h->elem_sz * i, arr + h->elem_sz * min,
                h->tmp_buff);

      i = min;
    }
  } else {
    /* max heap */
    while (true) {
      l = LCHILD_IDX(i);
      r = RCHILD_IDX(i);
      if ((l < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * max, arr + h->elem_sz * l) < 0)) {
        max = l;
      }
      if ((r < h->size) &&
          (h->fn_cmp(arr + h->elem_sz * max, arr + h->elem_sz * r) < 0)) {
        max = r;
      }
      if (max == i) {
        break;
      }
      surt_swap(h->elem_sz, arr + h->elem_sz * i, arr + h->elem_sz * max,
                h->tmp_buff);
      i = max;
    }
  }
}

/**
 * @brief Pop Heap Top
 * <BR>   将堆顶元素与堆最后一个元素交换
 * <BR>   然后把最后一个元素排除，从堆顶开始执行堆化
 *
 * @param[in]  h    堆
 * @param[out] top  被弹出的堆顶元素
 * @return int32_t 堆为空时返回 -1 表示失败，否则返回 0 表示成功
 */
int32_t pop(Heap *h, void **top) {
  if (HEAP_EMPTY(h) == true) {
    return -1;
  }
  surt_swap(h->elem_sz, h->payload, h->payload + h->elem_sz * (h->size - 1),
            h->tmp_buff);
  if (top != NULL) {
    *top = h->payload + h->elem_sz * (h->size - 1);
  }
  h->size--;
  sift_top_down(h, 0);
  return 0;
}

/**
 * @brief Build Heap
 * <BR>   建堆的核心是自下而上地把每个局部的子树堆化，
 * <BR>   这样一直到把堆顶为根的子堆（也就是整个堆）也堆化，整个堆就建好了。
 * <BR>   这个堆化的过程（很自然地）应该从最后一个有子节点的子堆开始，
 *
 * @param[out] h        堆
 * @param[in]  arr      建堆的数组
 * @param[in]  len      数组长度
 * @param[in]  elem_sz  数组成员大小
 * @param[in]  fn_cmp   成员比较函数
 * @param[in]  tmp_buff 交换用的缓冲区
 * @param[in]  flag     真表示小顶堆，假表示大顶堆
 * @return true  建堆成功
 * @return false 建堆失败（内存分配失败）
 */
bool surt_build_heap(Heap **h, void *arr, uint32_t len, uint32_t elem_sz,
                     cmp_fn_p fn_cmp, char *tmp_buff, bool flag) {
  Heap *addr = NULL;

  addr = (Heap *)malloc(sizeof(Heap));
  if (addr == NULL) {
    goto fatal_malloc;
  }

  *h = addr;
  (*h)->flag = flag;
  (*h)->size = len;
  (*h)->max_sz = len;
  (*h)->elem_sz = elem_sz;
  (*h)->payload = arr;
  (*h)->fn_cmp = fn_cmp;
  (*h)->tmp_buff = tmp_buff;

  for (int32_t i = PARENT_IDX(len - 1); i >= 0; i--) {
    sift_top_down(*h, i);
  }

  return true;

fatal_malloc:

  return false;
}

void surt_destroy_heap(Heap *h) { free(h); }

/**
 * @brief heap sort
 * <BR>   堆排序的原理是利用堆这种数据结构的特点（堆的数组实现）
 * <BR>   1. 堆顶永远是最大/最小元素
 * <BR>   2. 元素出堆（Pop）后，会把堆顶元素放到堆的末尾
 * <BR>   具体的方法是：
 * <BR>   @b 正序排序（从小到大） ：创建一个大顶堆，然后不停地弹出堆顶元素
 *                因为每次弹出都会导致堆中当前最大值到堆（数组）的末尾
 *                这样等所有元素都弹出后，数组倒着看就是从大到小，也就是正序
 * <BR>   @b 逆序排序（从大到小） ：原理与正序排序类似，只是曾经的是小顶堆
 *
 * @param[in,out] arr_ptr   要排序的数组
 * @param[in]     arr_len   数组长度
 * @param[in]     elem_sz   数组成员大小
 * @param[in]     fn_cmp    成员比较函数
 * @param[in]     fn_trv    成员打印函数  可为空
 * @param[in]     tmp_buff  替换用的缓冲区（可为空，至少为一个成员大小）
 * @param[in]     option    替换选项
 * @return int32_t 返回 0 表示排序成功，返回 -1 表示排序失败（内存分配失败）
 */
int32_t surt_heap(void *arr_ptr, uint32_t arr_len, uint32_t elem_sz,
                  cmp_fn_p fn_cmp, trv_fn_p fn_trv, void *tmp_buff,
                  uint16_t option) {
  char *ans = arr_ptr, *tmp_elem = tmp_buff, *top = NULL;
  bool alloc_flag = false;
  Heap *heap = NULL;

  if (tmp_elem == NULL) {
    tmp_elem = (char *)malloc(elem_sz);
    if (tmp_elem == NULL) {
      goto fatal_malloc_error;
    }
    alloc_flag = true;
  }

  if ((option & SOPT_ORDER_MASK) == SOPT_INVERSE_ORDER) {
    if (surt_build_heap(&heap, arr_ptr, arr_len, elem_sz, fn_cmp, tmp_elem,
                        true) != true) {
      goto failure_build_heap;
    }
    for (int32_t i = 0; i < arr_len; i++) {
#ifdef DEBUG_MODE
      if (pop(heap, NULL) != 0) {
        goto fatal_call_error;
      };
#else  /* DEBUG_MODE */
      pop(heap, NULL);
#endif /* DEBUG_MODE */
    }
  } else {
    if (surt_build_heap(&heap, arr_ptr, arr_len, elem_sz, fn_cmp, tmp_elem,
                        false) != true) {
      goto failure_build_heap;
    }
    for (int32_t i = 0; i < arr_len; i++) {
#ifdef DEBUG_MODE
      if (pop(heap, NULL) != 0) {
        goto fatal_call_error;
      }
#else  /* DEBUG_MODE */
      pop(heap, NULL);
#endif /* DEBUG_MODE */
    }
  }

  surt_destroy_heap(heap);

failure_build_heap:

  if (alloc_flag == true) {
    free(tmp_elem);
  }

  return 0;

#ifdef DEBUG_MODE
fatal_call_error:
  surt_destroy_heap(heap);
  if (alloc_flag == true) {
    free(tmp_elem);
  }
#endif /* DEBUG_MODE */

fatal_malloc_error:

  return -1;
}