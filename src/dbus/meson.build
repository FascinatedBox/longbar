gdbus_codegen = find_program('gdbus-codegen', required: true)

gdbus_codegen_code = generator(
    gdbus_codegen,
    output: '@BASENAME@.c',
    arguments: [
        '--generate-c-code',
        '@BASENAME@',
        '--output-dir',
        '@BUILD_DIR@',
        '--interface-prefix',
        'com.fascinatedbox.',
        '@INPUT@',
    ],
)

client_protocols = [
    'dbus-interface.xml',
]

dbus_protos_src = [
    'dbus-dispatch.c'
]

foreach xml : client_protocols
    dbus_protos_src += gdbus_codegen_code.process(xml)
endforeach

lib_longbar_dbus = static_library(
    'longbar_dbus',
    dbus_protos_src,
    dependencies: [
        gtk,
    ],
    include_directories: [
        src_dir,
    ],
)

dep_dbus = declare_dependency(
    link_with: lib_longbar_dbus,
    sources: dbus_protos_src,
)
