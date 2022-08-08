cmake_minimum_required(VERSION 3.10)
project(WinTUI DESCRIPTION "Terminal based UI" LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wnull-dereference -Wshadow -Wformat=2 -pedantic)
endif()

include_directories("${CMAKE_SOURCE_DIR}/include")
add_library(wintui wintui/wintui.c)

add_executable(main main.c)
target_link_libraries(main wintui m)


# Start testing
# include(CTest)
# add_executable(test-modname tests/test-modname.c)
# target_link_libraries(test-modname modname)
# add_test(NAME "test-modname" COMMAND test-modname)
# set_tests_properties(test-modname PROPERTIES TIMEOUT 5)
