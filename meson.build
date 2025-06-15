project(
  'Winyl',
  'cpp',
  default_options: ['cpp_std=c++17', 'warning_level=3', 'werror=true'],
)

includes = ['thirdparty/RGFW', 'thirdparty/imgui', 'thirdparty/imgui_imp_rgfw.h']

sources = ['src/winyl.cpp']

imgui_sources = [
  'thirdparty/imgui/imgui.cpp',
  'thirdparty/imgui/imgui_draw.cpp',
  'thirdparty/imgui/imgui_widgets.cpp',
  'thirdparty/imgui/imgui_tables.cpp',
  'thirdparty/imgui/backends/imgui_impl_opengl3.cpp',
]
imgui_lib = static_library(
  'imgui',
  imgui_sources,
  include_directories: include_directories('thirdparty/imgui'),
)

os = build_machine.system()
deps = []

if os == 'linux'
  x11_dep = dependency('x11', required: true)
  gl_dep = dependency('GL', required: true)
  xrandr_dep = dependency('xrandr', required: true)
  deps = [x11_dep, gl_dep, xrandr_dep]
elif os == 'windows'
  # currently nothing to do.
  # ogl_dep = dependency('opengl32', required: true)
  # gdi_dep = dependency('gdi32', required: true)
  # deps = [ogl_dep, gdi_dep]
else
  error('This operating system is unsupported')
endif

executable(
  'winyl',
  sources,
  include_directories: include_directories(includes),
  dependencies: [deps],
  link_with: [imgui_lib],
)