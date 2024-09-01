#pragma once

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4365) // signed/unsigned mismatch
#  pragma warning(disable : 4464) // include path contains '..'
#  pragma warning(disable : 4619) // TODO: The warning below sometimes does not exist??
#  pragma warning(disable : 5267) // definition of implicit copy constructor is deprecated
#  pragma warning(disable : 5027) // move assignment operator was implicitly defined as deleted
#  pragma warning(disable : 5031) // #pragma warning(pop): likely mismatch, popping warning state pushed in different file
#endif

#include <eigen/dense>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif