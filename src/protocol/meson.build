wayland_scanner = find_program('wayland-scanner', required: true)

wayland_scanner_code = generator(
    wayland_scanner,
    output: '@BASENAME@-protocol.c',
    arguments: [
        'private-code',
        '@INPUT@',
        '@OUTPUT@',
    ],
)

wayland_scanner_client = generator(
    wayland_scanner,
    output: '@BASENAME@-protocol.h',
    arguments: [
        'client-header',
        '@INPUT@',
        '@OUTPUT@',
    ],
)

client_protocols = [
    'wlr-foreign-toplevel-management-unstable-v1.xml',
]

client_protos_src = []
client_protos_headers = []

foreach xml : client_protocols
    client_protos_src += wayland_scanner_code.process(xml)
    client_protos_headers += wayland_scanner_client.process(xml)
endforeach

lib_longbar_protocol = static_library(
    'longbar_protocol',
    client_protos_src + client_protos_headers,
    dependencies: [
        wayland_client,
    ],
)

dep_protocol = declare_dependency(
    link_with: lib_longbar_protocol,
    sources: client_protos_headers,
)
