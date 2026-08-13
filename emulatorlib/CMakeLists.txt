file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_library(emulatorlib STATIC ${SOURCES} ${HEADERS})

target_include_directories(emulatorlib PUBLIC include PRIVATE src)

target_link_libraries(emulatorlib PUBLIC utilitylib)

add_subdirectory(test)