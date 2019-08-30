#ifndef __UVE_BUF_IO_H__
#define __UVE_BUF_IO_H__ 1

#include <uv.h>
#include "request.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uve_bufio_reader_t uve_bufio_reader_t;

int uve_bufio_new(uve_bufio_reader_t** preader, uv_stream_t* handle, size_t size);
void uve_bufio_delete(uve_bufio_reader_t* reader);

uv_alloc_cb uve_bufio_alloc_cb(uve_bufio_reader_t* reader);
uv_read_cb uve_bufio_read_cb(uve_bufio_reader_t* reader);

int uve_bufio_request(uve_bufio_reader_t* reader, uve_request_t* request);

#ifdef __cplusplus
}
#endif

#endif // __UVE_BUF_IO_H__
