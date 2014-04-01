//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/addr.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_global_malloc_sync(XPI_Addr process,
        size_t count, size_t size, XPI_Distribution distribution,
        XPI_Addr* address)
    {
        if (0 == address)
            return XPI_ERR_BAD_ARG;
        if (XPI_NULL == process || XPI_LOCAL == distribution)
            process = hpxpi::from_id(hpx::find_here());

        using hpx::components::stubs::memory;
        hpx::id_type mem_id = memory::allocate_sync(
            hpxpi::get_id(process), count*size);

        *address = hpxpi::from_id(mem_id);

        return XPI_SUCCESS;
    }

    XPI_Addr XPI_Process_global_malloc_helper(XPI_Addr process,
        size_t count, size_t size, XPI_Distribution distribution)
    {
        XPI_Addr address = XPI_NULL;
        XPI_Err error = XPI_Process_global_malloc_sync(process, count, size,
            distribution, &address);
        if (error != XPI_SUCCESS)
        {
            HPX_THROW_EXCEPTION(hpx::last_error - error,
                "XPI_Process_global_malloc_helper",
                "XPI_Process_global_malloc_sync failed");
        }
        return address;
    }

    XPI_Err XPI_Process_global_malloc(XPI_Addr process,
        size_t count, size_t size, XPI_Distribution distribution,
        XPI_Addr future)
    {
        hpx::future<XPI_Addr> f = hpx::async(
            &XPI_Process_global_malloc_helper, process, count, size,
            distribution);

        hpxpi::propagate(std::move(f), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_PROCESS_GLOBAL_MALLOC_ACTION(XPI_Global_Malloc_Descriptor* desc)
    {
        if (0 == desc)
            return XPI_ERR_BAD_ARG;

        XPI_Addr process = XPI_NULL;
        XPI_Err error = XPI_Thread_get_process_sync(
            XPI_Thread_get_self(), &process);
        if (error != XPI_SUCCESS) return error;

        XPI_Addr address = XPI_NULL;
        error = XPI_Process_global_malloc_sync(process, desc->count,
            desc->size, desc->distribution, &address);
        if (error != XPI_SUCCESS) return error;

        XPI_continue(sizeof(XPI_Addr), &address);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_global_free_sync(XPI_Addr process,
        XPI_Addr address)
    {
        // create an id, taking ownership, destructor releases credits
        if (XPI_NULL != address)
            hpxpi::release_id(address);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Process_global_free(XPI_Addr process,
        XPI_Addr address, XPI_Addr future)
    {
        return XPI_Process_global_free_sync(process, address);
    }

    XPI_Err XPI_PROCESS_GLOBAL_FREE_ACTION(XPI_Addr *address)
    {
        if (0 == address)
            return XPI_ERR_BAD_ARG;

        // create an id, taking ownership, destructor releases credits
        if (XPI_NULL != *address)
            hpxpi::release_id(*address);

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_lco_malloc_sync(XPI_Addr process,
        size_t count, size_t size, XPI_LCO_Descriptor handlers,
        XPI_Distribution distribution, size_t init_data_size,
        void const* const init_data, XPI_Addr *address)
    {
        if (0 == address || 0 == count)
            return XPI_ERR_BAD_ARG;

        // FIXME: How to send the XPI_LCO_Descriptor over the wire?
        if (XPI_NULL != process && process != hpxpi::from_id(hpx::find_here()))
            return XPI_ERR_INV_ADDR;

        for (size_t i = 0; i != count; ++i)
        {
            hpxpi::custom_lco* lco = static_cast<hpxpi::custom_lco*>(
                hpxpi::custom_lco::create());

            lco->get()->init(handlers, size, init_data_size, init_data);

            hpx::naming::gid_type gid = lco->get_base_gid();
            if (!gid)
            {
                hpxpi::custom_lco::destroy(lco);
                return XPI_ERR_NO_MEM;
            }

            // everything is ok, return the new id
            address[i] = hpxpi::from_id(gid);
        }

        return XPI_SUCCESS;
    }

    XPI_Addr XPI_Process_lco_malloc_helper(XPI_Addr process,
        boost::shared_ptr<XPI_Process_LCO_Malloc_Descriptor> data)
    {
        XPI_Addr address = XPI_NULL;
        XPI_Err error = XPI_Process_lco_malloc_sync(process, data->count,
            data->size, data->handlers, data->distribution,
            data->init_data_size, data->init_data, &address);
        if (error != XPI_SUCCESS)
        {
            HPX_THROW_EXCEPTION(hpx::last_error - error,
                "XPI_Process_lco_malloc_helper",
                "XPI_Process_lco_malloc_sync failed");
        }
        return address;
    }

    XPI_Err XPI_Process_lco_malloc(XPI_Addr process,
        size_t count, size_t size, XPI_LCO_Descriptor handlers,
        XPI_Distribution distribution, size_t init_data_size,
        const void * const init_data, XPI_Addr future)
    {
        XPI_Process_LCO_Malloc_Descriptor data =
        {
            count, size, handlers,
            distribution, init_data_size, init_data
        };

        boost::shared_ptr<XPI_Process_LCO_Malloc_Descriptor> d(
            boost::make_shared<XPI_Process_LCO_Malloc_Descriptor>(data));

        hpx::future<XPI_Addr> f =
            hpx::async(&XPI_Process_lco_malloc_helper, process, d);

        hpxpi::propagate(std::move(f), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_PROCESS_LCO_MALLOC_ACTION(XPI_Process_LCO_Malloc_Descriptor* desc)
    {
        if (0 == desc)
            return XPI_ERR_BAD_ARG;

        XPI_Addr process = XPI_NULL;
        XPI_Err error = XPI_Thread_get_process_sync(
            XPI_Thread_get_self(), &process);
        if (error != XPI_SUCCESS) return error;

        XPI_Addr address = XPI_NULL;
        error = XPI_Process_lco_malloc_sync(process, desc->count, desc->size,
            desc->handlers, desc->distribution, desc->init_data_size,
            desc->init_data, &address);
        if (error != XPI_SUCCESS) return error;

        XPI_continue(sizeof(XPI_Addr), &address);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_future_new_sync(XPI_Addr process,
        size_t count, size_t bytes, XPI_Distribution distribution,
        XPI_Addr *address)
    {
        XPI_LCO_Descriptor handlers = (0 != bytes) ?
            hpxpi::detail::lco_descriptor<hpxpi::future>::get_handlers() :
            hpxpi::detail::lco_descriptor<hpxpi::trigger>::get_handlers();

        return XPI_Process_lco_malloc_sync(process, count, bytes, handlers,
            distribution, 0, 0, address);
    }

    XPI_Addr XPI_Process_future_new_helper(XPI_Addr process,
        size_t count, size_t bytes, XPI_Distribution distribution)
    {
        XPI_Addr address = XPI_NULL;
        XPI_Err error = XPI_Process_future_new_sync(process, count, bytes,
            distribution, &address);
        if (error != XPI_SUCCESS)
        {
            HPX_THROW_EXCEPTION(hpx::last_error - error,
                "XPI_Process_future_new_helper",
                "XPI_Process_future_new_sync failed");
        }
        return address;
    }

    XPI_Err XPI_Process_future_new(XPI_Addr process,
        size_t count, size_t bytes, XPI_Distribution distribution,
        XPI_Addr future)
    {
        hpx::future<XPI_Addr> f = hpx::async(
            &XPI_Process_future_new_helper, process, count, bytes,
            distribution);

        hpxpi::propagate(std::move(f), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_PROCESS_FUTURE_NEW_ACTION(
        XPI_Process_Future_New_Descriptor* desc)
    {
        if (0 == desc)
            return XPI_ERR_BAD_ARG;

        XPI_Addr process = XPI_NULL;
        XPI_Err error = XPI_Thread_get_process_sync(
            XPI_Thread_get_self(), &process);
        if (error != XPI_SUCCESS) return error;

        XPI_Addr address = XPI_NULL;
        error = XPI_Process_future_new_sync(process, desc->count,
            desc->bytes, desc->distribution, &address);
        if (error != XPI_SUCCESS) return error;

        XPI_continue(sizeof(XPI_Addr), &address);
        return XPI_SUCCESS;
    }
}
