#include "tod_py/inc_nanobind.h"
#include "tod_core/inc_sdl.h"

void nb_repo_version(nanobind::module_&);
void nb_data_tod_py(nanobind::module_ &m);

NB_MODULE(tod_py, m)
{
    SDL_Init(SDL_INIT_VIDEO);

    nb_repo_version(m);
    nb_data_tod_py(m);
}
