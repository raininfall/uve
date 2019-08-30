#ifndef __UVE_HANDLE_INTERNAL_H__
#define __UVE_HANDLE_INTERNAL_H__ 1

#include <stddef.h>
#include <stdint.h>
#include <uve/handle.h>

#define uve_container_of(ptr, type, field) \
  ((type*)((char*)(ptr)-offsetof(type, field)))

#define uve_handle_private_part() \
  uve_handle_type type;           \
  size_t active_count;            \
  uint32_t flag;

void uve_handle_init(uve_handle_t* handle, uve_handle_type type);

#endif // __UVE_HANDLE_INTERNAL_H__
