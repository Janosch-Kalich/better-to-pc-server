#ifndef TO_PC_SERVER_STRINGHELPER_H
#define TO_PC_SERVER_STRINGHELPER_H

#include <string>
#include <stringapiset.h>

std::wstring string_to_wstring (std::string string) {
  char* char_ptr = const_cast<char *>(string.c_str());

  int size = MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, char_ptr, static_cast<int>(strlen(char_ptr)), NULL, 0);

  std::wstring file_name_wchar;
  file_name_wchar.resize(size + 10);

  MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, char_ptr, static_cast<int>(strlen(char_ptr)), &file_name_wchar[0], static_cast<int>(file_name_wchar.size()));

  return file_name_wchar;
}
#endif //TO_PC_SERVER_STRINGHELPER_H