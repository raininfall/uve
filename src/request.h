#ifndef __UVE_REQUEST_INTERNAL_H__
#define __UVE_REQUEST_INTERNAL_H__ 1

#include <uve/list.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  UVE_REQUEST_PEEK,
  UVE_REQUEST_READ,
  UVE_REQUEST_READ_FULL,
} uve_request_type;

typedef struct uve_peek_request_t {
  size_t len;
} uve_peek_request_t;

typedef struct uve_read_request_t {
  size_t len;
} uve_read_request_t;

struct uve_request_t {
  uve_list_t link;
  uve_request_type type;
  union {
    uve_peek_request_t peek;
    uve_read_request_t read;
  };
  union {
    uve_request_cb cb;
  };
  void* data;
};


#ifdef __cplusplus
}
#endif

#endif // __UVE_REQUEST_INTERNAL_H__
