#include "TempFile.h"
#include <filesystem>
#include <shellapi.h>
#include <boost/algorithm/string/replace.hpp>
#include "FileHelper.h"
#include <boost/nowide/fstream.hpp>

TempFile::TempFile(fs::path batch_id, restinio::optional_t<std::string> file_name, restinio::string_view_t body)
{
  this->batch_id = batch_id;

  fs::path path = fs::temp_directory_path();
  path = path.append("to-pc").append(batch_id);
  fs::create_directories(path);
  std::string final_file_name = find_available_name(path, file_name->c_str());

  path = path.append(final_file_name);

  OutputDebugString(path.string().c_str());

  this->path = path;

  boost::nowide::ofstream file(path.string().c_str(), std::ios::binary);

  file.write(body.data(), body.size());

  file.close();
}

void TempFile::open()
{
  std::string path = this->path.string();
  boost::replace_all(path, "\\", "\\\\");
  ShellExecute(0, "open", path.c_str(), 0, 0, 0);
}

void TempFile::open_with()
{
  show_file_received_toast(this->path.string());
}

void TempFile::del()
{
  std::filesystem::remove(this->path.c_str());
}