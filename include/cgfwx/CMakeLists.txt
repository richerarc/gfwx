set(MODULE_NAME "cgfwx-headers")
set(MODULE_PREFIX "CGFWX_HEADERS")

set(${MODULE_PREFIX}_HEADERS
        cgfwx.h)

add_custom_target(${MODULE_NAME} SOURCES ${${MODULE_PREFIX}_HEADERS})