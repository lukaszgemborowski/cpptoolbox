cmake_minimum_required (VERSION 3.2)
project (libcpputil)

include(CMakeDependentOption)

option (CONFIG_TESTS		"build unit tests" OFF)
option (CONFIG_EXAMPLES		"build examples" OFF)

set    (TOOLBOX_CXX_STANDARD "14"   CACHE STRING "Define current C++ standard")
option (TOOLBOX_HAS_STDLIB          "Do we have standard C++ library" ON)
option (TOOLBOX_USE_FMT             "Download fmt library" OFF)

cmake_dependent_option(TOOLBOX_HAS_EXCEPTIONS "Do we have exceptions support" ON "TOOLBOX_HAS_STDLIB" OFF)

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

set(
    sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/argv/dummy.cxx
)

if (TOOLBOX_HAS_STDLIB)
    list(
        APPEND sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/argv/parser.cxx
    )

    if (TOOLBOX_CXX_STANDARD GREATER "16")
        list(
            APPEND sources
            ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/string.cxx
        )
    endif ()
endif ()

add_library(
    cpptoolbox
    STATIC
    ${sources}
)

target_include_directories (
    cpptoolbox
    INTERFACE ${include-dir}
    PRIVATE ${include-dir}
)

if (CONFIG_TESTS)
    add_subdirectory(tests)
endif (CONFIG_TESTS)

if (CONFIG_EXAMPLES AND TOOLBOX_HAS_STDLIB)
    add_subdirectory(examples)
endif (CONFIG_EXAMPLES AND TOOLBOX_HAS_STDLIB)

file(GLOB json-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/json/*.hpp")
file(GLOB meta-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/meta/*.hpp")
file(GLOB toolbox-alloc-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/alloc/*.hpp")
file(GLOB toolbox-argv-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/argv/*.hpp")
file(GLOB toolbox-container-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/container/*.hpp")
file(GLOB toolbox-cpp-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/cpp/*.hpp")
file(GLOB toolbox-curl-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/curl/*.hpp")
file(GLOB toolbox-gcc-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/gcc/*.hpp")
file(GLOB toolbox-marshall-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/marshall/*.hpp")
file(GLOB toolbox-std-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/std/*.hpp")
file(GLOB toolbox-testing-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/testing/*.hpp")
set(toolbox-config-header ${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/config.h)
file(GLOB toolbox-main-headers "${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/*.hpp")

install(FILES ${json-headers} DESTINATION include/json)
install(FILES ${meta-headers} DESTINATION include/meta)
install(FILES ${toolbox-alloc-headers} DESTINATION include/toolbox/alloc)
install(FILES ${toolbox-argv-headers} DESTINATION include/toolbox/argv)
install(FILES ${toolbox-container-headers} DESTINATION include/toolbox/container)
install(FILES ${toolbox-cpp-headers} DESTINATION include/toolbox/cpp)
install(FILES ${toolbox-curl-headers} DESTINATION include/toolbox/curl)
install(FILES ${toolbox-gcc-headers} DESTINATION include/toolbox/gcc)
install(FILES ${toolbox-marshall-headers} DESTINATION include/toolbox/marshall)
install(FILES ${toolbox-std-headers} DESTINATION include/toolbox/std)
install(FILES ${toolbox-testing-headers} DESTINATION include/toolbox/testing)
install(FILES ${toolbox-config-header} DESTINATION include/toolbox/)
install(FILES ${toolbox-main-headers} DESTINATION include/toolbox/)

include(FindCURL)

if (CURL_FOUND)
    target_link_libraries(cpptoolbox CURL::libcurl)
endif ()

if (TOOLBOX_USE_FMT)
    # find package
    find_package(fmt 5.0)

    if (NOT fmt_FOUND)
        message(STATUS "No fmt library found via find_package")
        if (TARGET fmt)
            message (WARNING "fmt target available in current configuration, can't determine version. Trying to use it anyway...")
            target_link_libraries(cpptoolbox fmt::fmt)
        else ()
            message (FATAL_ERROR "fmt support requested but library not found")
        endif ()
    else ()
        target_link_libraries(cpptoolbox fmt::fmt)
    endif ()
endif ()
