function(configure_target_outputs project_ref)
  message(STATUS "Configure ouputs ${project_ref}")
  
  # Projects	
  set_property(TARGET ${project_ref} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$(Configuration)")
  
  if(MSVC)
    set_target_properties(${project_ref} PROPERTIES COMPILE_PDB_NAME               "$(ProjectName)")
    set_target_properties(${project_ref} PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  endif()
endfunction()
