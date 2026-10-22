
project(
  'rwinit',
  'c',
  version: '0.1.0',
  meson_version: '>=1.2.0',
  default_options: [
    'c_std=c17',
    'warning_level=3',
    'werror=true',
  ],
)

rwinit_inc = include_directories('include')

rwinit = executable(
  'rwinit',
  [
    'src/main.c',
    'src/core.c',
  ],
  include_directories: rwinit_inc,
  install: true,
)

subdir('tests')
