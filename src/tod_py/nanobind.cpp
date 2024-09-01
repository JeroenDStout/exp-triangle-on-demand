#include "tod_py/inc_nanobind.h"

void nb_repo_version(nanobind::module_&);

NB_MODULE(tod_py, m)
{
    nb_repo_version(m);
}