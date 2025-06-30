#define _CRT_SECURE_NO_WARNINGS

#ifndef NDEBUG
#define RGFW_DEBUG
#endif
#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#include "imgui_impl_rgfw.h"

#include "backends/imgui_impl_opengl3.h"

#include <print>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <shlwapi.h>

#include "station.hpp"

namespace Winyl
{

void ImguiNewFrame()
{
  ImGui::NewFrame();
  ImGui_ImplRgfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
}

void ImguiRender()
{
  ImGui::Render();

  ImDrawData *dd = ImGui::GetDrawData();

  ImGui_ImplOpenGL3_RenderDrawData(dd);
}

void ImguiShutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplRgfw_Shutdown();
  ImGui::DestroyContext();
}

constexpr int MAX_BUF_SIZE = 256;

std::vector<Winyl::Station> getStations()
{
  char path_buf[MAX_PATH];
  std::string cfgPath{};
  std::fstream cfg{};
  std::vector<Winyl::Station> stations{};

  GetModuleFileName(nullptr, path_buf, MAX_PATH);
  PathRemoveFileSpec(path_buf);
  cfgPath = path_buf;
  cfgPath += "\\Winyl.cfg";

  if (!std::filesystem::exists(cfgPath))
  {
    cfg.open(cfgPath, std::fstream::out | std::fstream::trunc);
    cfg << "#Lofi Hunter" << std::endl;
    cfg << "https://live.hunter.fm/lofi_high" << std::endl;
    cfg.close();
  }

  cfg.open(cfgPath, std::fstream::in);

  while (true)
  {
    std::string line{};
    Winyl::Station s{};

    std::getline(cfg, line);
    if (line == "")
    {
      break;
    }

    if (!line.starts_with('#'))
    {
      std::cerr << "\"#\" expected in the start of the line" << std::endl;
      std::exit(1);
    }
    s.Name = line.erase(0, 1);

    std::getline(cfg, line);
    if (line == "")
    {
      std::cerr << "URL expected" << std::endl;
      std::exit(1);
    }
    else if (line.size() > MAX_BUF_SIZE)
    {
      std::cerr << "URL has more than 256 characters" << std::endl;
      std::exit(1);
    }

    s.Url = line;
    stations.push_back(s);
  }

  cfg.close();

  std::println("{}", stations.size());

  return stations;
}

} // namespace Winyl

constexpr ImVec4 OPENGL_CLEAR_COLOR{0.094f, 0.094f, 0.094f, 1.0f};

int main(int, char **)
{
  RGFW_window *win;
  RGFW_monitor monitor;
  ImFont *segoeui;
  std::vector<Winyl::Station> stations{0};

  constexpr ImGuiWindowFlags IMGUI_WINDOW_FLAGS =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

  constexpr RGFW_windowFlags RGFW_WINDOW_FLAGS =
      RGFW_windowCenter | RGFW_windowNoResize;

  RGFW_setGLHint(RGFW_glMajor, 4);
  RGFW_setGLHint(RGFW_glMinor, 6);
  RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

  win = RGFW_createWindow("Winyl (Milestone 3)", RGFW_RECT(0, 0, 400, 500),
                          RGFW_WINDOW_FLAGS);
  RGFW_window_makeCurrent(win);
  monitor = RGFW_window_getMonitor(win);

  if (win == nullptr)
  {
    std::println("Failed to create a window");
    return 1;
  }

#ifndef NDEBUG
  auto gl_version = glGetString(GL_VERSION);
  printf("%s\n", gl_version);
#endif

  glClearColor(OPENGL_CLEAR_COLOR.x, OPENGL_CLEAR_COLOR.y, OPENGL_CLEAR_COLOR.z,
               OPENGL_CLEAR_COLOR.w);
  glViewport(0, 0, static_cast<GLsizei>(win->r.w * monitor.pixelRatio),
             static_cast<GLsizei>(win->r.h * monitor.pixelRatio));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize =
      ImVec2(static_cast<float>(win->r.w), static_cast<float>(win->r.h));
  io.DisplayFramebufferScale = ImVec2(monitor.scaleX, monitor.scaleY);
  io.IniFilename             = "";
  segoeui =
      io.Fonts->AddFontFromFileTTF("C:\\WINDOWS\\FONTS\\SEGOEUI.TTF", 18.0f);

  ImGui_ImplRgfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init();

  stations = Winyl::getStations();

  while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
    RGFW_window_checkEvents(win, RGFW_eventNoWait);

    Winyl::ImguiNewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("MainWindow", nullptr, IMGUI_WINDOW_FLAGS);

    ImGui::PushFont(segoeui, 32.0f);
    ImGui::Text("Stations List");
    ImGui::PopFont();

    ImGui::PushFont(segoeui, 18.0f);
    int i = 0;
    for (Winyl::Station &s : stations)
    {
      ImGui::PushID(i++);
      if (ImGui::CollapsingHeader(s.Name.c_str()))
      {
        static char buffer[Winyl::MAX_BUF_SIZE]{};

        std::memset(buffer, '\0', Winyl::MAX_BUF_SIZE);
        s.Url.copy(buffer, Winyl::MAX_BUF_SIZE - 1);

        ImGui::BeginGroup();
        /**/ ImGui::AlignTextToFramePadding();
        /**/ ImGui::Text("URL");
        /**/ ImGui::SameLine();
        /**/ ImGui::InputText("##station_url", buffer, Winyl::MAX_BUF_SIZE);
        /**/ ImGui::SameLine();
        if (ImGui::Button("Play"))
          s.ChangeState();
        ImGui::EndGroup();
      }
      ImGui::PopID();
    }
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleVar();

    glClear(GL_COLOR_BUFFER_BIT);
    Winyl::ImguiRender();
    RGFW_window_swapBuffers(win);
  }

  Winyl::ImguiShutdown();
  RGFW_window_close(win);

  return 0;
}
