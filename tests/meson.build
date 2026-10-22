test_version = executable(
  'test-version',
  'test_version.c',
  include_directories: rwinit_inc,
)

test(
  'version',
  test_version,
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
