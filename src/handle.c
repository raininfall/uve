#include <uve/handle.h>
#include "handle.h"

struct uve_handle_t {
  uve_handle_private_part()
};

uve_handle_type uve_handle_get_type(uve_handle_t* handle) {
  return handle->type;
}

void uve_handle_init(uve_handle_t* handle, uve_handle_type type) {
  handle->type = type;
  handle->active_count = 0;
  handle->flag = 0; // TODO: init flag
}