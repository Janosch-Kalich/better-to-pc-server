#ifndef TO_PC_SERVER_RESOURCES_H
#define TO_PC_SERVER_RESOURCES_H

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

extern fs::path resources_root;

void init_resources();

fs::path fonts();

fs::path icons();

fs::path templates();

#endif //TO_PC_SERVER_RESOURCES_H