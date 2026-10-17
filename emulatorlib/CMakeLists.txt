file(GLOB_RECURSE MODULES_SOURCES CONFIGURE_DEPENDS src/*.cppm)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HANDLER_PARTITIONS CONFIGURE_DEPENDS src/cpu/instruction_handlers/*.cpp)
list(REMOVE_ITEM SOURCES ${HANDLER_PARTITIONS})

add_library(emulatorlib STATIC)

target_sources(emulatorlib
        PUBLIC
        FILE_SET interface_modules
        TYPE CXX_MODULES
        BASE_DIRS src
        FILES ${MODULES_SOURCES}

        PRIVATE
        FILE_SET handler_partitions
        TYPE CXX_MODULES
        BASE_DIRS src
        FILES ${HANDLER_PARTITIONS}

        PRIVATE
        ${SOURCES}
)

target_link_libraries(emulatorlib PUBLIC utilitylib)

add_subdirectory(test)
