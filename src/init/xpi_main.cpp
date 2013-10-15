//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/hpx.hpp>
#include <hpx/hpx_finalize.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hpxpi
{
    int xpi_main_wrapper(int argc, char** argv, XPI_Err (*XPI_main_)(size_t, void**))
    {
        int result = XPI_main_(argc, reinterpret_cast<void**>(argv));
        hpx::finalize();
        return result;
    }
}
