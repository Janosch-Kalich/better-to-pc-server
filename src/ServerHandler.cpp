#include "ServerHandler.h"

void ServerHandler::init(std::string host, std::uint16_t port, std::string password, fs::path templates_path)
{
  this->server = std::unique_ptr<Server>(new Server(host, port, password, templates_path));
}

void ServerHandler::start()
{
  this->server->listen();
}

void ServerHandler::stop()
{
  this->server->shutdown();
}