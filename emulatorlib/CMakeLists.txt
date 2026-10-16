file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS src/*.h)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE MOCK_HEADERS CONFIGURE_DEPENDS include/emulatorlib/test/*.h)

if (MOCK_HEADERS)
    list(REMOVE_ITEM HEADERS ${MOCK_HEADERS})
endif ()

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
        FILES ${PRIVATE_HEADERS}
)

#target_include_directories(emulatorlib PUBLIC include PRIVATE src)

target_link_libraries(emulatorlib PUBLIC utilitylib)

add_subdirectory(test)