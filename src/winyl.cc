#define OEMRESOURCE
#include <GL/gl3w.h>

#undef WIN32_LEAN_AND_MEAN
#define RGFW_DEBUG // Todo: delete on release!
#define RGFW_IMPLEMENTATION
#include <RGFW.h>

#include <cstdio>

int main(int, char **)
{
  RGFW_window *win;

  std::puts("winyl!");

  win = RGFW_createWindow("winyl (Milestone 1)", RGFW_RECT(0, 0, 400, 500),
                          RGFW_windowCenter | RGFW_windowNoResize);

  if (win == nullptr)
  {
    std::puts("Failed to create a window");
    return 1;
  }

  RGFW_window_makeCurrent(win);

  if (gl3wInit() != 0)
  {
    std::puts("Failed to initialize OpenGL");
    return 1;
  }

  glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

  while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
    RGFW_window_checkEvents(win, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    RGFW_window_swapBuffers(win);
  }

  return 0;
}