#ifndef __UVE_REQUEST_H__
#define __UVE_REQUEST_H__ 1

#include <uv.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uve_request_t uve_request_t;
typedef void (*uve_request_cb)(uve_request_t*, int, uv_buf_t*);

uve_request_t* uve_request_peek(size_t len, void* data, uve_request_cb cb);
uve_request_t* uve_request_read(size_t len, void* data, uve_request_cb cb);

void uve_request_delete(uve_request_t* request);

void* uve_request_data(uve_request_t* request);

#ifdef __cplusplus
}
#endif

#endif // __UVE_REQUEST_H__
