//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/parcel.hpp>
#include <hpxpi/impl/thread.hpp>

namespace hpxpi
{
    thread::thread(parcel& creator)
      : continuation_(creator),
        addr_(creator.get_target_address()),
        target_action_(creator.get_target_action()),
        environment_data_(creator.get_environment_data())
    {
        // we need to refer to our continuation data only
        continuation_.pop_frame();
    }

    thread* get_self_thread()
    {
        thread* ts = reinterpret_cast<thread*>(
            hpx::threads::get_thread_data(hpx::threads::get_self_id()));
        return ts;
    }
}
