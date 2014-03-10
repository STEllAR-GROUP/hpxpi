//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/process.hpp>
#include <hpxpi/impl/parcel.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    void register_process_actions()
    {
        HPXPI_REGISTER_DIRECT_ACTION(XPI_PROCESS_GLOBAL_MALLOC_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_PROCESS_GLOBAL_FREE_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_PROCESS_LCO_MALLOC_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_PROCESS_FUTURE_NEW_ACTION);
    }
}

