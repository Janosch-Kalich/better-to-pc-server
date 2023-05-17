#ifndef TO_PC_SERVER_WINDOW_H
#define TO_PC_SERVER_WINDOW_H

#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_stdlib.h>
#include "Settings.h"

bool WINDOW_OPEN = false;

void resize_callback(GLFWwindow* window, int width, int height);

void overwrite_settings();
void overwrite_tmp_settings();
void start_server();
void stop_server();
bool server_running();

Settings CURRENT_SETTINGS("0.0.0.0", 3333, "Password");
Settings TMP_SETTINGS("0.0.0.0", 3333, "Password");
int TMP_PORT;

int show_window() {
  if (WINDOW_OPEN)
    return 0;

  WINDOW_OPEN = true;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(600, 260, "to-pc-server", nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resize_callback);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  fs::path font_path = TMP_SETTINGS.path;
  font_path = font_path.parent_path().append("resources").append("fonts").append("Inter-Regular.ttf");

  io.Fonts->AddFontFromFileTTF(font_path.string().c_str(), 16);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  std::string asd;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("to-pc-server", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::InputText("-> Host", &TMP_SETTINGS.host);
    ImGui::InputInt("-> Port", &TMP_PORT);
    ImGui::InputText("-> Password", &TMP_SETTINGS.password);
    if (ImGui::Button("Load"))
      overwrite_tmp_settings();
    ImGui::SameLine();
    if (ImGui::Button("Save"))
      overwrite_settings();
    ImGui::SameLine();
    if (!server_running())
    {
      if (ImGui::Button("Start"))
        start_server();
    }
    if (server_running())
    {
      ImGui::SameLine();
      if (ImGui::Button("Stop"))
        stop_server();
      ImGui::SameLine();
      if (ImGui::Button("Restart")) {
        stop_server();
        start_server();
      }
    }
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Text("Current settings:");
    ImGui::Text(std::format("Host: {}", CURRENT_SETTINGS.host).c_str());
    ImGui::Text(std::format("Port: {}", std::to_string(CURRENT_SETTINGS.port)).c_str());
    ImGui::Text(std::format("Port: {}", CURRENT_SETTINGS.password).c_str());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwWaitEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  WINDOW_OPEN = false;

  return 0;
}

void resize_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

#endif //TO_PC_SERVER_WINDOW_H