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
        parcel const& continuation_;
        XPI_Addr addr_;
        std::string target_action_;
        std::vector<uint8_t> environment_data_;

        thread(parcel const& creator);

        void* get_environment_data()
        {
            return environment_data_.data();
        }
        void const* get_environment_data() const
        {
            return environment_data_.data();
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    thread* get_self_thread();
}
