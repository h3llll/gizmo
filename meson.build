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

USE_SDL = true  # false: glfw
USE_GL = true   # false: vulkan
USE_PA = true #false: idk yet maybe it's js gonna be only port audio

add_global_arguments('-DDEBUG_BUILD', '-DACTIVE_LOG_LEVEL=3', language: 'c')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
inc_root = include_directories('include', 'modules')

# lower level modules
subdir('modules/dyarr')
subdir('modules/event')

if USE_SDL:
  subdir('modules/sdl_renderer')
else:
  subdir('modules/glfw_renderer')

subdir('modules/audio')

dyarr_dep = dependency('dyarr')
event_dep = dependency('event')
renderer_dep = dependency('renderer')
glfw_window_dep = dependency('glfw_window')
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
    glfw_window_dep,
    event_dep,
    dyarr_dep,
    audio_dep,
  ],
  include_directories: [
    inc_root,
  ],
)
