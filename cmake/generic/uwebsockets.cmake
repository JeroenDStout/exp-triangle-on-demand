message(STATUS "Setup uWebSockets")

find_package(ZLIB REQUIRED)
find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")

find_library(USOCKETS   NAMES usockets PATHS ${vcpkg_lib_dir}   NO_DEFAULT_PATH REQUIRED)
find_library(USOCKETS_D NAMES usockets PATHS ${vcpkg_lib_dir_d} NO_DEFAULT_PATH REQUIRED)
find_library(UV_LIB     NAMES uv       PATHS ${vcpkg_lib_dir}   NO_DEFAULT_PATH REQUIRED)
find_library(UV_LIB_D   NAMES uv       PATHS ${vcpkg_lib_dir_d} NO_DEFAULT_PATH REQUIRED)

function(configure_project_uwebsockets project_ref)
  include_directories(${UWEBSOCKETS_INCLUDE_DIRS})
  target_link_libraries(${project_ref} PRIVATE ZLIB::ZLIB)
  target_link_libraries(${project_ref} PRIVATE optimized ${USOCKETS} debug ${USOCKETS_D})
  target_link_libraries(${project_ref} PRIVATE optimized ${UV_LIB}   debug ${UV_LIB_D})
endfunction()
