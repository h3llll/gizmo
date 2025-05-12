inc_audio = include_directories('include')

list_script = meson.project_source_root() / 'scripts' / 'list-all-src.sh'
src_files = run_command(
  list_script,
  'src',
  check: true,
).stdout().strip().split('\n')

al_dep = dependency('openal')
event_dep = dependency('event')

audio_lib = shared_library(
  'audio',
  src_files,
  include_directories: [inc_audio, inc_root],
  dependencies: [
    al_dep,
    event_dep,
  ],
)

audio_dep = declare_dependency(
  include_directories: inc_audio,
  link_with: audio_lib,
)

meson.override_dependency('audio', audio_dep)
