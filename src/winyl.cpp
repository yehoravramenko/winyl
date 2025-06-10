#ifndef NDEBUG
#define RGFW_DEBUG
#endif
#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#include "imgui_impl_rgfw.h"

#include "backends/imgui_impl_opengl3.h"

#include <stdio.h>

void imgui_new_frame();
void imgui_render();
void imgui_shutdown();

constexpr ImVec4 OPENGL_CLEAR_COLOR{0.094f, 0.094f, 0.094f, 1.0f};

int main(int, char **)
{
  RGFW_window *win;
  RGFW_monitor monitor;
  unsigned char *tex_pixels;
  int tex_w, tex_h;

  RGFW_setGLHint(RGFW_glMajor, 4);
  RGFW_setGLHint(RGFW_glMinor, 6);
  RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

  win = RGFW_createWindow("winyl (Milestone 2)", RGFW_RECT(0, 0, 400, 500),
                          RGFW_windowCenter | RGFW_windowNoResize);
  RGFW_window_makeCurrent(win);
  monitor = RGFW_window_getMonitor(win);

  if (win == nullptr)
  {
    puts("Failed to create a window");
    return 1;
  }

#ifndef NDEBUG
  auto gl_version = glGetString(GL_VERSION);
  printf("%s\n", gl_version);
#endif

  glClearColor(OPENGL_CLEAR_COLOR.x, OPENGL_CLEAR_COLOR.y, OPENGL_CLEAR_COLOR.z,
               OPENGL_CLEAR_COLOR.w);
  glViewport(0, 0, win->r.w * monitor.pixelRatio,
             win->r.h * monitor.pixelRatio);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io                = ImGui::GetIO();
  io.DisplaySize             = ImVec2(win->r.w, win->r.h);
  io.DisplayFramebufferScale = ImVec2(monitor.scaleX, monitor.scaleY);
  io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_w, &tex_h);
  io.IniFilename = "";

  ImGui_ImplRgfw_InitForOpenGL(win, true);
  ImGui_ImplOpenGL3_Init();

  while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
    RGFW_window_checkEvents(win, RGFW_eventNoWait);

    imgui_new_frame();

    // ImGui::Begin("Hello!");

    glClear(GL_COLOR_BUFFER_BIT);
    imgui_render();
    RGFW_window_swapBuffers(win);
  }

  imgui_shutdown();
  RGFW_window_close(win);

  return 0;
}

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