inc_event = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

event_lib = shared_library(
  'event',
  src_files,
  include_directories: [inc_event, inc_root],
)

event_dep = declare_dependency(
  include_directories: inc_event,
  link_with: event_lib,
)

meson.override_dependency('event', event_dep)
