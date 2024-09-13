#include "tod_py/inc_nanobind.h"
#include "tod_py/inc_nanobind_stl.h"
#include "tod_core/inc_sdl.h"
#include "tod_core/proc_tod.h"
#include "tod_core/poli_tod.h"

void nb_repo_version(nanobind::module_&);
void nb_data_tod_py(nanobind::module_ &m);

NB_MODULE(tod_py, m)
{
    m.def("init", [](const std::string &asset_path){
        tod::poli_tod_init poli{
          .asset_main_path = asset_path
        };
        tod::proc_tod proc{};
        proc.init_tod(poli);
    });


    nb_repo_version(m);
    nb_data_tod_py(m);
}
