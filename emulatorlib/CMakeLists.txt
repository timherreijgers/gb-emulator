file(GLOB_RECURSE MODULES_SOURCES CONFIGURE_DEPENDS src/*.cppm)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.cxx)

add_library(emulatorlib STATIC)

target_sources(emulatorlib
        PUBLIC
        FILE_SET interface_modules
        TYPE CXX_MODULES
        BASE_DIRS src
        FILES ${MODULES_SOURCES}

        PRIVATE
        ${SOURCES}
)

target_link_libraries(emulatorlib PUBLIC utilitylib)

add_subdirectory(test)
