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
    thread::thread(parcel const& creator)
      : continuation(creator),
        addr(creator.addr()),
        target_action(creator.target_action()),
        environment_data(creator.environment_data())
    {
    }

    thread* get_self_thread()
    {
        thread* ts = reinterpret_cast<thread*>(
            hpx::threads::get_thread_data(hpx::threads::get_self_id()));
        return ts;
    }
}
