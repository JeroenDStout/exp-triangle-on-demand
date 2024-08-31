message(STATUS "Setup sdl_3")

include(FetchContent)

# Obtain SDL3
FetchContent_Declare(
  sdl_3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        Main
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(sdl_3)

set_target_properties(SDL_uclibc SDL3_test SDL3-shared PROPERTIES
                      FOLDER "dependencies/sdl_3"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    
# Config
function(configure_sdl_target project_ref)
  message(STATUS "Configure SDL Target ${project_ref}")
  
  include_directories(${sdl_3_SOURCE_DIR}/include)
  target_link_libraries(${project_ref} SDL3-shared)
endfunction()
