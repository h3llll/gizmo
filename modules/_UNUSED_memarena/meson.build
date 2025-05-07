inc_memarena = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

memarena_lib = shared_library(
  'memarena',
  src_files,
  include_directories: [
    inc_memarena,
    inc_root,
  ],
)

memarena_dep = declare_dependency(
  include_directories: inc_memarena,
  link_with: memarena_lib,
)

meson.override_dependency('memarena', memarena_dep)
