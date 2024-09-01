init_directory(tod_core)

# Define the tod_core project
init_project(tod_core "triangle_on_demand")

# Sources
setup_project_source(tod_core "core/data"
  src:
    data_gpu.cpp
  include:
    data_gpu.h
)
setup_project_source(tod_core "core/policy"
  src:
    poli_gpu.cpp
  include:
    poli_gpu.h
)
setup_project_source(tod_core "core/proc"
  src:
    proc_gpu.cpp
    proc_tod.cpp
  include:
    proc_gpu.h
    proc_tod.h
)
setup_project_source(tod_core "third_party"
  src:
    imp_shadercross.cpp
  include:
    inc_shadercross.h
    inc_sdl.h
)

# Target
configure_project_static_lib(tod_core)
configure_cxx_target(tod_core)
configure_target_outputs(tod_core)
configure_project_sdl(tod_core)
configure_publish_exec(tod_core)
	
add_dependencies(tod_core repo_version)
