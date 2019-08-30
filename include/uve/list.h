#ifndef __BQ_LIST_H__
#define __BQ_LIST_H__ 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* uve_list_t[2];

#define uve_list_next(q) (*(uve_list_t **)&((*(q))[0]))
#define uve_list_prev(q) (*(uve_list_t **)&((*(q))[1]))
#define uve_list_prev_next(q) (uve_list_next(uve_list_prev(q)))
#define uve_list_next_prev(q) (uve_list_prev(uve_list_next(q)))

#define uve_list_data(ptr, type, field) \
  ((type *)((char *)(ptr)-offsetof(type, field)))

#define uve_list_empty(q) \
  ((const uve_list_t *)(q) == (const uve_list_t *)uve_list_next(q))

#define uve_list_init(q)    \
  do {                     \
    uve_list_next(q) = (q); \
    uve_list_prev(q) = (q); \
  } while (0)

#define uve_list_insert_after(h, q)      \
  do {                                 \
    uve_list_next(q) = uve_list_next(h); \
    uve_list_prev(q) = (h);             \
    uve_list_next_prev(q) = (q);        \
    uve_list_next(h) = (q);             \
  } while (0)

#define uve_list_insert_before(h, q)     \
  do {                                 \
    uve_list_next(q) = (h);             \
    uve_list_prev(q) = uve_list_prev(h); \
    uve_list_prev_next(q) = (q);        \
    uve_list_prev(h) = (q);             \
  } while (0);

#define uve_list_replace(o, n)          \
  do {                                 \
    uve_list_prev_next(o) = (n);        \
    uve_list_next_prev(o) = (n);        \
    uve_list_prev(n) = uve_list_prev(o); \
    uve_list_next(n) = uve_list_next(o); \
  } while (0)

#define uve_list_concat(h1, h2)                \
  do {                                        \
    uve_list_prev_next(h1) = uve_list_next(h2); \
    uve_list_next_prev(h2) = uve_list_prev(h1); \
    uve_list_prev(h1) = uve_list_prev(h2);      \
    uve_list_prev_next(h2) = (h1);             \
    uve_list_init(h2);                         \
  } while (0)

#define uve_list_remove(q)                   \
  do {                                      \
    uve_list_prev_next(q) = uve_list_next(q); \
    uve_list_next_prev(q) = uve_list_prev(q); \
    uve_list_init(q);                        \
  } while (0)

#define uve_list_foreach(q, h) \
  for ((q) = uve_list_next(h); (q) != (h); (q) = uve_list_next(q))


#ifdef __cplusplus
}
#endif

#endif