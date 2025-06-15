#ifndef NDEBUG
#define RGFW_DEBUG
#endif
#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#include "imgui_impl_rgfw.h"

#include "backends/imgui_impl_opengl3.h"

#include <string_view>

#include <shellapi.h>
#include "resource.h"

namespace winyl
{

void imgui_new_frame()
{
  ImGui::NewFrame();
  ImGui_ImplRgfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
}

void imgui_render()
{
  ImGui::Render();

  ImDrawData *dd = ImGui::GetDrawData();

  ImGui_ImplOpenGL3_RenderDrawData(dd);
}

void imgui_shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplRgfw_Shutdown();
  ImGui::DestroyContext();
}

} // namespace winyl

constexpr ::ImVec4 OPENGL_CLEAR_COLOR{0.094f, 0.094f, 0.094f, 1.0f};
constexpr std::string_view APP_FONT    = "C:\\WINDOWS\\FONTS\\SEGOEUI.TTF";
constexpr unsigned int WM_APPTRAY_ICON = 102;

::HINSTANCE instance_handle = nullptr;
::RGFW_window *app_window;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  case WM_APPTRAY_ICON: {

    switch (LOWORD(lparam))
    {
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDOWN:
      ::HMENU tray_menu, tray_submenu;
      ::POINT cursor_pos;

      ::GetCursorPos(&cursor_pos);
      tray_menu    = ::LoadMenu(instance_handle, MAKEINTRESOURCE(IDR_TRAYMENU));
      tray_submenu = ::GetSubMenu(tray_menu, 0);

      ::SetForegroundWindow(hwnd);
      ::TrackPopupMenu(tray_submenu,
                       TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON,
                       cursor_pos.x, cursor_pos.y, 0, hwnd, nullptr);

      ::DestroyMenu(tray_menu);
      break;
    }

    return TRUE;
  }

  case WM_COMMAND: {
    switch (LOWORD(wparam))
    {
    case ID_TRAY_EXIT:
      ::RGFW_window_setShouldClose(app_window, true);
      break;
    }
    return TRUE;
  }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main(int, char **)
{
  ::HANDLE mutex_instance;

  ::RGFW_monitor monitor;
  ::ImFont *segoeui;

  ::HWND dummy_hwnd;
  ::NOTIFYICONDATA nidata;

  mutex_instance = ::CreateMutex(
      nullptr, FALSE, "Winyl-{ea6fef0a-568d-4ac9-b44e-0a1e24924443}");
  if (::GetLastError() == ERROR_ALREADY_EXISTS ||
      ::GetLastError() == ERROR_ACCESS_DENIED)
    return 0;

  instance_handle = GetModuleHandle(nullptr);

  ::RGFW_setGLHint(RGFW_glMajor, 4);
  ::RGFW_setGLHint(RGFW_glMinor, 6);
  ::RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

  app_window =
      ::RGFW_createWindow("winyl (Milestone 3)", RGFW_RECT(0, 0, 400, 500),
                          RGFW_windowCenter | RGFW_windowNoResize);
  ::RGFW_window_makeCurrent(app_window);
  monitor = ::RGFW_window_getMonitor(app_window);

  if (app_window == nullptr)
  {
    std::puts("Failed to create a window");
    return 1;
  }

#ifndef NDEBUG
  auto gl_version = ::glGetString(GL_VERSION);
  std::printf("%s\n", gl_version);
#endif

  ::glClearColor(OPENGL_CLEAR_COLOR.x, OPENGL_CLEAR_COLOR.y,
                 OPENGL_CLEAR_COLOR.z, OPENGL_CLEAR_COLOR.w);
  ::glViewport(0, 0, static_cast<GLsizei>(app_window->r.w * monitor.pixelRatio),
               static_cast<GLsizei>(app_window->r.h * monitor.pixelRatio));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io                = ImGui::GetIO();
  io.DisplaySize             = ::ImVec2(static_cast<float>(app_window->r.w),
                                        static_cast<float>(app_window->r.h));
  io.DisplayFramebufferScale = ::ImVec2(monitor.scaleX, monitor.scaleY);
  io.IniFilename             = "";
  segoeui = io.Fonts->AddFontFromFileTTF(APP_FONT.data(), 18.0f, nullptr,
                                         io.Fonts->GetGlyphRangesDefault());

  ::ImGui_ImplRgfw_InitForOpenGL(app_window, true);
  ::ImGui_ImplOpenGL3_Init();

  dummy_hwnd = ::CreateWindow("STATIC", "dummy", 0, 0, 0, 0, 0, nullptr,
                              nullptr, nullptr, nullptr);
  ::SetWindowLongPtr(dummy_hwnd, GWLP_WNDPROC, (LONG_PTR)&wnd_proc);

  nidata.cbSize = sizeof(NOTIFYICONDATA);
  nidata.hWnd   = dummy_hwnd;
  nidata.uID    = IDI_SYSTRAY;
  nidata.uFlags = NIF_ICON | NIF_MESSAGE;
  nidata.hIcon  = ::LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_SYSTRAY));
  nidata.uCallbackMessage = WM_APPTRAY_ICON;
  ::Shell_NotifyIcon(NIM_ADD, &nidata);

  while (RGFW_window_shouldClose(app_window) == RGFW_FALSE)
  {
    ::RGFW_window_checkEvents(app_window, RGFW_eventNoWait);

    winyl::imgui_new_frame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushFont(segoeui);
    ImGui::Begin("Winyl", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove);

    ImGui::Text("Winyl!");

    ImGui::End();
    ImGui::PopFont();
    ImGui::PopStyleVar();

    ::glClear(GL_COLOR_BUFFER_BIT);
    winyl::imgui_render();
    ::RGFW_window_swapBuffers(app_window);
  }

  winyl::imgui_shutdown();
  ::RGFW_window_close(app_window);

  ::Shell_NotifyIcon(NIM_DELETE, &nidata);

  ::CloseHandle(mutex_instance);
  ::ReleaseMutex(mutex_instance);

  return 0;
}