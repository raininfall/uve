#ifndef __TASK_H__
#define __TASK_H__

#include <uv.h>

/* Fully close a loop */
static void close_walk_cb(uv_handle_t* handle, void* arg) {
  if (!uv_is_closing(handle)) {
    printf("handle:%p type:%d\r\n", handle, uv_handle_get_type(handle));
    uv_close(handle, NULL);
  }
}

static void close_loop(uv_loop_t* loop) {
  uv_walk(loop, close_walk_cb, NULL);
  uv_run(loop, UV_RUN_DEFAULT);
}

#define MAKE_VALGRIND_HAPPY()                         \
  do {                                                \
    close_loop(uv_default_loop());                    \
    ASSERT_EQ(0, uv_loop_close(uv_default_loop())); \
  } while (0)

#endif // __TASK_H__