#pragma once

#include "tod_core/inc_eigen.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4365) // signed/unsigned mismatch
#  pragma warning(disable : 4371) // layout of class may have changed from a previous version of the compiler due to better packing of member
#  pragma warning(disable : 4800) // implicit conversion
#  pragma warning(disable : 5027) // move assignment operator was implicitly defined as deleted
#  pragma warning(disable : 5054) // operator '==': deprecated between enumerations of different types
#endif

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
