#include "SingleFileToastHandler.h"
#include "MultiFileToastHandler.h"
#include "EmptyToastHandler.h"
#include "Toast.h"
#include "Resources.h"
#include <format>
#include "StringHelper.h"

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

void show_file_received_toast(fs::path path)
{
  fs::path icon_path = icons();
  icon_path.append("upload.png");

  fs::path file_name = path;

  std::string file_name_str = file_name.filename().string();
  std::wstring file_name_wstr = string_to_wstring(file_name_str);

  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::ImageAndText02);
  tmpl.setTextField(L"File received", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.setTextField(file_name_wstr.c_str(), WinToastLib::WinToastTemplate::SecondLine);
  tmpl.setImagePath(icon_path.wstring());
  tmpl.addAction(L"Open with");

  WinToastLib::WinToast::instance()->showToast(tmpl, new SingleFileToastHandler(path.string()));
}

void show_files_received_toast(std::string path, unsigned int files)
{
  fs::path icon_path = icons();
  icon_path.append("upload.png");

  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::ImageAndText02);
  tmpl.setTextField(L"Files received", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.setTextField(std::format(L"Receive {} files", files), WinToastLib::WinToastTemplate::SecondLine);
  tmpl.setImagePath(icon_path.wstring());
  tmpl.addAction(L"Open folder");

  WinToastLib::WinToast::instance()->showToast(tmpl, new MultiFileToastHandler(path));
}

void show_verify_toast(std::string device_name)
{
  fs::path path = icons();
  path.append("check.png");

  std::wstring device_name_wstr = string_to_wstring(device_name);

  WinToastLib::WinToastTemplate tmpl = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::ImageAndText02);
  tmpl.setTextField(L"Connected", WinToastLib::WinToastTemplate::FirstLine);
  tmpl.setTextField(std::format(L"Established connection to {}", device_name_wstr.c_str()), WinToastLib::WinToastTemplate::SecondLine);
  tmpl.setImagePath(path.wstring());

  WinToastLib::WinToast::instance()->showToast(tmpl, new EmptyToastHandler());
}