src_dir = include_directories('.')

subdir('dbus')

executable(
    'longbar',
    files(
        'longbar.c',
        'main.c',
        'modules/clock-module.c',
    ),
    dependencies: [
        dep_dbus,
        gtk,
        gtk_layer_shell,
    ],
    install: false
)
