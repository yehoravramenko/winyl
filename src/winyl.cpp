#ifndef NDEBUG
#define RGFW_DEBUG
#endif

#define RGFW_NO_INFO
#define RGFW_IMGUI_IMPLEMENTATION
#include "imgui_impl_rgfw.h"

#define RGFW_IMPLEMENTATION
#include "winyl.hpp"

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

App::App(const std::string &title)
{
  RGFW_setGLHint(RGFW_glMajor, 4);
  RGFW_setGLHint(RGFW_glMinor, 6);
  RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

  this->window = RGFW_createWindow(title.data(), RGFW_RECT(0, 0, 400, 500),
                                   this->rgfwWindowFlags);
  RGFW_window_makeCurrent(this->window);
  this->monitor = RGFW_window_getMonitor(this->window);

  if (this->window == nullptr)
  {
    std::println("Failed to create a window");
    // TODO: error
  }

#ifndef NDEBUG
  auto gl_version = glGetString(GL_VERSION);
  printf("%s\n", gl_version);
#endif

  this->stations = this->getStations();
}

void App::Run()
{
  glClearColor(this->openGLClearColor.x, this->openGLClearColor.y,
               this->openGLClearColor.z, this->openGLClearColor.w);
  glViewport(
      0, 0, static_cast<GLsizei>(this->window->r.w * this->monitor.pixelRatio),
      static_cast<GLsizei>(this->window->r.h * this->monitor.pixelRatio));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(this->window->r.w),
                                      static_cast<float>(this->window->r.h));
  ImGui::GetIO().DisplayFramebufferScale =
      ImVec2(this->monitor.scaleX, this->monitor.scaleY);
  ImGui::GetIO().IniFilename = "";
  mainFont                   = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      "C:\\WINDOWS\\FONTS\\SEGOEUI.TTF", 18.0f);

  ImGui_ImplRgfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init();

  while (RGFW_window_shouldClose(this->window) == RGFW_FALSE)
  {
    RGFW_window_checkEvents(this->window, RGFW_eventNoWait);

    ImGui::NewFrame();
    ImGui_ImplRgfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("MainWindow", nullptr, this->imguiWindowFlags);

    ImGui::PushFont(mainFont, 32.0f);
    ImGui::Text("Stations List");
    ImGui::PopFont();

    ImGui::PushFont(mainFont, 18.0f);

    int i = 0;
    // std::println("{}", this->stations[0].Name.c_str());
    for (Winyl::Station &s : stations)
    {
      ImGui::PushID(i++);
      if (ImGui::CollapsingHeader(s.Name.c_str()))
      {
        // std::println("got here");
        static char buffer[MAX_PATH]{};

        std::memset(buffer, '\0', MAX_PATH);
        s.Url.copy(buffer, MAX_PATH - 1);

        ImGui::BeginGroup();
        /**/ ImGui::AlignTextToFramePadding();
        /**/ ImGui::Text("URL");
        /**/ ImGui::SameLine();
        /**/ ImGui::InputText("##station_url", buffer, MAX_PATH);
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

    ImGui::Render();
    this->dd = ImGui::GetDrawData();
    ImGui_ImplOpenGL3_RenderDrawData(this->dd);

    RGFW_window_swapBuffers(this->window);
  }
}

App::~App()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplRgfw_Shutdown();
  ImGui::DestroyContext();

  RGFW_window_close(this->window);
}

std::vector<Winyl::Station> App::getStations()
{
  char pathBuffer[MAX_PATH];
  std::string cfgPath{};
  std::fstream cfg{};
  std::vector<Winyl::Station> stations{};

  GetModuleFileName(nullptr, pathBuffer, MAX_PATH);
  PathRemoveFileSpec(pathBuffer);
  cfgPath = pathBuffer;
  cfgPath += "\\winyl.cfg";

  // Fill with default data
  if (!std::filesystem::exists(cfgPath))
  {
    cfg.open(cfgPath, std::fstream::out | std::fstream::trunc);
    cfg << "#Lofi Hunter" << std::endl;
    cfg << "https://live.hunter.fm/lofi_high" << std::endl;
    cfg.close();
  }

  cfg.open(cfgPath, std::fstream::in);
  for (std::string line{}; std::getline(cfg, line);)
  {
    Winyl::Station s{};
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
    else if (line.size() > MAX_PATH)
    {
      std::cerr << "URL has more than 260 characters" << std::endl;
      std::exit(1);
    }

    s.Url = line;
    stations.push_back(s);
  }
  cfg.close();

  // TODO: Delete on release
  std::println("{}", stations.size());

  return stations;
}
} // namespace Winyl