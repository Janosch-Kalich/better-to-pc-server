#ifndef TO_PC_SERVER_SERVERHANDLER_H
#define TO_PC_SERVER_SERVERHANDLER_H

#include "Server.h"

class ServerHandler {
public:
  std::unique_ptr<Server> server;

  void init(std::string host, std::uint16_t port, std::string password);

  void start();

  void stop();
};


#endif //TO_PC_SERVER_SERVERHANDLER_H
