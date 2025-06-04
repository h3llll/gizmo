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

USE_SDL = false # false: glfw
USE_GL = true # false: vulkan
USE_PA = true #false: idk yet maybe it's js gonna be only port audio

add_global_arguments('-DDEBUG_BUILD', '-DACTIVE_LOG_LEVEL=3', language: 'c')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
inc_root = include_directories('include', 'modules')

# lower level modules
subdir('modules/dyarr')
subdir('modules/event')
subdir('modules/io')

if USE_SDL
  # subdir('modules/sdl_window')
else
  subdir('modules/glfw_window')
endif

if USE_GL
  subdir('modules/gl_renderer')
else
  #  subdir('modules/vk_renderer')
endif

subdir('modules/audio')

io_dep = dependency('io')
dyarr_dep = dependency('dyarr')
event_dep = dependency('event')
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
    io_dep,
  ],
  include_directories: [
    inc_root,
  ],
)
