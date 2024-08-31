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

set_target_properties(SDL_uclibc  PROPERTIES FOLDER "dependencies/sdl_3")
set_target_properties(SDL3_test   PROPERTIES FOLDER "dependencies/sdl_3")
set_target_properties(SDL3-shared PROPERTIES FOLDER "dependencies/sdl_3")

#find_package(sdl_3 REQUIRED)

## Create a target to make the SDL2 files relevant
#add_custom_target(sdl_copy ALL
#  DEPENDS ${_added_copy_files}
#)
#set_target_properties(sdl_copy PROPERTIES FOLDER "meta")
#
## Helper function for setting up project
#function(configure_sdl_target project_ref)
#  message(STATUS "Configure SDL Target ${project_ref}")
#  
#  target_link_libraries(${project_ref} ${SDL2_LIBRARIES})
#endfunction()