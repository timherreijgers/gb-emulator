file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp src/*.h)

add_executable(emulatorlibTest ${SOURCES})

target_link_libraries(emulatorlibTest
        PUBLIC
        emulatorlib
        PRIVATE
        GTest::gmock
        GTest::gmock_main)

target_include_directories(emulatorlibTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../src)

add_test(EmulatorLibTest emulatorlibTest)