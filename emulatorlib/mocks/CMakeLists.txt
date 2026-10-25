file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(emulatorlibMocks INTERFACE)

target_sources(emulatorlibMocks
        INTERFACE
        FILE_SET interface_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES ${HEADERS})

target_link_libraries(emulatorlibMocks
        INTERFACE
        emulatorlib
        GTest::GTest)