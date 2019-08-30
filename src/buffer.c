#include <string.h>
#include <stdlib.h>
#include <uve/buffer.h>

uv_buf_t* uve_buf_zero(void) {
  static uv_buf_t zero = {0};
  return &zero;
}

uv_buf_t* uve_buf_new(size_t size) {
  uv_buf_t* buf = (uv_buf_t*)malloc(size + sizeof(uv_buf_t));
  if (NULL == buf) {
    return NULL;
  }
  buf->base = (char*)(buf + 1);
  buf->len = size;
  return buf;
}

uv_buf_t* uve_buf_clone(uv_buf_t* buf) {
  uv_buf_t* new_buf = uve_buf_new(buf->len);
  if (NULL == new_buf) {
    return NULL;
  }

  memcpy(new_buf->base, buf->base, buf->len);
  return new_buf;
}

uv_buf_t* uve_buf_create(const char* base, size_t len) {
  uv_buf_t* new_buf = uve_buf_new(len);
  if (NULL == new_buf) {
    return NULL;
  }

  memcpy(new_buf->base, base, len);
  new_buf->len = len;

  return new_buf;
}

void uve_buf_delete(uv_buf_t* buf) {
  if (NULL != buf->base) {
    free(buf);
  }
}
