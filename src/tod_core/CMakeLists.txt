init_directory(tod_core)

# Define the tod_core project
init_project(tod_core "triangle_on_demand")

# Sources
setup_project_source(tod_core "core"
  src:
    triangle_server.cpp
  include:
    triangle_server.h
)
setup_project_source(tod_core "include"
  include:
    inc_sdl.h
)

# Target
configure_project_static_lib(tod_core)
configure_cxx_target(tod_core)
configure_target_outputs(tod_core)
configure_sdl_target(tod_core)
configure_publish_exec(tod_core)
	
add_dependencies(tod_core repo_version)
