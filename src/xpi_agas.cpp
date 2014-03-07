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
    XPI_Err XPI_Agas_store_u8_sync(XPI_Addr addr, uint8_t val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        memory::store8_sync(hpxpi::get_id(addr), val, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u16_sync(XPI_Addr addr, uint16_t val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        memory::store16_sync(hpxpi::get_id(addr), val, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u32_sync(XPI_Addr addr, uint32_t val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        memory::store32_sync(hpxpi::get_id(addr), val, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u64_sync(XPI_Addr addr, uint64_t val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        memory::store64_sync(hpxpi::get_id(addr), val, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    // XPI_Err XPI_Agas_store_u128_sync(XPI_Addr addr, __uint128_t val);

    XPI_Err XPI_Agas_store_s8_sync(XPI_Addr addr, int8_t val)
    {
        return XPI_Agas_store_u8_sync(addr, val);
    }

    XPI_Err XPI_Agas_store_s16_sync(XPI_Addr addr, int16_t val)
    {
        return XPI_Agas_store_u16_sync(addr, val);
    }

    XPI_Err XPI_Agas_store_s32_sync(XPI_Addr addr, int32_t val)
    {
        return XPI_Agas_store_u32_sync(addr, val);
    }

    XPI_Err XPI_Agas_store_s64_sync(XPI_Addr addr, int64_t val)
    {
        return XPI_Agas_store_u64_sync(addr, val);
    }

    // XPI_Err XPI_Agas_store_s128_sync(XPI_Addr addr, __int128_t val);

    XPI_Err XPI_Agas_store_f_sync(XPI_Addr addr, float val)
    {
        return XPI_Agas_store_u32_sync(addr, *reinterpret_cast<uint32_t*>(&val));
    }

    XPI_Err XPI_Agas_store_d_sync(XPI_Addr addr, double val)
    {
        return XPI_Agas_store_u64_sync(addr, *reinterpret_cast<uint64_t*>(&val));
    }

    // XPI_Err XPI_Agas_store_fc_sync(XPI_Addr addr, float _Complex val);
    // XPI_Err XPI_Agas_store_dc_sync(XPI_Addr addr, double _Complex val);

    XPI_Err XPI_Agas_store_addr_sync(XPI_Addr addr, XPI_Addr val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::components::server::memory::uint128_t u128(val.msb, val.lsb);
        hpx::error_code ec;
        memory::store128_sync(hpxpi::get_id(addr), u128, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_addrdiff_sync(XPI_Addr addr, XPI_AddrDiff val)
    {
        return XPI_Agas_store_addr_sync(addr, *reinterpret_cast<XPI_Addr*>(&val));
    }
}
