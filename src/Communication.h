#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <thread>

namespace Communication
{
  extern std::map<std::string, std::string> shared_variables;
  extern std::map<std::string, std::function<void()>> shared_functions;
  extern bool running;

  void listen();

  void add_var(std::string id, std::string value, std::function<void()> update_function = nullptr, std::function<void()> get_function = nullptr);

  void delete_var(std::string id);

  void add_function(std::string id, std::function<void()> function);

  void delete_function(std::string id);

  void shutdown();
}
