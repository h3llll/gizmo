inc_sdl_window = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

sdl_dep = dependency('SDL2')
event_dep = dependency('event')

sdl_window_lib = shared_library(
  'glfw_window',
  src_files,
  include_directories: [inc_sdl_window, inc_root],
  dependencies: [sdl_dep, event_dep],
)

sdl_window_dep = declare_dependency(
  include_directories: inc_sdl_window,
  link_with: sdl_window_lib,
  dependencies: [sdl_dep, event_dep]
  ,
)

meson.override_dependency('window', sdl_window_dep)
