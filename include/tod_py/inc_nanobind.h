#pragma once

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4365) // signed/unsigned mismatch
#  pragma warning(disable : 4800) // implicit conversion
#endif

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
