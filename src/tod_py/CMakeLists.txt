init_directory(tod_py)

# Define the tod_py project
init_project(tod_py "triangle_on_demand")

# Sources
setup_project_source(tod_py "nanobind"
  src:
    nanobind.cpp
    nanobind_meta.cpp
)
setup_project_source(tod_py "core"
  src:
    data_tod_py.cpp
    proc_tod_py.cpp
  include:
    data_tod_py.h
    proc_tod_py.h
)
setup_project_source(tod_py "third_party"
  include:
    inc_nanobind.h
    inc_nanobind_stl.h
)

# Target
configure_project_nanobind(tod_py)
configure_cxx_target(tod_py)
configure_target_outputs(tod_py)
configure_project_sdl(tod_py)
configure_project_eigen(tod_py)
	
add_dependencies(tod_py tod_core repo_version)
target_link_libraries(tod_py PRIVATE tod_core)
