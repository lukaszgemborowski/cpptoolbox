project(libs-fmt)
cmake_minimum_required (VERSION 3.2)
include(ExternalProject)

ExternalProject_Add(
    cpptoolbox-fmt
    URL https://github.com/fmtlib/fmt/archive/5.3.0.tar.gz
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ""
    PREFIX "."
)

