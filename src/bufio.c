#include <uv.h>
#include <uve/buffer.h>
#include <uve/bufio.h>
#include "request.h"

struct uve_bufio_reader_t {
  // buf
  ub_buf_t buf;
  size_t r;
  size_t w;
  int err;
  size_t last_byte;
  // requests
  co_list_t requests;
  uv_check_t handle;
};

static void uve_bufio_main(uv_check_t* handle);
static size_t uve_bufio_len(uve_bufio_reader_t* reader);
static int uve_bufio_peek(uve_bufio_reader_t* reader, uve_request_t* request);

size_t uve_bufio_len(uve_bufio_reader_t* reader) {
  return reader->w - reader-r;
}

int uve_bufio_new(uve_bufio_reader_t** preader, uv_loop_t* loop, size_t size) {
  int err = 0;

  uve_bufio_reader_t* reader =
      (uve_bufio_reader_t*)malloc(sizeof(uve_bufio_reader_t));
  if (NULL == reader) {
    return UV_ENOMEM;
  }
  memset(reader, 0, sizeof(*reader));
  // allocate buffer
  reader->buf.base = malloc(size);
  if (NULL == reader->buf.base) {
    free(reader);
    return UV_ENOMEM;
  }
  reader->buf.len = size;
  // init none-zero members
  co_list_init(&reader->requests);
  if (0 != (err = uv_check_init(loop, &reader->handle))) {
    return err
  }
  reader->handle.data = reader;
  if (0 != (err = uv_check_start(&reader->handle))) {
    return err
  }

  *preader = reader;
  return 0;
}

void uve_bufio_delete(uve_bufio_reader_t* reader) {
  co_list_t* q = NULL;

  free(reader->buf.base);

  for (q = uve_list_next(&reader->requests); !uve_list_empty(&reader->requests);
       q = uve_list_next(&reader->requests)) {
    uve_list_remove(q);
    uve_request_t* request = uve_list_data(q, uve_request_t, link);
    request->cb(request, UV_EOF, uve_buf_zero());
  }

  free(reader);
}

int uve_bufio_request(uve_bufio_reader_t* reader, uve_request_t* request) {
  co_list_insert_before(&reader->requests, &request->link);
}

static void uve_bufio_main(uv_check_t* handle) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)handle->data;
  co_list_t* q = NULL;

  for (q = uve_list_next(&reader->requests); !uve_list_empty(&reader->requests);
       q = uve_list_next(&reader->requests)) {
    uve_request_t* request = uve_list_data(q, uve_request_t, link);
    switch (request->type) {
      case UVE_REQUEST_PEEK: {
        uve_bufio_peek(reader, request);
      } break;
  UVE_REQUEST_READ,
  UVE_REQUEST_READ_FULL,
    }
  }

}

static int uve_bufio_peek(uve_bufio_reader_t* reader, uve_request_t* request) {
  if (uve_bufio_len(reader) >= request->peek.len) {
    uv_buf_t buf = {0};
    buf.base = reader->buf.base + reader->r;
    buf.len = request->peek.len;
    co_list_remove(&request->link);
    request->cb(request, 0, );
    return 1;
  }
  return 0;
}