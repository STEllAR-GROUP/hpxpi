//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/impl/thread.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    void* XPI_Thread_get_env()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self->environment_data.data();
    }
}
