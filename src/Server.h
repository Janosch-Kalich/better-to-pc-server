#ifndef TO_PC_SERVER_SERVER_H
#define TO_PC_SERVER_SERVER_H

#include <restinio/all.hpp>
#include <json/json.h>
#include <sstream>
#include <string>
#include <shellapi.h>

using traits_t = restinio::traits_t<restinio::asio_timer_manager_t, restinio::single_threaded_ostream_logger_t, restinio::router::express_router_t<>>;

class Server {
public:
  std::string host;
  std::uint16_t port;
  std::string password;
  std::unique_ptr<std::thread> server_thread;
  std::unique_ptr<restinio::http_server_t<traits_t>> server;
  bool running;
  std::string last_url;

  Server(std::string host, std::uint16_t port, std::string password);

  int listen();

  void shutdown();

private:
  auto server_handler();
};

#endif //TO_PC_SERVER_SERVER_H
