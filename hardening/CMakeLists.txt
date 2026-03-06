project(hardening)

add_library(hardening INTERFACE)

option(HARDENING_UBSAN "Enable undefined behaviour sanitizer" ON)
option(HARDENING_ASAN "Enable address sanitizer" ON)

if (HARDENING_UBSAN)
    message(STATUS "[hardening] Undefined sanitizer:   Enabled")
    target_compile_options(hardening INTERFACE -fsanitize=undefined)
    target_link_options(hardening INTERFACE -fsanitize=undefined)
else()
    message(STATUS "[hardening] Undefined sanitizer:   Disabled")
endif()

if (HARDENING_ASAN)
    message(STATUS "[hardening] Address sanitizer:     Enabled")
    target_compile_options(hardening INTERFACE -fsanitize=address)
    target_link_options(hardening INTERFACE -fsanitize=address)
else()
    message(STATUS "[hardening] Address sanitizer:     Disabled")
endif()
