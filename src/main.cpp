#include "targetver.h"
#include <exception>
#include <iostream>
#include "Server.h"
#include <Windows.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <stdlib.h>
#include <string>
#include "../traypp/tray/include/tray.hpp"
#include "../traypp/tray/include/components/button.hpp"
#include "ServerHandler.h"
#include "ClearTemp.h"
#include "Toast.h"
#include "Resources.h"
#include "Communication.h"
#include "ComFunctions.h"
#include "Trys.h"

namespace fs = boost::filesystem;

void start_server();

Settings SETTINGS("0.0.0.0", 3333, "Password");
ServerHandler handler{};

void stop_server();
void start_server();

bool changes = false;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmd_show)
{
  init_toasts();

  Resources::init_resources();

  clear_temp();
  
  SETTINGS.load();

  start_server();

  Communication::add_var("tmp_dir", fs::temp_directory_path().append("to-pc").string(), tmp_dir_change);

  Communication::add_var("host", SETTINGS.host, host_change, host_get);
  Communication::add_var("port", std::to_string(SETTINGS.port), port_change, port_get);
  Communication::add_var("password", SETTINGS.password, password_change, password_get);

  Communication::add_var("changes", std::to_string(changes), changes_change, changes_get);

  Communication::add_var("server_running", std::to_string(handler.server->running), server_runnning_change, server_runnning_get);

  Communication::add_var("locked", std::to_string(Trys::locked), locked_change, locked_get);

  Communication::add_function("start_server", start_server);
  Communication::add_function("stop_server", stop_server);
  Communication::add_function("unlock", Trys::unlock);

  std::thread com_thread (&Communication::listen);

  Tray::Tray tray("to-pc-server", Resources::icons().append("favicon.ico").string());
  tray.addEntries(Tray::Button("Exit", [&]{
    tray.exit();
    Communication::shutdown();
    com_thread.join();
    handler.stop();
  }));

  tray.run();

  for(FileBatch batch : handler.server->batches)
  {
    batch.del();
  }

  handler.server->batches.clear();

  return 0;
}

void start_server()
{
  fs::path templates_path = SETTINGS.path;
  handler.init(SETTINGS.host, SETTINGS.port, SETTINGS.password, templates_path.parent_path().append("resources").append("templates"));
  handler.start();
}

void stop_server()
{
  handler.stop();
}

bool server_running()
{
  return handler.server->running;
}
