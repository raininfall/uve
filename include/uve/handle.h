#ifndef __UVE_HANDLE_H__
#define __UVE_HANDLE_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  UVE_UNKNOWN_HANDLE = 0,
  UVE_BUFIO,
} uve_handle_type;

typedef struct uve_handle_t uve_handle_t;

uve_handle_type uve_handle_get_type(uve_handle_t* handle);

#ifdef __cplusplus
}
#endif

#endif // __UVE_HANDLE_H__
