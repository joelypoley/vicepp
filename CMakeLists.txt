cmake_minimum_required(VERSION 2.8.12)
project(Vice)
# enable_testing()

# set( CMAKE_VERBOSE_MAKEFILE ON )
set(CMAKE_CXX_STANDARD, 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -Wall -Wextra -pedantic -Wno-sign-compare -Wno-c++17-extensions -Werror=return-type")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -Wno-c++17-extensions")
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=memory -Wall -Wextra -pedantic -Wnosign-compare")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=memory")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(vice vice.cc init.cc bitboards.cc board.cc hashkeys.cc data.cc attack.cc movegen.cc io.cc validate.cc makemove.cc perft.cc)
add_executable(vice_test vice_test.cc init.cc bitboards.cc board.cc hashkeys.cc data.cc attack.cc movegen.cc io.cc validate.cc makemove.cc perft.cc)

target_link_libraries(vice ${CONAN_LIBS})
target_link_libraries(vice_test ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(vice_test)