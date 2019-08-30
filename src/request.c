#include <uve/request.h>
#include "request.h"

uve_request_t* uve_request_peek(size_t len, void* data, uve_request_cb cb) {
  uve_request_t* request = (uve_request_t*)malloc(sizeof(uve_request_t));
  if (NULL == request) {
    return NULL;
  }
  memset(request, 0 ,sizeof(*request));
  
  uve_list_init(&request->link);
  request->type = UVE_REQUEST_PEEK;
  request->peek.len = len;
  request->cb = cb;
  request->data = data;

  return request;
}

uve_request_t* uve_request_read(size_t len, void* data, uve_request_cb cb) {
  uve_request_t* request = (uve_request_t*)malloc(sizeof(uve_request_t));
  if (NULL == request) {
    return NULL;
  }
  memset(request, 0 ,sizeof(*request));
  
  uve_list_init(&request->link);
  request->type = UVE_REQUEST_READ;
  request->peek.len = len;
  request->cb = cb;
  request->data = data;

  return request;
}

void uve_request_delete(uve_request_t* request) {
  free(request);
}


void* uve_request_data(uve_request_t* request) {
  return request->data;
}