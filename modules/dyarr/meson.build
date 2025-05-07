inc_dyarr = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

dyarr_lib = shared_library(
  'dyarr',
  src_files,
  include_directories: [inc_dyarr, inc_root],
)

dyarr_dep = declare_dependency(
  include_directories: inc_dyarr,
  link_with: event_lib,
)

meson.override_dependency('dyarr', dyarr_dep)
