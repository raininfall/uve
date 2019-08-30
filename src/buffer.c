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

void uve_buf_delete(uv_buf_t* buf) {
  if (NULL != buf->base) {
    free(buf);
  }
}