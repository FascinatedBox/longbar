src_dir = include_directories('.')
protocol_dir = include_directories('protocol')

subdir('dbus')
subdir('protocol')

executable(
    'longbar',
    files(
        'longbar.c',
        'main.c',
        'modules/clock-module.c',
        'modules/taskbar-module.c',
        'wayland-event-source.c',
    ),
    dependencies: [
        dbus_dep,
        gtk,
        gtk_layer_shell,
        protocol_dep,
        wayland_client,
    ],
    include_directories: [
        protocol_dir,
    ],
    install: false,
)
