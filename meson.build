project(
  'winyl',
  [
    'cpp',
    'c',
  ],
  default_options: [
    'cpp_std=c++17',
  ],
)

sources = files(
  'src/winyl.cc',
  'thirdparty/gl3w/src/gl3w.c',
)

includes = include_directories(
  'thirdparty/RGFW',
  'thirdparty/gl3w/include',
)

executable(
  'winyl',
  sources,
  include_directories: includes,
  cpp_args: [
    '/WX',
  ],
)