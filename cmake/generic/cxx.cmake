function(configure_cxx_target project_ref)
  message(STATUS "Configure CXX Target ${project_ref}")
  
  # Architecture
  set_property(TARGET ${project_ref} PROPERTY CXX_STANDARD 20)
  
  # Warnings
  if(MSVC)
    target_compile_options(${project_ref} PRIVATE
      /Wall /WX
      /wd4061 # Switch of Enum is not explicitly handled by a case label (even if default: is present).
      /wd4514 # Unreferenced inline function
      /wd4583 # Destructor is not implicitly called
      /wd4623 # Default constructor was implicitly defined as deleted
      /wd4625 # Copy constructor was implicitly defined as deleted
      /wd4626 # Assignment operator was implicitly defined as deleted
      /wd4668 # 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
      /wd4710 # Function not inlined
      /wd4711 # Function automatically inlined
      /wd4820 # Struct padding
      /wd4868 # Compiler may not enforce left-to-right evaluation order in braced initializer list
      /wd5026 # Move constructor was implicitly defined as deleted
      /wd5045 # Spectre mitigation
      /wd5219 # Implicit conversion from 'type-1' to 'type-2', possible loss of data
      /wd5257 # Enumeration was previously declared without a fixed underlying type
      /wd5262 # Implicit fall-through
      /wd5264 # 'const' variable is not used
    )
    
    #Generate PDB in all build configurations
    target_compile_options(${project_ref} PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/Zi>")
    add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
  else()
    #Flags for all non-MSVC compilers:
    target_compile_options(${project_ref} PRIVATE
      -Wall -Werror
    )
    
    #Disable RTTI 
    target_compile_options(${project_ref} PRIVATE -fno-rtti)
  endif()
endfunction()
