//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>

using namespace std;

namespace hpxpi
{
    future::future(size_t buffer_size)
      : buffsize(buffer_size),
        hpx_promise(),
        hpx_future(hpx_promise.get_future())
    {
    }

    // Should this return a copy? How do we know how big?
    void* future::value()
    {
        return hpx_future.get().data();
    }

    void future::trigger(void* data)
    {
        unsigned char* buffer = reinterpret_cast<unsigned char*>(data);
        hpx_promise.set_value(data_type(buffer, buffer+buffsize));
    }
}

