
test_version = executable(
  'test-version',
  'test_version.c',
  include_directories: rwinit_inc,
)

test(
  'version',
  test_version,
)
test_reaper = executable(
  'test-reaper',
  [
    'test_reaper.c',
    '../src/core.c',
  ],
  include_directories: rwinit_inc,
)

test(
  'child-reaping',
  test_reaper,
)
test(
  'version-cli',
  rwinit,
  args: ['--version'],
  should_fail: false,
)

test(
  'help-cli',
  rwinit,
  args: ['--help'],
  should_fail: false,
)
