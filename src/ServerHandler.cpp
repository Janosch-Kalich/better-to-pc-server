#include "ServerHandler.h"

void ServerHandler::init(std::string host, std::uint16_t port, std::string password)
{
  this->server = std::unique_ptr<Server>(new Server(host, port, password));
}

void ServerHandler::start()
{
  this->server->listen();
}

void ServerHandler::stop()
{
  this->server->shutdown();
}