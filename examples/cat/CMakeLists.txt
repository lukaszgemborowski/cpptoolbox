if (TOOLBOX_CXX_STANDARD GREATER "16")
    add_executable(cat main.cxx)
    target_link_libraries(cat cpptoolbox)
endif ()
