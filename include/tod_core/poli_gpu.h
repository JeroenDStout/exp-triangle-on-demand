#pragma once

/*
 *  ...
 */
 
#include <string>

namespace tod {

    struct poli_gpu_context {
        
        bool        debug_mode    = true;
        std::string name          = "";

        bool        create_window = true;
        std::string window_name   = "";
        int         window_w      = 640,
                    window_h      = 480;

    };

    struct poli_gpu_shader {
    };

}