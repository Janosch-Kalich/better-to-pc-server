#include "SingleFileToastHandler.h"
#include "MultiFileToastHandler.h"
#include "Toast.h"

bool toast_initialized = false;

void init_toasts()
{
  WinToastLib::WinToast::instance()->setAppName(L"To PC");
  const auto aumi = WinToastLib::WinToast::configureAUMI(L"janoschkalich", L"to-pc", L"to-pc-server", L"13");
  WinToastLib::WinToast::instance()->setAppUserModelId(aumi);
  if (!WinToastLib::WinToast::instance()->initialize())
    OutputDebugString("Error, your system in not compatible!");
}

void example_toast()
{
  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text01);
  tmpl.setTextField(L"Yo", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.addAction(L"Open with");

  WinToastLib::WinToast::instance()->showToast(tmpl, new SingleFileToastHandler("E:\\qr.jpg"));
}

void show_file_received_toast(std::string path)
{
  if (!toast_initialized)
    init_toasts();

  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text01);
  tmpl.setTextField(L"File received", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.addAction(L"Open with");

  WinToastLib::WinToast::instance()->showToast(tmpl, new SingleFileToastHandler(path));
}

void show_files_received_toast(std::string path)
{
  if (!toast_initialized)
    init_toasts();

  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text01);
  tmpl.setTextField(L"File received", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.addAction(L"Open folder");
  //tmpl.addAction(L"Download zip");

  WinToastLib::WinToast::instance()->showToast(tmpl, new MultiFileToastHandler(path));
}
