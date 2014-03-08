//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/thread.hpp>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/xpi_addr.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    void* XPI_Thread_get_env()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self->get_environment_data();
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Thread_wait(XPI_Addr lco, void *value)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;

        hpxpi::detail::custom_lco::get_value_action act;
        buffer_type b = act(hpxpi::get_id(lco));

#if !defined(NDEBUG)
        size_t size = 0;
        XPI_Err error = XPI_LCO_get_size_sync(lco, &size);
        if (error != XPI_SUCCESS)
            return error;
        assert(size >= b.size());
#endif

        // copy data to destination buffer
        if (0 != value)
        {
            uint8_t* data = reinterpret_cast<uint8_t*>(value);
            std::memcpy(data, b.data(), b.size());
        }

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Thread_wait_all(size_t n, XPI_Addr lco[], void* values[])
    {
        XPI_Err error = XPI_SUCCESS;
        for (int i = 0; i != n; ++i)
        {
            error = XPI_Thread_wait(lco[i], values[i]);
            if (error != XPI_SUCCESS)
                break;
        }
        return error;
    }
}
