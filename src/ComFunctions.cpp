#include "Communication.h"
#include "FileBatch.h"
#include "ServerHandler.h"
#include "Settings.h"
#include "TempFile.h"
#include "com.pb.h"
#include <boost/filesystem/operations.hpp>
#include <string>
#include "ComFunctions.h"
#include "Trys.h"

extern Settings SETTINGS;
extern ServerHandler handler;
extern bool changes;

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

void changes_get()
{
  changes = !(SETTINGS.host == handler.server->host && 
            SETTINGS.port == handler.server->port &&
            SETTINGS.password == handler.server->password);

  Communication::shared_variables["changes"] = std::to_string(changes);
}

void changes_change()
{
  changes_get();
}

void server_runnning_get()
{
  Communication::shared_variables["server_running"] = std::to_string(handler.server->running);
}

void server_runnning_change()
{
  Communication::shared_variables["server_running"] = std::to_string(handler.server->running);
}

void locked_get()
{
  Communication::shared_variables["locked"] = std::to_string(Trys::locked);
}

void locked_change()
{
  locked_get();
}

void tmp_dir_change() {
  Communication::shared_variables["tmp_dir"] = fs::temp_directory_path().append("to-pc").string();
}
