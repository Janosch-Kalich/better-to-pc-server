cmake_minimum_required(VERSION 3.25)
project(to_pc_server CXX)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/bigobj)
endif()

find_package(Boost COMPONENTS filesystem program_options REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(restinio REQUIRED)
find_package(unofficial-nayuki-qr-code-generator CONFIG REQUIRED)
find_path(wintoast_INCLDUE_PATH wintoast/wintoast)
find_library(wintoast_LIB_PATH wintoast)

include_directories(traypp/tray/include ${wintoast_INCLUDE_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)
add_subdirectory(traypp EXCLUDE_FROM_ALL)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 src/main.cpp src/Gui.cpp src/Gui.h src/Window.h src/Server.cpp src/Server.h src/targetver.h src/Settings.h src/ServerHandler.cpp src/ServerHandler.h src/QR.h src/TempFile.cpp src/TempFile.h src/FileBatch.cpp src/FileBatch.h src/ClearTemp.h src/Toast.h src/SingleFileToastHandler.h src/MultiFileToastHandler.h src/FileHelper.h src/Toast.cpp)
#else (!WIN32)
#    add_executable(${PROJECT_NAME} src/main.cpp src/Gui.cpp src/Gui.h src/Window.h src/Server.cpp src/Server.h src/Settings.h)
endif ()

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Boost::filesystem
        Boost::program_options
        glfw
        glad::glad
        imgui::imgui
        JsonCpp::JsonCpp
        restinio::restinio
        tray
        unofficial::nayuki-qr-code-generator::nayuki-qr-code-generator
        wsock32
        ws2_32
        iphlpapi
        ${wintoast_LIB_PATH})