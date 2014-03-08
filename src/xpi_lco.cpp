//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/xpi_addr.hpp>

extern "C"
{
    XPI_Err XPI_LCO_trigger_sync(XPI_Addr lco, void const* data)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == data)
            return XPI_ERR_BAD_ARG;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_get_size_sync(XPI_Addr lco, size_t *size)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == size)
            return XPI_ERR_BAD_ARG;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_had_get_value_sync(XPI_Addr lco, bool *value)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == value)
            return XPI_ERR_BAD_ARG;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_free_sync(XPI_Addr lco)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        // create an id, taking ownership, destructor releases credits
        hpx::id_type id = hpxpi::from_address(lco);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_get_value(XPI_Addr lco, void const* data)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        return XPI_SUCCESS;
    }
}
