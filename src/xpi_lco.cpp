//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/thread.hpp>
#include <hpxpi/impl/parcel.hpp>
#include <hpxpi/impl/addr.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_LCO_trigger(XPI_Addr lco, void const* data, XPI_Addr cont)
    {
        if (XPI_NULL != lco)
        {
            if (0 == data)
            {
                if (XPI_NULL == cont)
                    hpxpi::trigger_lco_event(lco);
                else
                    hpxpi::trigger_lco_event(lco, cont);
            }
            else
            {
                size_t size = 0;
                XPI_Err error = XPI_LCO_get_size_sync(lco, &size);
                if (error != XPI_SUCCESS)
                    return error;

                if (XPI_NULL == cont)
                    hpxpi::set_lco_value(lco, size, data);
                else
                    hpxpi::set_lco_value(lco, size, data, cont);
            }
        }
        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_trigger_sync(XPI_Addr lco, void const* data)
    {
        return XPI_LCO_trigger(lco, data, XPI_NULL);
    }

    // XPI_LCO_TRIGGER_ACTION(value)
    //  IN value (optional) an optional trigger value
    XPI_Err XPI_LCO_TRIGGER_ACTION(void* args)
    {
        XPI_Addr lco = XPI_Thread_get_addr();
        if (XPI_NULL != lco)
        {
            if (0 == args)
            {
                hpxpi::trigger_lco_event(lco);
            }
            else
            {
                size_t size = 0;
                XPI_Err error = XPI_LCO_get_size_sync(lco, &size);
                if (error != XPI_SUCCESS)
                    return error;

                hpxpi::set_lco_value(lco, size, args);
            }
        }
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_LCO_get_size(XPI_Addr lco, XPI_Addr cont)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        hpxpi::detail::custom_lco::get_size_action act;
        if (XPI_NULL == cont)
            hpx::apply(act, hpxpi::get_id(lco));
        else
            hpx::apply_continue(act, hpxpi::get_id(lco), hpxpi::get_id(cont));

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

    // XPI_LCO_GET_SIZE_ACTION() CONTINUE(size)
    //  CONT size the size of the user-portion of the LCO
    XPI_Err XPI_LCO_GET_SIZE_ACTION(void* args)
    {
        size_t size = 0;
        XPI_Err error = XPI_LCO_get_size_sync(XPI_Thread_get_addr(), &size);
        if (error != XPI_SUCCESS)
            return error;

        XPI_continue(sizeof(size), &size);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_LCO_had_get_value(XPI_Addr lco, XPI_Addr cont)
    {
        if (XPI_NULL == lco)
            return XPI_ERR_INV_ADDR;

        hpxpi::detail::custom_lco::had_get_value_action act;
        if (XPI_NULL == cont)
            hpx::apply(act, hpxpi::get_id(lco));
        else
            hpx::apply_continue(act, hpxpi::get_id(lco), hpxpi::get_id(cont));

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

    // XPI_LCO_HAD_GET_VALUE_ACTION CONTINUE(value)
    //   CONT value true if any threads have performed XPI_LCO_GET_VALUE on the
    //        target LCO
    XPI_Err XPI_LCO_HAD_GET_VALUE_ACTION(void* args)
    {
        bool value = false;

        XPI_Err error = XPI_LCO_had_get_value_sync(XPI_Thread_get_addr(), &value);
        if (error != XPI_SUCCESS)
            return error;

        XPI_continue(sizeof(value), &value);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_LCO_free(XPI_Addr lco, XPI_Addr future)
    {
        // create an id, taking ownership, destructor releases credits
        if (XPI_NULL != lco)
            hpxpi::release_id(lco);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_LCO_free_sync(XPI_Addr lco)
    {
        return XPI_LCO_free(lco, XPI_NULL);
    }

    // XPI_LCO_FREE_ACTION
    XPI_Err XPI_LCO_FREE_ACTION(void* args)
    {
        return XPI_LCO_free_sync(XPI_Thread_get_addr());
    }

    ///////////////////////////////////////////////////////////////////////////
    // XPI_LCO_GET_VALUE_ACTION CONTINUE(value)
    //   CONT value the LCO value: CONT(void *data)
    // FIXME: make wait-free
    XPI_Err XPI_LCO_GET_VALUE_ACTION(void* args)
    {
        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;

        XPI_Addr lco = XPI_Thread_get_addr();

        hpxpi::detail::custom_lco::get_value_action act;
        buffer_type b = act(hpxpi::get_id(lco));

        XPI_continue(b.size(), b.data());
        return XPI_SUCCESS;
    }
}
