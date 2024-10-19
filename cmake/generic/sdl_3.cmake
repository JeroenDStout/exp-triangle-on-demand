message(STATUS "Setup sdl_3")

include(FetchContent)

# Obtain SDL3
FetchContent_Declare(
  sdl_3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        541ba33714376d29795fcc22add8e624603d9727
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(sdl_3)

set_target_properties(SDL_uclibc SDL3_test SDL3-shared PROPERTIES
                      FOLDER "dependencies/sdl_3"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Obtain SDL_gpu_shadercross
# Single-file header to support shader compilation
FetchContent_Declare(
  sdl_gpu_shadercross
  GIT_REPOSITORY https://github.com/flibitijibibo/SDL_gpu_shadercross.git
  GIT_TAG        8c45abff9a8195da6779e63b0a7dc6f90013cf01
  GIT_PROGRESS   TRUE
  SOURCE_DIR     $CACHE{FETCHCONTENT_BASE_DIR}/_deps/sdl_gpu_shadercross-src/shadercross
)
FetchContent_MakeAvailable(sdl_gpu_shadercross)
file(GLOB sdl_gpu_shadercross_source_h ${sdl_gpu_shadercross_SOURCE_DIR}/*.h)
add_library(sdl_gpu_shadercross INTERFACE ${sdl_gpu_shadercross_source_h})
set_target_properties(sdl_gpu_shadercross PROPERTIES FOLDER "dependencies/sdl_3")
    
# Config
function(configure_project_sdl project_ref)
  message(STATUS "Configure SDL ${project_ref}")
  
  include_directories(${sdl_3_SOURCE_DIR}/include)
  include_directories(${sdl_gpu_shadercross_SOURCE_DIR}/..)
  target_link_libraries(${project_ref} PRIVATE SDL3-shared)
endfunction()
