#include <gtest/gtest.h>
#include <uve/bufio.h>
#include "task.h"

static void on_server_close(uv_handle_t* handle) {}

static void on_client_close(uv_handle_t* handle) {
  uve_bufio_reader_t* reader = (uve_bufio_reader_t*)handle->data;
  uve_bufio_delete(reader);

  uv_tcp_t* server = (uv_tcp_t*)handle->loop->data;
  uv_close((uv_handle_t*)server, on_server_close);

  free(handle);
}

static bool called[1] = {0};

static void on_peek(uve_request_t* request, int status, uv_buf_t* buf) {
  called[0] = true;

  ASSERT_EQ(0, status);

  ASSERT_EQ(4, buf->len);
  ASSERT_EQ(0, memcmp("1234", buf->base, 4));

  uv_tcp_t* client = (uv_tcp_t*)uve_request_data(request);
  uv_close((uv_handle_t*)client, on_client_close);

  uve_request_delete(request);
}

static void on_connection(uv_stream_t* server, int status) {
  int r = 0;

  ASSERT_LE(0, status);

  uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(server->loop, client);
  r = uv_accept(server, (uv_stream_t*)client);
  ASSERT_EQ(0, r);

  uve_bufio_reader_t* reader = NULL;
  r = uve_bufio_new(&reader, (uv_stream_t*)client, 1024);
  ASSERT_EQ(0, r);

  r = uv_read_start((uv_stream_t*)client, uve_bufio_alloc_cb(reader),
                    uve_bufio_read_cb(reader));
  ASSERT_EQ(0, r);

  r = uve_bufio_request(reader, uve_request_peek(4, client, on_peek));
  ASSERT_EQ(0, r);
}

static void on_sender_close(uv_handle_t* handle) {}

static void on_sender_write(uv_write_t* req, int status) {
  ASSERT_EQ(0, status);

  uv_tcp_t* sender = (uv_tcp_t*)req->data;
  uv_close((uv_handle_t*)sender, on_sender_close);

  free(req);
}

static void on_sender_connect(uv_connect_t* req, int status) {
  int r = 0;

  ASSERT_EQ(0, status);

  uv_tcp_t* sender = (uv_tcp_t*)req->data;
  uv_write_t* wr = (uv_write_t*)malloc(sizeof(uv_write_t));
  wr->data = sender;
  uv_buf_t buf = uv_buf_init("1234", 4);
  r = uv_write(wr, (uv_stream_t*)sender, &buf, 1, on_sender_write);
}

TEST(bufio, peek) {
  auto loop = uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(loop, &server);
  loop->data = &server;

  struct sockaddr_in addr = {0};
  uv_ip4_addr("0.0.0.0", 8809, &addr);

  uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
  int r = uv_listen((uv_stream_t*)&server, 16, on_connection);
  ASSERT_EQ(0, r);

  uv_tcp_t sender;
  uv_tcp_init(loop, &sender);

  uv_connect_t connect;
  connect.data = &sender;
  struct sockaddr_in dest = {0};
  uv_ip4_addr("127.0.0.1", 8809, &dest);

  uv_tcp_connect(&connect, &sender, (const struct sockaddr*)&dest,
                 on_sender_connect);

  r = uv_run(loop, UV_RUN_DEFAULT);
  ASSERT_EQ(0, r);

  for (size_t i = 0; i < (sizeof(called)/sizeof(called[0])); ++i) {
    ASSERT_TRUE(called[i]);
  }

  MAKE_VALGRIND_HAPPY();
}