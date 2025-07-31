#pragma once

#include "station.hpp"

#define RGFW_NO_WINDOW_SRC
#include "RGFW.h"
#include "imgui.h"

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>

// class RGFW_window;
// class RGFW_monitor;

namespace Winyl
{
class App
{
public:
  App(const std::string &title);
  void Run();
  ~App();

  [[noreturn]] static void Error(const std::string &msg);

private:
  RGFW_window *window = nullptr;
  RGFW_monitor monitor{};
  RGFW_windowFlags rgfwWindowFlags = RGFW_windowCenter | RGFW_windowNoResize;

  ImFont *mainFont = nullptr;
  ImDrawData *dd   = nullptr;
  ImVec4 openGLClearColor{0.094f, 0.094f, 0.094f, 1.0f};
  ImGuiWindowFlags imguiWindowFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

  std::vector<Winyl::Station> stations{0};

  bool isMinimized = false;

  void getStations();
};
} // namespace Winyl
