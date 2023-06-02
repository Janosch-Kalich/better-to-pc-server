#ifndef TO_PC_SERVER_SINGLEFILETOASTHANDLER_H
#define TO_PC_SERVER_SINGLEFILETOASTHANDLER_H

#include <iostream>
#include <ShlObj_core.h>
#include <wintoast/wintoastlib.h>
#include "StringHelper.h"
#include <boost/filesystem.hpp>
#include <shellapi.h>
#include <boost/algorithm/string/replace.hpp>

namespace fs = boost::filesystem;

class SingleFileToastHandler : public WinToastLib::IWinToastHandler {
public:
  fs::path path;

  SingleFileToastHandler(fs::path path)
  {
    this->path = path;
  }

  void toastActivated() const
  {
    std::wcout << L"The user clicked in this toast" << std::endl;
  }

  void toastActivated(int action_index) const
  {
    std::wstring path = string_to_wstring(this->path.string());
    OPENASINFO info {
        path.c_str(),
        NULL,
        OAIF_EXEC
    };

    switch (action_index)
    {
      case (0):
        SHOpenWithDialog(NULL, &info);
        break;
      case (1):
        fs::path parent = this->path.parent_path();
        std::string parent_str = parent.string();
        boost::replace_all(parent_str, "\\", "\\\\");
        ShellExecute(0, "explore", parent_str.c_str(), 0, 0, SW_SHOWNORMAL);
        OutputDebugString(parent_str.c_str());
        break;
    }
  }

  void toastDismissed(WinToastDismissalReason state) const
  {
    switch (state) {
      case UserCanceled:
        std::wcout << L"The user dismissed this toast" << std::endl;
        break;
      case TimedOut:
        std::wcout << L"The toast has timed out" << std::endl;
        break;
      case ApplicationHidden:
        std::wcout << L"The application hid the toast using ToastNotifier.hide()" << std::endl;
        break;
      default:
        std::wcout << L"Toast not activated" << std::endl;
        break;
    }
  }

  void toastFailed() const
  {
    std::wcout << L"Error showing current toast" << std::endl;
  }
};

#endif //TO_PC_SERVER_SINGLEFILETOASTHANDLER_H
