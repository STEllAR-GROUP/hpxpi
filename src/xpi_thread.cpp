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
    // Get own global address
    XPI_Addr XPI_Thread_get_self()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self ? self->get_thread_id() : XPI_NULL;
    }

    // Gets the environment data of instantiating parcel
    void* XPI_Thread_get_env()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self ? self->get_environment_data() : 0;
    }

    // Gets target address of instantiating parcel
    XPI_Addr XPI_Thread_get_addr()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self ? self->get_address() : XPI_NULL;
    }

    // Gets the handle for continuation parcel
    XPI_Parcel XPI_Thread_get_cont()
    {
        hpxpi::thread* self = hpxpi::get_self_thread();
        return self ? self->get_continuation() : XPI_PARCEL_NULL;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Thread_wait(XPI_Addr lco, size_t bytes, void *value)
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
        assert(bytes == b.size());
#endif

        // copy data to destination buffer
        if (0 != value)
        {
            uint8_t* data = reinterpret_cast<uint8_t*>(value);
            memcpy(data, b.data(), bytes);
        }

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Thread_wait_all(size_t n, XPI_Addr lco[], size_t bytes[],
        void* values[])
    {
        XPI_Err error = XPI_SUCCESS;
        for (int i = 0; i != n; ++i)
        {
            error = XPI_Thread_wait(lco[i], bytes[i], values[i]);
            if (error != XPI_SUCCESS)
                break;
        }
        return error;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Thread_get_process_sync(XPI_Addr address, XPI_Addr *process)
    {
//         if (XPI_NULL == address)
//             return XPI_ERR_INV_ADDR;
        if (0 == process)
            return XPI_ERR_BAD_ARG;

        // always local (for now)
        *process = hpxpi::from_id(hpx::find_here());
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    // FIXME: what is this supposed to do? Append all data blocks into one?
//     void XPI_continue(size_t n, size_t sizes[], const void * vals[])
//     {
//     }

    void XPI_continue1(size_t size, const void *val)
    {
        XPI_Parcel p = XPI_Thread_get_cont();
        XPI_Parcel_set_data(p, size, val);
    }
}
