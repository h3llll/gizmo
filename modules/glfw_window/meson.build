inc_glfw_window = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

glfw_dep = dependency('glfw3')
event_dep = dependency('event')

glfw_window_lib = shared_library(
  'glfw_window',
  src_files,
  include_directories: [inc_glfw_window, inc_root],
  dependencies: [glfw_dep, event_dep],
)

glfw_window_dep = declare_dependency(
  include_directories: inc_glfw_window,
  link_with: glfw_window_lib,
  dependencies: [glfw_dep, event_dep]
  ,
)

meson.override_dependency('window', glfw_window_dep)
