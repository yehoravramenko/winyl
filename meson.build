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

executable('winyl', sources,
  include_directories: include_directories(includes),
  link_with: [imgui_lib]
)
