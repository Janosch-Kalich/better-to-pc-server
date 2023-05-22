#ifndef TO_PC_SERVER_CLEARTEMP_H
#define TO_PC_SERVER_CLEARTEMP_H

#include <boost/filesystem.hpp>
#include <filesystem>

namespace fs = boost::filesystem;

void clear_temp()
{
  fs::remove_all(fs::temp_directory_path().append("to-pc"));
}

#endif //TO_PC_SERVER_CLEARTEMP_H