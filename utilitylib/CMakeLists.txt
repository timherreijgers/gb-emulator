file(GLOB_RECURSE MODULES_SOURCES CONFIGURE_DEPENDS src/*.cppm)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS src/*.h)

add_library(utilitylib STATIC)

set_target_properties(utilitylib PROPERTIES LINKER_LANGUAGE CXX)

target_sources(utilitylib
        PUBLIC
        FILE_SET interface_modules
        TYPE CXX_MODULES
        BASE_DIRS src
        FILES ${MODULES_SOURCES}

        PRIVATE
        ${SOURCES}

        PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS src
        FILES ${PRIVATE_HEADERS})

add_subdirectory(test)
