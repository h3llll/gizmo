project(
  'gizmo',
  'c',
  version: '0.1',
  default_options: [
    'warning_level=3',
    'c_std=c99',
    'buildtype=debug',
    'optimization=2',
    'c_args=-Wno-pedantic',
    'b_sanitize=address,undefined',
  ],
)

add_global_arguments('-DDEBUG_BUILD', '-DACTIVE_LOG_LEVEL=3', language: 'c')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
inc_root = include_directories('include')

subdir('modules/event')
subdir('modules/dyarr')

subdir('modules/renderer')
subdir('modules/window')
subdir('modules/audio')


event_dep = dependency('event')
dyarr_dep = dependency('dyarr')

renderer_dep = dependency('renderer')
window_dep = dependency('window')
audio_dep = dependency('audio')

src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

executable(
  'gizmo',
  src_files,
  dependencies: [
    renderer_dep,
    window_dep,
    event_dep,
    dyarr_dep,
    audio_dep,
  ],
  include_directories: [
    inc_root,
  ],
)
