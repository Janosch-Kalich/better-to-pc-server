#include "StringHelper.h"

std::wstring string_to_wstring (std::string string) {
  char* char_ptr = const_cast<char *>(string.c_str());

  int size = MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, char_ptr, static_cast<int>(strlen(char_ptr)), NULL, 0);

  std::wstring wstr;
  wstr.resize(size + 10);

  MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, char_ptr, static_cast<int>(strlen(char_ptr)), &wstr[0], static_cast<int>(wstr.size()));

  return wstr;
}
