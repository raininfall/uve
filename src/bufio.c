#include <assert.h>
#include <uv.h>
#include <uve/buffer.h>
#include <uve/math.h>
#include <uve/bufio.h>
#include "request.h"

struct uve_bufio_reader_t {
  // buf
  uv_buf_t buf;
  size_t r;
  size_t w;
  int err;
  size_t last_byte;
  // requests
  uve_list_t requests;
  uv_check_t handle;
};

static void uve_bufio_main(uv_check_t* handle);
static size_t uve_bufio_len(uve_bufio_reader_t* reader);
static int uve_bufio_handle_peek(uve_bufio_reader_t* reader, uve_request_t* request);
static int uve_bufio_handle_read(uve_bufio_reader_t* reader, uve_request_t* request);

size_t uve_bufio_len(uve_bufio_reader_t* reader) {
  return reader->w - reader->r;
}

int uve_bufio_new(uve_bufio_reader_t** preader, uv_stream_t* handle, size_t size) {
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
  uve_list_init(&reader->requests);
  if (0 != (err = uv_check_init(handle->loop, &reader->handle))) {
    return err;
  }
  reader->handle.data = reader;
  if (0 != (err = uv_check_start(&reader->handle, uve_bufio_main))) {
    return err;
  }

  handle->data = reader;

  *preader = reader;
  return 0;
}

static void uve_bufio_on_close(uv_handle_t* handle) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)handle->data;
  free(reader->buf.base);
  free(reader);
}

void uve_bufio_delete(uve_bufio_reader_t* reader) {
  uve_list_t* q = NULL;

  for (q = uve_list_next(&reader->requests); !uve_list_empty(&reader->requests);
       q = uve_list_next(&reader->requests)) {
    uve_list_remove(q);
    uve_request_t* request = uve_list_data(q, uve_request_t, link);
    request->cb(request, UV_EOF, uve_buf_zero());
  }

  uv_close((uv_handle_t*)&reader->handle, uve_bufio_on_close);
}

int uve_bufio_request(uve_bufio_reader_t* reader, uve_request_t* request) {
  uve_list_insert_before(&reader->requests, &request->link);
  return 0;
}

static void uve_bufio_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)handle->data;
  buf->base = reader->buf.base + reader->w;
  buf->len = reader->buf.len - reader->w;
}

uv_alloc_cb uve_bufio_alloc_cb(uve_bufio_reader_t* reader) {
  return uve_bufio_alloc;
}

static void uve_bufio_read(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)stream->data;
  if (nread > 0) {
    assert(reader->buf.base + reader->w == buf->base);
    reader->w += buf->len;
  } else if (nread < 0) {
    reader->err = nread;
  }
}

uv_read_cb uve_bufio_read_cb(uve_bufio_reader_t* reader) {
  return uve_bufio_read;
}

static void uve_bufio_main(uv_check_t* handle) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)handle->data;
  uve_list_t* q = NULL;
  int err = 0;

  for (q = uve_list_next(&reader->requests); !uve_list_empty(&reader->requests);
       q = uve_list_next(&reader->requests)) {
    uve_request_t* request = uve_list_data(q, uve_request_t, link);
    switch (request->type) {
      case UVE_REQUEST_PEEK: {
        err = uve_bufio_handle_peek(reader, request);
      } break;
      case UVE_REQUEST_READ: {
        err = uve_bufio_handle_read(reader, request);
      } break;
    }
    if (err > 0) {
      continue;
    } else if (err == 0) {
      break;
    } else {
      reader->err = err;
      return;
    }
  }
}

static int uve_bufio_handle_peek(uve_bufio_reader_t* reader,
                                 uve_request_t* request) {
  if (uve_bufio_len(reader) >= request->peek.len) {
    uve_list_remove(&request->link);

    uv_buf_t* buf =
        uve_buf_create(reader->buf.base + reader->r, request->peek.len);
    if (NULL == buf) {
      request->cb(request, UV_ENOMEM, NULL);
    } else {
      request->cb(request, 0, buf);
    }
    return 1;
  }
  return 0;
}

static int uve_bufio_handle_read(uve_bufio_reader_t* reader,
                                 uve_request_t* request) {
  if (uve_bufio_len(reader) == 0) {
    return 0;
  }
  
  uv_buf_t buf = {0};
  buf.base = reader->buf.base + reader->r;
  buf.len = UVE_MIN(uve_bufio_len(reader), request->read.len);
  // callback
  uve_list_remove(&request->link);
  request->cb(request, 0, &buf);
  // read data
  reader->r += request->read.len;
  // reset read and write marker when buffer is empty
  if (reader->r == reader->w) {
    reader->r = reader->w = 0;
  }

  return 1;
}