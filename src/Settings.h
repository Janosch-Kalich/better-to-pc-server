#ifndef TO_PC_SERVER_SETTINGS_H
#define TO_PC_SERVER_SETTINGS_H

#include <Windows.h>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <json/json.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct Settings
{
  bool advanced;
  fs::path path;
  std::string host;
  std::uint16_t port;
  std::string password;

  Settings (std::string host, std::uint16_t port, std::string password)
  {
    char* pValue;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
    fs::path path = fs::path(pValue);
    path.append("to-pc");
    fs::create_directories(path);
    fs::path settings_path = path.append("settings.json");

    this->advanced = false;
    this->path = settings_path;
    this->host = host;
    this->port = port;
    this->password = password;
  }

  void load()
  {
    try
    {
      Json::Value json;

      std::ifstream file(path.c_str());
      file >> json;

      this->advanced = json["advanced"].asBool();
      this->host = json["host"].asString();
      this->port = static_cast<std::uint16_t>(json["port"].asInt());
      this->password = json["password"].asString();
      file.close();
    }
    catch (std::exception &e)
    {
      OutputDebugString(e.what());
    }
  }

  void save()
  {
    try
    {
      Json::Value json;

      json["host"] = host;
      json["port"] = port;
      json["password"] = password;
      json["advanced"] = advanced;

      Json::StyledWriter writer;

      std::ofstream file;
      file.open(path.c_str());
      file << writer.write(json);
      file.close();
    }
    catch (std::exception &e)
    {
      OutputDebugString(e.what());
    }
  }
};

#endif //TO_PC_SERVER_SETTINGS_H
