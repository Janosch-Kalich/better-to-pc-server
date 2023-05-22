#ifndef TO_PC_SERVER_FILEBATCH_H
#define TO_PC_SERVER_FILEBATCH_H

#include "TempFile.h"

class FileBatch {
public:
  fs::path batch_id;
  std::vector<TempFile> files;

  FileBatch(fs::path batch_id);

  void default_action();

  void open();

  void del();
};


#endif //TO_PC_SERVER_FILEBATCH_H
