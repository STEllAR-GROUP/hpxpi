//  Copyright (c) 2007-2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_CONFIG_MAR_09_2014_0828PM)
#define HPXPI_CONFIG_MAR_09_2014_0828PM

#include <hpx/config/defines.hpp>

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
#include <cstdlib>
using std::size_t;
using std::intptr_t;
#else
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#endif

#if defined(_MSC_VER)
#include <hpx/util/numerics/int128.hpp>
#include <hpx/util/numerics/uint128.hpp>
#endif

#endif


