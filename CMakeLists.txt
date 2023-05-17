cmake_minimum_required(VERSION 3.25)
project(to_pc_server CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS filesystem program_options REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(restinio REQUIRED)

include_directories(traypp/tray/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)
add_subdirectory(traypp EXCLUDE_FROM_ALL)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 src/main.cpp src/Gui.cpp src/Gui.h src/Window.h src/Server.cpp src/Server.h src/targetver.h src/Settings.h src/ServerHandler.cpp src/ServerHandler.h)
#else (!WIN32)
#    add_executable(${PROJECT_NAME} src/main.cpp src/Gui.cpp src/Gui.h src/Window.h src/Server.cpp src/Server.h src/Settings.h)
endif (WIN32)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Boost::filesystem
        Boost::program_options
        glfw
        glad::glad
        imgui::imgui
        JsonCpp::JsonCpp
        restinio::restinio
        tray)