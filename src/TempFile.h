#ifndef TO_PC_SERVER_TEMPFILE_H
#define TO_PC_SERVER_TEMPFILE_H

#include <boost/filesystem.hpp>
#include <restinio/all.hpp>
#include <fstream>
#include "Toast.h"
#include <cstdio>

namespace fs = boost::filesystem;

class TempFile {
public:
  fs::path batch_id;
  fs::path path;

  TempFile(fs::path batch_id, restinio::optional_t<std::string> file_name, restinio::string_view_t body);

  void open();

  void open_with();

  void del();
};

#endif //TO_PC_SERVER_TEMPFILE_H
