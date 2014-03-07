// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpx/exception.hpp>
#include <hpx/include/lcos.hpp>

#include <boost/serialization/serialization.hpp>

#include <hpxpi/xpi.h>

#include <vector>

namespace hpxpi
{
    struct future
    {
    private:
        typedef std::vector<unsigned char> data_type;

        size_t buffsize;
        hpx::lcos::promise<data_type> hpx_promise;
        hpx::unique_future<data_type> hpx_future;

    public:
        future(size_t buffer_size);

        void* value();
        void trigger(void* data);
        size_t size() const { return buffsize; }
        bool had_get() const { return hpx_future.valid(); }
    };

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T>
        void on_ready(hpx::unique_future<T> f, XPI_Addr cont)
        {
            hpx::error_code ec;
            T val = f.get(ec);
            if (!ec) {
                // Propagate value to given continuation.
                XPI_LCO_trigger_sync(cont, &val);
            }
            else {
                // FIXME: What should we do in this case? XPI does not have a
                //        way to propagate an error to the waiting LCO.
            }
        }

        void on_ready(hpx::unique_future<void> f, XPI_Addr cont);
    }

    template <typename T>
    void propagate(hpx::unique_future<T> f, XPI_Addr cont)
    {
        using hpx::util::placeholders::_1;
        f.then(hpx::util::bind(&detail::on_ready<T>, _1, cont));
    }

    inline void propagate(hpx::unique_future<void> f, XPI_Addr cont)
    {
        using hpx::util::placeholders::_1;
        f.then(hpx::util::bind(&detail::on_ready, _1, cont));
    }
}

/*
extern "C"{

//Spec doesn't have void* argument required for action type?
XPI_Err XPI_LCO_GET_VALUE(void* nothing);

//How do we know how big the data is?
XPI_Err XPI_LCO_TRIGGER_ACTION(void* data);

//Spec doesn't have void* argument required for action type?
XPI_Err XPI_LCO_GET_SIZE_ACTION(void* nothing);

//Spec doesn't have void* argument required for action type?
XPI_Err XPI_LCO_HAD_GET_VALUE_ACTION(void* nothing);
}
*/
