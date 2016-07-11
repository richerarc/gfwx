set(MODULE_NAME "gfwx-headers")
set(MODULE_PREFIX "GFWX_HEADERS")

set(${MODULE_PREFIX}_HEADERS
        gfwx.hpp)

add_custom_target(${MODULE_NAME} SOURCES ${${MODULE_PREFIX}_HEADERS})