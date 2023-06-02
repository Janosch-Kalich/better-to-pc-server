#include "Resources.h"

namespace Resources
{
  fs::path resources_root;

  void init_resources() {
    fs::path path(fs::initial_path<fs::path>());
    path = fs::system_complete(fs::path(__argv[0]));
    resources_root = path.parent_path().append("resources");
  }

  fs::path fonts() {
    fs::path tmp = resources_root;
    return tmp.append("fonts");
  }

  fs::path icons() {
    fs::path tmp = resources_root;
    return tmp.append("icons");
  }

  fs::path templates() {
    fs::path tmp = resources_root;
    return tmp.append("templates");
  }
}
