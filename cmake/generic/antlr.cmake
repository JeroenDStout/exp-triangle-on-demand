message(STATUS "Config antlr")


function(antlr_py_build_single source destination append_generated_files)
  sources_get_canonprint_filepath(${source} source_relpath)
  sources_get_canonprint_filepath("${abs_gen_script}/${destination}" destination_relpath)
  
  get_filename_component(base_out_name ${source} NAME_WLE)
  
  set(loc_used_files ${${append_generated_files}})
  list(APPEND loc_used_files "${destination}/${base_out_name}Lexer.py")
  list(APPEND loc_used_files "${destination}/${base_out_name}Visitor.py")
  list(APPEND loc_used_files "${destination}/${base_out_name}Parser.py")
  
  message(STATUS "Antlr setup: ${source_relpath} -> ${destination_relpath}")
  message(STATUS " - ${destination}/${base_out_name}Lexer.py")
  message(STATUS " - ${destination}/${base_out_name}Visitor.py")
  message(STATUS " - ${destination}/${base_out_name}Parser.py")
  
  file(MAKE_DIRECTORY "${abs_gen_script}/${destination}")
  
  add_custom_command(
    COMMENT           "Antlr: ${source} -> ${abs_gen_script}/${destination}/${base_out_name}"
    OUTPUT            "${abs_gen_script}/${destination}/${base_out_name}Lexer.py"
                      "${abs_gen_script}/${destination}/${base_out_name}Visitor.py"
                      "${abs_gen_script}/${destination}/${base_out_name}Parser.py"
    COMMAND           antlr4
                      -Dlanguage=Python3
                      "${source}"
                      -o "${abs_gen_script}/${destination}/"
                      -no-listener
                      -visitor
    DEPENDS           ${source}
    WORKING_DIRECTORY "${abs_gen_script}/${destination}/"
    VERBATIM
  )
  
  set(${append_generated_files} ${loc_used_files} PARENT_SCOPE)
endfunction()
