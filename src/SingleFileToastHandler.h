#ifndef TO_PC_SERVER_SINGLEFILETOASTHANDLER_H
#define TO_PC_SERVER_SINGLEFILETOASTHANDLER_H

#include <iostream>
#include <ShObjIdl_core.h>
#include <wintoast/wintoastlib.h>

class SingleFileToastHandler : public WinToastLib::IWinToastHandler {
public:
  std::wstring path;

  SingleFileToastHandler(std::string path)
  {
    this->path = std::wstring(path.begin(), path.end());
  }

  void toastActivated() const
  {
    std::wcout << L"The user clicked in this toast" << std::endl;
  }

  void toastActivated(int action_index) const
  {
    switch (action_index)
    {
      case (0):
        OPENASINFO info {
          path.c_str(),
          NULL,
          OAIF_EXEC
        };
        SHOpenWithDialog(NULL, &info);
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
