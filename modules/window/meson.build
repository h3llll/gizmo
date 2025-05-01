inc_window = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

glfw_dep = dependency('glfw3')
event_dep = dependency('event')

window_lib = shared_library(
  'window',
  src_files,
  include_directories: [inc_window, inc_root],
  dependencies: [glfw_dep, event_dep]
)

window_dep = declare_dependency(
  include_directories: inc_window,
  link_with: window_lib,
)

meson.override_dependency('window', window_dep)
