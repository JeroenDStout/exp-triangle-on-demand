// Test test test

#include <iostream>
#include <random>

#include "repo_version/git_version.h"
#include "tod_live/live.h"

int main(int, char*[])
{
    std::cout
      << std::endl
      << "          * * * * * * * ** * * * * * * " << std::endl
      << "         * * * Triangle on Demand * * * " << std::endl
      << "          * * * * * * * ** * * * * * * " << std::endl
      << std::endl
      << gaos::version::get_git_essential_version() << std::endl
      << gaos::version::get_compile_stamp() << std::endl
      << std::endl
      << gaos::version::get_git_history() << std::endl
      << std::endl;

    tod_live::handler_tod_live handler{};
    
    if (handler.prepare() != tod_live::handler_tod_live::handler_result::success)
    { std::cout << "ERROR: Error prepare'ing ToD Live" << std::endl; return -1; }
    if (handler.init()    != tod_live::handler_tod_live::handler_result::success)
    { std::cout << "ERROR: Error init'ing ToD Live"    << std::endl; return -1; }
    if (handler.run() != tod_live::handler_tod_live::handler_result::success)
    { std::cout << "ERROR: Error run'ing ToD Live"     << std::endl; return -1; }
    if (handler.deinit()  != tod_live::handler_tod_live::handler_result::success)
    { std::cout << "ERROR: Error deinit'ing ToD Live"  << std::endl; return -1; }
    if (handler.cleanup() != tod_live::handler_tod_live::handler_result::success)
    { std::cout << "ERROR: Error cleanup'ing ToD Live" << std::endl; return -1; }

    return 0;
}
