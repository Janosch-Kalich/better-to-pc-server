#ifndef TO_PC_SERVER_TOAST_H
#define TO_PC_SERVER_TOAST_H

#include <iostream>
#include <ShObjIdl_core.h>
#include <wintoast/wintoastlib.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void init_toasts();

void example_toast();

void show_file_received_toast(fs::path path);

void show_files_received_toast(std::string path, unsigned int files);

void show_verify_toast(std::string device_name);

void show_security_toast();

#endif //TO_PC_SERVER_TOAST_H