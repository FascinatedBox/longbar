dbus_dir = include_directories('dbus')

executable(
    'longbar',
    files(
        'dbus/dbus-autogen.c',
        'dbus/dbus-dispatch.c',
        'longbar.c',
        'main.c',
        'modules/clock-module.c',
    ),
    dependencies: [
        gtk,
        gtk_layer_shell,
    ],
    include_directories: [
        dbus_dir,
    ],
    install: false
)
