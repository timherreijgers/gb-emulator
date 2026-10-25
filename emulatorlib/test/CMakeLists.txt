file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS src/*.h)

add_executable(emulatorlibTest)

target_sources(emulatorlibTest
        PRIVATE
        ${SOURCES}

        PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS src
        FILES ${PRIVATE_HEADERS})

target_link_libraries(emulatorlibTest
        PUBLIC
        emulatorlib
        emulatorlibMocks
        PRIVATE
        GTest::gmock
        GTest::gmock_main)

target_include_directories(emulatorlibTest PRIVATE ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ../src)

add_test(EmulatorLibTest emulatorlibTest)