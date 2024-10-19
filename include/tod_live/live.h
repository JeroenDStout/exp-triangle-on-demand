#pragma once

/*
 *  ...
 */

#include "tod_core/data_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_gpu.h"
#include "tod_core/data_tod.h"
#include "tod_core/poli_tod.h"
#include "tod_core/proc_tod.h"

namespace tod_live {

    struct handler_tod_live {
        tod::proc_gpu         proc_gpu;
        tod::proc_tod         proc_tod;

        tod::data_gpu_context gpu_context;
        tod::data_tod_context tod_context;

        enum class handler_result { success, failure };
        
        handler_result prepare();
        handler_result init();
        handler_result run();
        handler_result deinit();
        handler_result cleanup();
    };

}