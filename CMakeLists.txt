cmake_minimum_required(VERSION 3.25)
project(to_pc_server CXX)

set(CMAKE_CXX_STANDARD 20)

enable_language("RC")

if (MSVC)
    add_compile_options(/bigobj)
endif()

find_package(Boost COMPONENTS filesystem program_options nowide REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(restinio REQUIRED)
find_package(unofficial-nayuki-qr-code-generator CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_path(wintoast_INCLDUE_PATH wintoast/wintoast)
find_library(wintoast_LIB_PATH wintoast)

include_directories(traypp/tray/include ${wintoast_INCLUDE_PATH} include)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)
add_subdirectory(traypp EXCLUDE_FROM_ALL)

#if (WIN32)
add_executable(${PROJECT_NAME}
        WIN32
        src/main.cpp
        src/Server.cpp
        src/Server.h
        src/targetver.h
        src/Settings.h
        src/ServerHandler.cpp
        src/ServerHandler.h
        src/QR.h
        src/TempFile.cpp
        src/TempFile.h
        src/FileBatch.cpp
        src/FileBatch.h
        src/ClearTemp.h
        src/Toast.h
        src/SingleFileToastHandler.h
        src/MultiFileToastHandler.h
        src/FileHelper.h
        src/Toast.cpp src/StringHelper.h
        src/Resources.h
        src/Resources.cpp
        src/EmptyToastHandler.h
        src/StringHelper.cpp
        src/Trys.cpp
        src/Trys.h
        src/SecurityToastHandler.h
        src/Resources.rc
        src/Theme.h
        src/com.pb.h
        src/com.pb.cc
        src/Communication.h
        src/Communication.cpp
        src/ComFunctions.h
        src/ComFunctions.cpp)
#else (!WIN32)
#    add_executable(${PROJECT_NAME} src/main.cpp src/Gui.cpp src/Gui.h src/Window.h src/Server.cpp src/Server.h src/Settings.h)
#endif ()

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Boost::filesystem
        Boost::program_options
        Boost::nowide
        JsonCpp::JsonCpp
        restinio::restinio
        tray
        unofficial::nayuki-qr-code-generator::nayuki-qr-code-generator
        wsock32
        ws2_32
        iphlpapi
        ${wintoast_LIB_PATH}
        protobuf::libprotobuf
        cppzmq)
