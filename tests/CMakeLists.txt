include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        CCirtucalBufferExt_test
        CCirtucalBufferExt_test.cpp
)

add_executable(
        CCirtucalBuffer_test
        CCirtucalBuffer_test.cpp
)


target_link_libraries(
        CCirtucalBufferExt_test
        GTest::gtest_main
)

target_link_libraries(
        CCirtucalBuffer_test
        GTest::gtest_main
)

target_include_directories(CCirtucalBufferExt_test PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(CCirtucalBuffer_test PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(CCirtucalBufferExt_test)
gtest_discover_tests(CCirtucalBuffer_test)