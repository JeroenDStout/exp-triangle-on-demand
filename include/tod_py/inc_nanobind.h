#pragma once

#include "tod_core/inc_eigen.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4365) // signed/unsigned mismatch
#  pragma warning(disable : 4800) // implicit conversion
#  pragma warning(disable : 5054) // operator '==': deprecated between enumerations of different types
#endif

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
