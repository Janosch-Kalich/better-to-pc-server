#include <filesystem>
#include "FileBatch.h"

FileBatch::FileBatch(fs::path batch_id)
{
  this->batch_id = batch_id;
}

void FileBatch::del()
{
  for (TempFile file : this->files)
  {
    file.del();
  }

  files.clear();
  std::filesystem::remove(fs::temp_directory_path().append("to-pc").append(this->batch_id).c_str());
}

void FileBatch::default_action()
{
  if (this->files.size() == 1)
  {
    this->files[0].open_with();
  }
  else {
    std::string path = fs::temp_directory_path().append("to-pc").append(this->batch_id).string();
    show_files_received_toast(path);
  }
}
