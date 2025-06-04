inc_io = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

io_lib = shared_library(
  'io',
  src_files,
  include_directories: [inc_io, inc_root],
)

io_dep = declare_dependency(
  include_directories: inc_io,
  link_with: io_lib,
)

meson.override_dependency('io', io_dep)
