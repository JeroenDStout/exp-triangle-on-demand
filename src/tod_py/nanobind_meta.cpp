#include "tod_py/inc_nanobind.h"
#include "repo_version/git_version.h"

void nb_repo_version(nanobind::module_ &m)
{
    m.def("get_version", [](){ return gaos::version::get_git_essential_version(); });
}