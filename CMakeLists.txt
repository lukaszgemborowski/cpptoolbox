cmake_minimum_required (VERSION 3.2)
project (libcpputil)

option (CONFIG_TESTS		"build unit tests" OFF)
option (CONFIG_EXAMPLES		"build examples" OFF)

set    (TOOLBOX_CXX_STANDARD "14"   CACHE STRING "Define current C++ standard")
option (TOOLBOX_HAS_STDLIB          "Do we have standard C++ library" ON)
option (TOOLBOX_HAS_EXCEPTIONS		"Do we have exceptions support" ON)

if (TOOLBOX_CXX_STANDARD EQUAL "11")
	set (CMAKE_CXX_STANDARD 11)
elseif (TOOLBOX_CXX_STANDARD EQUAL "14")
	set (CMAKE_CXX_STANDARD 14)
elseif (TOOLBOX_CXX_STANDARD EQUAL "17")
	set (CMAKE_CXX_STANDARD 17)
else ()
	message (FATAL_ERROR "Wrong TOOLBOX_CXX_STANDARD specified, should be one of: 11, 14, 17")
endif ()

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/config.h
)

set(include-dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library (cpptoolbox INTERFACE)
target_include_directories (cpptoolbox INTERFACE ${include-dir})

if (CONFIG_TESTS)
    add_subdirectory(tests)
endif (CONFIG_TESTS)

if (CONFIG_EXAMPLES)
    add_subdirectory(examples)
endif (CONFIG_EXAMPLES)
