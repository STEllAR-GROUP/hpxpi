// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpxpi/xpi.h>
#include <hpxpi/impl/parcel.hpp>

#include <string>
#include <vector>

#include <boost/serialization/serialization.hpp>

namespace hpxpi
{
    struct thread
    {
        parcel const& continuation;
        XPI_Addr addr;
        std::string target_action;
        std::vector<unsigned char> environment_data;

        thread(parcel const& creator);
    };

    ///////////////////////////////////////////////////////////////////////////
    thread* get_self_thread();
}
