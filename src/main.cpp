#include "targetver.h"
#include <iostream>
#include "Server.h"
#include "Window.h"
#include <Windows.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <stdlib.h>
#include "../traypp/tray/include/tray.hpp"
#include "../traypp/tray/include/components/button.hpp"
#include "ServerHandler.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

void tray_handler();
void server_handler();
void start_server();

Settings SETTINGS("0.0.0.0", 3333, "Password");
ServerHandler handler{};


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmd_show)
{
  po::options_description desc("to-pc-server");
  desc.add_options()
      ("gui", "Shows gui when starting");

  po::variables_map vm;
  po::store(po::parse_command_line(__argc, __argv, desc), vm);
  po::notify(vm);

  SETTINGS.load();

  if (vm.count("gui"))
  {
    overwrite_tmp_settings();
    std::thread window_thread(show_window);
  }

  CURRENT_SETTINGS = SETTINGS;

  start_server();

  Tray::Tray tray("to-pc-server", "resources/icons/favicon.ico");
  tray.addEntries(Tray::Button("Show UI", [&]{
    overwrite_tmp_settings();
    show_window();
  }), Tray::Button("Exit", [&]{
    tray.exit();
    handler.stop();
  }));

  tray.run();

  return 0;
}

void overwrite_settings()
{
  SETTINGS = TMP_SETTINGS;
  SETTINGS.port = static_cast<std::uint16_t>(TMP_PORT);
  SETTINGS.save();
}

void overwrite_tmp_settings()
{
  SETTINGS.load();
  TMP_SETTINGS = SETTINGS;
  TMP_PORT = static_cast<int>(SETTINGS.port);
}

void start_server()
{
  handler.init(SETTINGS.host, SETTINGS.port, SETTINGS.password);
  CURRENT_SETTINGS = SETTINGS;
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