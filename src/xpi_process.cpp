//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/xpi_addr.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_global_malloc_sync(XPI_Addr process,
        size_t count, size_t size, XPI_Distribution distribution,
        XPI_Addr* address)
    {
        if (0 == address)
            return XPI_ERR_BAD_ARG;
        if (XPI_NULL == process)
            process = hpxpi::from_id(hpx::find_here());

        using hpx::components::stubs::memory;
        hpx::id_type mem_id = memory::allocate_sync(
            hpxpi::get_id(process), count*size);

        *address = hpxpi::from_id(mem_id);

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_global_free_sync(XPI_Addr process,
        XPI_Addr address)
    {
        if (XPI_NULL == address)
            return XPI_ERR_INV_ADDR;

        // create an id, taking ownership, destructor releases credits
        hpx::id_type id = hpxpi::from_address(address);

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_lco_malloc_sync(XPI_Addr process,
        size_t count, size_t size, XPI_LCO_Descriptor handlers,
        XPI_Distribution distribution, size_t init_data_size,
        const void * const init_data, XPI_Addr *address)
    {
        if (0 == address)
            return XPI_ERR_BAD_ARG;

        // FIXME: How to send the XPI_LCO_Descriptor over the wire?
        if (XPI_NULL != process)
            return XPI_ERR_INV_ADDR;

        hpxpi::custom_lco* lco = static_cast<hpxpi::custom_lco*>(
            hpxpi::custom_lco::create());

        lco->get()->init(handlers, init_data_size, init_data);

        hpx::naming::gid_type gid = lco->get_base_gid();
        if (!gid)
        {
            hpxpi::custom_lco::destroy(lco);
            return XPI_ERR_NO_MEM;
        }

        // everything is ok, return the new id
        *address = hpxpi::from_id(gid);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Process_future_new_sync(XPI_Addr process,
        size_t count, size_t bytes, XPI_Distribution distribution,
        XPI_Addr *address)
    {
        if (0 == address)
            return XPI_ERR_BAD_ARG;
        if (XPI_NULL == process)
            process = hpxpi::from_id(hpx::find_here());

        return XPI_SUCCESS;
    }
}
