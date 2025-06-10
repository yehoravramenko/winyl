project('Winyl', 'cpp', 
        default_options: ['cpp_std=c++17', 'warning_level=3', 'werror=true'])

includes = ['thirdparty/RGFW', 'thirdparty/imgui']

sources = ['src/winyl.cpp']

imgui_sources = [
  'thirdparty/imgui/imgui.cpp',
  'thirdparty/imgui/imgui_draw.cpp',
  'thirdparty/imgui/imgui_widgets.cpp',
  'thirdparty/imgui/imgui_tables.cpp',
  'thirdparty/imgui/backends/imgui_impl_opengl3.cpp',
]
imgui_lib = static_library('imgui', imgui_sources,
  include_directories: include_directories('thirdparty/imgui')
)

x11_dep = dependency('x11', required: true)
gl_dep = dependency('GL', required: true)
xrandr_dep = dependency('xrandr', required: true)

executable('winyl', sources,
  include_directories: include_directories(includes),
  dependencies: [x11_dep, gl_dep, xrandr_dep],
  link_with: [imgui_lib]
)
