inc_renderer = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

gl_dep = dependency('GL')
memarena_dep = dependency('memarena')

renderer_lib = shared_library(
  'renderer',
  src_files,
  include_directories: [inc_renderer, inc_root],
  dependencies: [
    gl_dep,
    memarena_dep,
  ],
)

renderer_dep = declare_dependency(
  include_directories: [inc_renderer],
  link_with: renderer_lib,
)

meson.override_dependency('renderer', renderer_dep)
