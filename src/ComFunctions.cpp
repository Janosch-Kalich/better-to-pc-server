#include "Communication.h"
#include "ServerHandler.h"
#include "Settings.h"
#include "com.pb.h"
#include <string>

extern Settings SETTINGS;
extern ServerHandler handler;

void host_get()
{
  Communication::shared_variables["host"] = SETTINGS.host;
}

void host_change()
{
  SETTINGS.host = Communication::shared_variables["host"];
  SETTINGS.save();
}

void port_get()
{
  Communication::shared_variables["port"] = std::to_string(SETTINGS.port);
}

void port_change()
{
  SETTINGS.port = std::stoi(Communication::shared_variables["port"]);
  SETTINGS.save();
}

void password_get()
{
  Communication::shared_variables["password"] = SETTINGS.password;
}

void password_change()
{
  SETTINGS.password = Communication::shared_variables["password"];
  SETTINGS.save();
}

void server_runnning_get()
{
  Communication::shared_variables["server_running"] = std::to_string(handler.server->running);
}

void server_runnning_change()
{
  Communication::shared_variables["server_running"] = std::to_string(handler.server->running);
}
