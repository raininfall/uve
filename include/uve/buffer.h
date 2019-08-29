#ifndef __UVE_BUFFER_INTERNAL_H__
#define __UVE_BUFFER_INTERNAL_H__

#include <uv.h>

#ifdef __cplusplus
extern "C" {
#endif

uv_buf_t* uve_buf_zero(void);

uv_buf_t* uve_buf_new(size_t size);

void uve_buf_delete(uv_buf_t* buf);

#ifdef __cplusplus
}
#endif

#endif // __UVE_BUFFER_INTERNAL_H__
