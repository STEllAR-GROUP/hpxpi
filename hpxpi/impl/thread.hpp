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
    private:
        parcel& continuation_;
        XPI_Addr addr_;
        std::string target_action_;
        std::vector<uint8_t> environment_data_;

    public:
        thread(parcel& creator)
          : continuation_(creator),
            addr_(creator.get_target_address()),
            target_action_(creator.get_target_action()),
            environment_data_(creator.get_environment_data())
        {
            // we need to refer to our continuation data only
            continuation_.pop_frame();
        }

        void* get_environment_data()
        {
            return environment_data_.data();
        }
        void const* get_environment_data() const
        {
            return environment_data_.data();
        }

        XPI_Addr get_address() const
        {
            return addr_;
        }

        // FIXME: this needs to refer to the thread's address, not its target
        XPI_Addr get_thread_id() const
        {
            return addr_;
        }

        XPI_Parcel get_continuation() const
        {
            XPI_Parcel parcel = { reinterpret_cast<intptr_t>(&continuation_) };
            return parcel;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    thread* get_self_thread();
}
