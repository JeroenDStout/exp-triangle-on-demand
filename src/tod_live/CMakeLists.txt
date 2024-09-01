init_directory(tod_live)

# Define the tod_live project
init_project(tod_live "triangle_on_demand")

# Sources
setup_project_source(tod_live "core"
  src:
    main.cpp
    live.cpp
  include:
    live.h
)

# Target
configure_project_executable(tod_live)
configure_project_console(tod_live)
configure_cxx_target(tod_live)
configure_target_outputs(tod_live)
configure_project_sdl(tod_live)
configure_publish_exec(tod_live)
	
add_dependencies(tod_live tod_core repo_version)
target_link_libraries(tod_live PRIVATE tod_core)
