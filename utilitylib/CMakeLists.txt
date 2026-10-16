file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(utilitylib INTERFACE)

target_sources(utilitylib
        INTERFACE
        FILE_SET interface_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES ${HEADERS})

add_subdirectory(test)