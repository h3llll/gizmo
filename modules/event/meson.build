inc_event = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

dyarr_dep = dependency('dyarr')

event_lib = shared_library(
  'event',
  src_files,
  include_directories: [inc_event, inc_root],
  dependencies: [dyarr_dep],
)

event_dep = declare_dependency(
  include_directories: inc_event,
  link_with: event_lib,
  dependencies: dyarr_dep,
)

meson.override_dependency('event', event_dep)
