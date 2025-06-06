#ifndef NDEBUG
#define RGFW_DEBUG // Todo: delete on release!
#endif
#define RGFW_IMPLEMENTATION
#include <RGFW.h>

#include <stdio.h>

int main(int, char **)
{
  RGFW_window *win;

  RGFW_setGLHint(RGFW_glMajor, 4);
  RGFW_setGLHint(RGFW_glMinor, 6);
  RGFW_setGLHint(RGFW_glProfile, RGFW_glCore);

  win = RGFW_createWindow("winyl (Milestone 1)", RGFW_RECT(0, 0, 400, 500),
                          RGFW_windowCenter | RGFW_windowNoResize);

  if (win == nullptr)
  {
    puts("Failed to create a window");
    return 1;
  }

#ifndef NDEBUG
  auto gl_version = ::glGetString(GL_VERSION);
  printf("%s\n", gl_version);
#endif

  glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

  while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
    RGFW_window_checkEvents(win, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    RGFW_window_swapBuffers(win);
  }

  return 0;
}