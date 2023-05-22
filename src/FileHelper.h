#ifndef TO_PC_SERVER_FILEHELPER_H
#define TO_PC_SERVER_FILEHELPER_H

#include <string>
#include <boost/filesystem.hpp>
#include <format>

namespace fs = boost::filesystem;

std::string find_available_name(fs::path path, std::string file_name)
{
  fs::path file_name_path = fs::path(file_name);
  fs::path original_path = path;
  path.append(file_name_path);
  int i = 1;

  while (fs::exists(path))
  {
    file_name = file_name_path.stem().string() + std::format(" ({})", i) + file_name_path.extension().string();
    path = original_path.append(file_name);
    original_path = original_path.parent_path();
    i++;
  }

  return file_name;
}

#endif //TO_PC_SERVER_FILEHELPER_H