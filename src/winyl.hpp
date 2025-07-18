#pragma once

#include "station.hpp"
#include <vector>

#include "RGFW.h"
#include "imgui.h"

namespace Winyl
{
class App
{
public:
  App(const std::string &title);
  void Run();
  ~App();

private:
  RGFW_window *window;
  RGFW_monitor monitor;

  ImFont *mainFont = nullptr;
  ImDrawData *dd   = nullptr;

  std::vector<Winyl::Station> stations{0};

  ImVec4 openGLClearColor{0.094f, 0.094f, 0.094f, 1.0f};

  ImGuiWindowFlags imguiWindowFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

  RGFW_windowFlags rgfwWindowFlags = RGFW_windowCenter | RGFW_windowNoResize;

  void updateImGui();
  std::vector<Winyl::Station> getStations();
};
} // namespace Winyl