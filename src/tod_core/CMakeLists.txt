init_directory(tod_core)

# Define the tod_core project
init_project(tod_core "triangle_on_demand")

# Sources
setup_project_source(tod_core "core/data"
  include:
    data_gpu.h
    data_tod.h
    data_vert.h
)
setup_project_source(tod_core "core/policy"
  include:
    poli_gpu.h
    poli_tod.h
)
setup_project_source(tod_core "core/proc"
  src:
    proc_gpu.cpp
    proc_tod.cpp
  include:
    proc_gpu.h
    proc_gpu_impl.h
    proc_tod.h
)
setup_project_source(tod_core "sugar"
  include:
    io.h
    refvalue.h
)
setup_project_source(tod_core "third_party"
  src:
    imp_shadercross.cpp
  include:
    inc_eigen.h
    inc_shadercross.h
    inc_sdl.h
)
setup_project_source(tod_core "assets"
  src:
    assets.cpp
  include:
    assets.h
)
setup_project_source_spv(tod_core tod_core_spv "assets/content"
  shaders/triangle.vert
  shaders/triangle.frag
)

# Target
configure_project_static_lib(tod_core)
configure_cxx_target(tod_core)
configure_target_outputs(tod_core)
configure_project_sdl(tod_core)
configure_publish_exec(tod_core)
configure_asset_folder(tod_core tod_core_assets ${abs_gen_assets}/shaders assets/shaders)
    
add_dependencies(tod_core repo_version)
