file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS src/*.h)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(emulatorlib STATIC)

target_sources(emulatorlib
        PUBLIC
        FILE_SET interface_headers
        TYPE HEADERS
        BASE_DIRS include
        FILES ${HEADERS}

        PRIVATE
        ${SOURCES}

        PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS src
        FILES ${PRIVATE_HEADERS})

target_link_libraries(emulatorlib
        PUBLIC
        utilitylib)

add_subdirectory(mocks)
add_subdirectory(test)