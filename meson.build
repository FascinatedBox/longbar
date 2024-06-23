project('longbar',
    ['c'],
    version: '0.0.1',
    license: 'GPLv2',
    meson_version: '>=0.45.1',
    default_options: [
        'c_std=gnu11',
        'warning_level=3'
    ])

add_project_arguments(
    [
        '-Wno-pedantic',
        '-Werror=unused-function',
    ],
    language: 'c')

gtk = dependency('gtk+-3.0', version: '>=3.22.0', required: true)
gtk_layer_shell = dependency('gtk-layer-shell-0', version: '>=0.7.0', required: true)

subdir('src')
