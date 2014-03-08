//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/xpi_addr.hpp>

extern "C"
{
    XPI_Err XPI_LCO_trigger_sync(XPI_Addr lco, void const* data)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == data)
            return XPI_ERR_BAD_ARG;

        size_t size = 0;
        XPI_Err error = XPI_LCO_get_size_sync(lco, &size);
        if (error != XPI_SUCCESS)
            return error;

        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;
        uint8_t const* d = reinterpret_cast<uint8_t const*>(data);

        hpxpi::detail::custom_lco::set_value_action act;
        act(hpxpi::get_id(lco), buffer_type(d, size, buffer_type::copy));
        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_get_size_sync(XPI_Addr lco, size_t *size)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == size)
            return XPI_ERR_BAD_ARG;

        hpxpi::detail::custom_lco::get_size_action act;
        *size = act(hpxpi::get_id(lco));

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_had_get_value_sync(XPI_Addr lco, bool *value)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;
        if (0 == value)
            return XPI_ERR_BAD_ARG;

        hpxpi::detail::custom_lco::had_get_value_action act;
        *value = act(hpxpi::get_id(lco));

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_free_sync(XPI_Addr lco)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        // create an id, taking ownership, destructor releases credits
        hpxpi::release_id(lco);

        return XPI_SUCCESS;
    }
}
