//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/runtime/components/stubs/memory.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/addr.hpp>
#include <hpxpi/impl/lco.hpp>

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    XPI_Addr XPI_NULL = { 0, 0 };
    XPI_Action XPI_ACTION_NULL = NULL;

    XPI_Err XPI_Addr_init(uint64_t address, XPI_Addr *result)
    {
        if (0 == result)
            return XPI_ERR_BAD_ARG;

        *result = hpxpi::from_id(hpx::find_here());
        result->lsb = address;

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Native Parcel Interface [5.2]
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_AGAS_STORE_U8_ACTION(uint8_t* val)
    {
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        memory::store8_async(hpxpi::get_id(addr), *val);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_STORE_U16_ACTION(uint16_t* val)
    {
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        memory::store16_async(hpxpi::get_id(addr), *val);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_STORE_U32_ACTION(uint32_t* val)
    {
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        memory::store32_async(hpxpi::get_id(addr), *val);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_STORE_U64_ACTION(uint64_t* val)
    {
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        memory::store64_async(hpxpi::get_id(addr), *val);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_STORE_U128_ACTION(__uint128_t* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        hpx::components::server::memory::uint128_t* val =
            reinterpret_cast<hpx::components::server::memory::uint128_t*>(args);
        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        memory::store128_async(hpxpi::get_id(addr), *val);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_STORE_S8_ACTION(int8_t* args)
    {
        return XPI_AGAS_STORE_U8_ACTION((uint8_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_S16_ACTION(int16_t* args)
    {
        return XPI_AGAS_STORE_U16_ACTION((uint16_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_S32_ACTION(int32_t* args)
    {
        return XPI_AGAS_STORE_U32_ACTION((uint32_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_S64_ACTION(int64_t* args)
    {
        return XPI_AGAS_STORE_U64_ACTION((uint64_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_S128_ACTION(__int128_t* args)
    {
        return XPI_AGAS_STORE_U128_ACTION((__uint128_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_F_ACTION(float* args)
    {
        return XPI_AGAS_STORE_U32_ACTION((uint32_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_D_ACTION(double* args)
    {
        return XPI_AGAS_STORE_U64_ACTION((uint64_t*)args);
    }

//     XPI_Err XPI_AGAS_STORE_FC_ACTION(void* args);
//     XPI_Err XPI_AGAS_STORE_DC_ACTION(void* args);

    XPI_Err XPI_AGAS_STORE_ADDR_ACTION(XPI_Addr* args)
    {
        return XPI_AGAS_STORE_U128_ACTION((__uint128_t*)args);
    }

    XPI_Err XPI_AGAS_STORE_ADDRDIFF_ACTION(XPI_AddrDiff* args)
    {
        return XPI_AGAS_STORE_U128_ACTION((__uint128_t*)args);
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_AGAS_LOAD_U8_ACTION(void* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        uint8_t val = memory::load8_sync(hpxpi::get_id(addr));

        XPI_continue1(sizeof(uint8_t), &val);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_LOAD_U16_ACTION(void* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        uint16_t val = memory::load16_sync(hpxpi::get_id(addr));

        XPI_continue1(sizeof(uint16_t), &val);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_LOAD_U32_ACTION(void* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        uint32_t val = memory::load32_sync(hpxpi::get_id(addr));

        XPI_continue1(sizeof(uint32_t), &val);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_LOAD_U64_ACTION(void* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        uint64_t val = memory::load64_sync(hpxpi::get_id(addr));

        XPI_continue1(sizeof(uint64_t), &val);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_LOAD_U128_ACTION(void* args)
    {
        if (0 == args)
            return XPI_ERR_BAD_ARG;

        XPI_Addr addr = XPI_Thread_get_addr();

        using hpx::components::stubs::memory;
        hpx::components::server::memory::uint128_t val =
            memory::load128_sync(hpxpi::get_id(addr));

        XPI_continue1(sizeof(hpx::components::server::memory::uint128_t), &val);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_AGAS_LOAD_S8_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U8_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_S16_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U16_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_S32_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U32_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_S64_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U64_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_S128_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U128_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_F_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U32_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_D_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U64_ACTION(args);
    }

//     XPI_Err XPI_AGAS_LOAD_FC_ACTION(void* args);
//     XPI_Err XPI_AGAS_LOAD_DC_ACTION(void* args);

    XPI_Err XPI_AGAS_LOAD_ADDR_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U128_ACTION(args);
    }

    XPI_Err XPI_AGAS_LOAD_ADDRDIFF_ACTION(void* args)
    {
        return XPI_AGAS_LOAD_U128_ACTION(args);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Asynchronous memory access [5.3.1]
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Agas_store_u8(XPI_Addr addr, uint8_t val, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::store8(hpxpi::get_id(addr), val), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u16(XPI_Addr addr, uint16_t val, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::store16(hpxpi::get_id(addr), val), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u32(XPI_Addr addr, uint32_t val, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::store32(hpxpi::get_id(addr), val), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u64(XPI_Addr addr, uint64_t val, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::store64(hpxpi::get_id(addr), val), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_u128(XPI_Addr addr, __uint128_t val, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::store128(hpxpi::get_id(addr), val), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_store_s8(XPI_Addr addr, int8_t val, XPI_Addr future)
    {
        return XPI_Agas_store_u8(addr, val, future);
    }

    XPI_Err XPI_Agas_store_s16(XPI_Addr addr, int16_t val, XPI_Addr future)
    {
        return XPI_Agas_store_u16(addr, val, future);
    }

    XPI_Err XPI_Agas_store_s32(XPI_Addr addr, int32_t val, XPI_Addr future)
    {
        return XPI_Agas_store_u32(addr, val, future);
    }

    XPI_Err XPI_Agas_store_s64(XPI_Addr addr, int64_t val, XPI_Addr future)
    {
        return XPI_Agas_store_u64(addr, val, future);
    }

    XPI_Err XPI_Agas_store_s128(XPI_Addr addr, __int128_t val, XPI_Addr future)
    {
        return XPI_Agas_store_u128(addr,
            *reinterpret_cast<__uint128_t*>(&val), future);
    }

    XPI_Err XPI_Agas_store_f(XPI_Addr addr, float val, XPI_Addr future)
    {
        return XPI_Agas_store_u32(addr, *reinterpret_cast<uint32_t*>(&val), future);
    }

    XPI_Err XPI_Agas_store_d(XPI_Addr addr, double val, XPI_Addr future)
    {
        return XPI_Agas_store_u64(addr, *reinterpret_cast<uint64_t*>(&val), future);
    }

    // XPI_Err XPI_Agas_store_fc(XPI_Addr addr, float _Complex val, XPI_Addr future);
    // XPI_Err XPI_Agas_store_dc(XPI_Addr addr, double _Complex val, XPI_Addr future);

    XPI_Err XPI_Agas_store_addr(XPI_Addr addr, XPI_Addr val, XPI_Addr future)
    {
        return XPI_Agas_store_u128(addr,
            *reinterpret_cast<__uint128_t*>(&val), future);
    }

    XPI_Err XPI_Agas_store_addrdiff(XPI_Addr addr, XPI_AddrDiff val,
        XPI_Addr future)
    {
        return XPI_Agas_store_u128(addr,
            *reinterpret_cast<__uint128_t*>(&val), future);
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Agas_load_u8(XPI_Addr addr, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::load8(hpxpi::get_id(addr)), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u16(XPI_Addr addr, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::load16(hpxpi::get_id(addr)), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u32(XPI_Addr addr, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::load32(hpxpi::get_id(addr)), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u64(XPI_Addr addr, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::load64(hpxpi::get_id(addr)), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u128(XPI_Addr addr, XPI_Addr future)
    {
        if (XPI_NULL == addr || XPI_NULL == future)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;
        hpxpi::propagate(memory::load128(hpxpi::get_id(addr)), future);

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_s8(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u8(addr, future);
    }

    XPI_Err XPI_Agas_load_s16(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u16(addr, future);
    }

    XPI_Err XPI_Agas_load_s32(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u32(addr, future);
    }

    XPI_Err XPI_Agas_load_s64(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u64(addr, future);
    }

    XPI_Err XPI_Agas_load_s128(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u128(addr, future);
    }

    XPI_Err XPI_Agas_load_f(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u32(addr, future);
    }

    XPI_Err XPI_Agas_load_d(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u64(addr, future);
    }

    // XPI_Err XPI_Agas_load_fc(XPI_Addr addr, XPI_Addr future);
    // XPI_Err XPI_Agas_load_dc(XPI_Addr addr, XPI_Addr future);

    XPI_Err XPI_Agas_load_addr(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u128(addr, future);
    }

    XPI_Err XPI_Agas_load_addrdiff(XPI_Addr addr, XPI_Addr future)
    {
        return XPI_Agas_load_u128(addr, future);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Synchronous memory access [5.3.2]
    ///////////////////////////////////////////////////////////////////////////
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

    XPI_Err XPI_Agas_store_u128_sync(XPI_Addr addr, __uint128_t val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        memory::store128_sync(hpxpi::get_id(addr), val, ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

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

    XPI_Err XPI_Agas_store_s128_sync(XPI_Addr addr, __int128_t val)
    {
        return XPI_Agas_store_u128_sync(addr,
            *reinterpret_cast<__uint128_t*>(&val));
    }

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
        return XPI_Agas_store_u128_sync(addr,
            *reinterpret_cast<__uint128_t*>(&val));
    }

    XPI_Err XPI_Agas_store_addrdiff_sync(XPI_Addr addr, XPI_AddrDiff val)
    {
        return XPI_Agas_store_u128_sync(addr,
            *reinterpret_cast<__uint128_t*>(&val));
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Agas_load_u8_sync(XPI_Addr addr, uint8_t *val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        *val = memory::load8_sync(hpxpi::get_id(addr), ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u16_sync(XPI_Addr addr, uint16_t *val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        *val = memory::load16_sync(hpxpi::get_id(addr), ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u32_sync(XPI_Addr addr, uint32_t *val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        *val = memory::load32_sync(hpxpi::get_id(addr), ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u64_sync(XPI_Addr addr, uint64_t *val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        *val = memory::load64_sync(hpxpi::get_id(addr), ec);
        if (ec)
            return XPI_ERR_ERROR;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_u128_sync(XPI_Addr addr, __uint128_t *val)
    {
        if (XPI_NULL == addr)
            return XPI_ERR_INV_ADDR;
        if (0 == val)
            return XPI_ERR_BAD_ARG;

        using hpx::components::stubs::memory;

        hpx::error_code ec;
        *val = memory::load128_sync(hpxpi::get_id(addr), ec);
        if (ec)
            return XPI_ERR_ERROR;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Agas_load_s8_sync(XPI_Addr addr, int8_t *val)
    {
        return XPI_Agas_load_u8_sync(addr, reinterpret_cast<uint8_t*>(val));
    }

    XPI_Err XPI_Agas_load_s16_sync(XPI_Addr addr, int16_t *val)
    {
        return XPI_Agas_load_u16_sync(addr, reinterpret_cast<uint16_t*>(val));
    }

    XPI_Err XPI_Agas_load_s32_sync(XPI_Addr addr, int32_t *val)
    {
        return XPI_Agas_load_u32_sync(addr, reinterpret_cast<uint32_t*>(val));
    }

    XPI_Err XPI_Agas_load_s64_sync(XPI_Addr addr, int64_t *val)
    {
        return XPI_Agas_load_u64_sync(addr, reinterpret_cast<uint64_t*>(val));
    }

    XPI_Err XPI_Agas_load_s128_sync(XPI_Addr addr, __int128_t *val)
    {
        return XPI_Agas_load_u128_sync(addr,
            reinterpret_cast<__uint128_t*>(val));
    }

    XPI_Err XPI_Agas_load_f_sync(XPI_Addr addr, float *val)
    {
        return XPI_Agas_load_u32_sync(addr, reinterpret_cast<uint32_t*>(val));
    }

    XPI_Err XPI_Agas_load_d_sync(XPI_Addr addr, double *val)
    {
        return XPI_Agas_load_u64_sync(addr, reinterpret_cast<uint64_t*>(val));
    }

    // XPI_Err XPI_Agas_load_fc_sync(XPI_Addr addr, float _Complex *val);
    // XPI_Err XPI_Agas_load_dc_sync(XPI_Addr addr, double _Complex *val);

    XPI_Err XPI_Agas_load_addr_sync(XPI_Addr addr, XPI_Addr *val)
    {
        return XPI_Agas_load_u128_sync(addr,
            reinterpret_cast<__uint128_t*>(val));
    }

    XPI_Err XPI_Agas_load_addrdiff_sync(XPI_Addr addr, XPI_AddrDiff *val)
    {
        return XPI_Agas_load_u128_sync(addr,
            reinterpret_cast<__uint128_t*>(val));
    }
}
