file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp src/*.h)

add_executable(utilitylibTest ${SOURCES})

target_link_libraries(utilitylibTest
        PUBLIC
        utilitylib
        PRIVATE
        GTest::gmock
        GTest::gmock_main)

target_include_directories(utilitylibTest PRIVATE src ../src)

add_test(UtilityLibTest utilitylibTest)