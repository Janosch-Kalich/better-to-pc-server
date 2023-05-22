#ifndef TO_PC_SERVER_TOAST_H
#define TO_PC_SERVER_TOAST_H

#include <iostream>
#include <ShObjIdl_core.h>
#include <wintoast/wintoastlib.h>
#include <boost/filesystem.hpp>

void init_toasts();

void example_toast();

void show_file_received_toast(std::string path);

void show_files_received_toast(std::string path);

#endif //TO_PC_SERVER_TOAST_H