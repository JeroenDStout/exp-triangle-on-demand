message(STATUS "Config spv compiler")


macro(setup_project_source_spv project_ref target_name project_source_group)
  message(STATUS "Add compile-to-spv sources for ${project_ref}/${project_source_group}")
  setup_project_source_internal(${project_ref} ${project_source_group} "script:" ${ARGN})
  
  set(added_assets "")
  foreach(asset_file ${new_files_src})
    file(RELATIVE_PATH rel_file ${abs_can_script} ${asset_file})
    get_filename_component(directory ${abs_gen_assets}/${rel_file} DIRECTORY)
    file(MAKE_DIRECTORY ${directory})
    setup_svp_compile_file(${asset_file} ${abs_gen_assets}/${rel_file}.spv)
    list(APPEND added_assets ${abs_gen_assets}/${rel_file}.spv)
  endforeach()
  
  add_custom_target(${target_name} ALL
     DEPENDS ${added_assets}
  )
  set_target_properties(${target_name} PROPERTIES FOLDER "assets")
endmacro()

function(setup_svp_compile_file source destination)
  sources_get_canonprint_filepath(${source} source_relpath)
  sources_get_canonprint_filepath(${destination} destination_relpath)
  
  message(STATUS " - ${source_relpath} -> ${destination_relpath}")
  
  if (NOT EXISTS ${destination})
    file(TOUCH ${destination})
  endif()
  
  add_custom_command(
    COMMENT "\nSpirv (glsllang_validator) compiling ${source} to ${destination}"
    OUTPUT  ${destination}
    DEPENDS	${source}
    COMMAND glslangValidator -V "${source}" -o "${destination}"
    VERBATIM
  )
endfunction()