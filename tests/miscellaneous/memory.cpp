//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/impl/addr.hpp>

#include <hpx/util/lightweight_test.hpp>

///////////////////////////////////////////////////////////////////////////////
void test_load_store_u8()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint8_t),
        XPI_LOCAL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint8_t),
        XPI_LOCAL, &mem), XPI_SUCCESS);

    uint8_t src = 0x55ul;
    uint8_t dest = 0ul;

    HPX_TEST_EQ(XPI_Agas_store_u8_sync(XPI_NULL, src), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Agas_load_u8_sync(XPI_NULL, &dest), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(dest, 0ul);

    HPX_TEST_EQ(XPI_Agas_load_u8_sync(mem, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Agas_store_u8_sync(mem, src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Agas_load_u8_sync(mem, &dest), XPI_SUCCESS);
    HPX_TEST_EQ(dest, src);

    HPX_TEST_EQ(XPI_Process_global_free_sync(process, XPI_NULL), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Process_global_free_sync(process, mem), XPI_SUCCESS);
}

void test_load_store_u16()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint16_t),
        XPI_LOCAL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint16_t),
        XPI_LOCAL, &mem), XPI_SUCCESS);

    uint16_t src = 0x0123ul;
    uint16_t dest = 0ul;

    HPX_TEST_EQ(XPI_Agas_store_u16_sync(XPI_NULL, src), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Agas_load_u16_sync(XPI_NULL, &dest), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(dest, 0ul);

    HPX_TEST_EQ(XPI_Agas_load_u16_sync(mem, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Agas_store_u16_sync(mem, src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Agas_load_u16_sync(mem, &dest), XPI_SUCCESS);
    HPX_TEST_EQ(dest, src);

    HPX_TEST_EQ(XPI_Process_global_free_sync(process, XPI_NULL), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Process_global_free_sync(process, mem), XPI_SUCCESS);
}

void test_load_store_u32()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint32_t),
        XPI_LOCAL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint32_t),
        XPI_LOCAL, &mem), XPI_SUCCESS);

    uint32_t src = 0x01234567ul;
    uint32_t dest = 0ul;

    HPX_TEST_EQ(XPI_Agas_store_u32_sync(XPI_NULL, src), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Agas_load_u32_sync(XPI_NULL, &dest), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(dest, 0ul);

    HPX_TEST_EQ(XPI_Agas_load_u32_sync(mem, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Agas_store_u32_sync(mem, src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Agas_load_u32_sync(mem, &dest), XPI_SUCCESS);
    HPX_TEST_EQ(dest, src);

    HPX_TEST_EQ(XPI_Process_global_free_sync(process, XPI_NULL), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Process_global_free_sync(process, mem), XPI_SUCCESS);
}

void test_load_store_u64()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint64_t),
        XPI_LOCAL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint64_t),
        XPI_LOCAL, &mem), XPI_SUCCESS);

    uint64_t src = 0x0123456789abcdefull;
    uint64_t dest = 0ull;

    HPX_TEST_EQ(XPI_Agas_store_u64_sync(XPI_NULL, src), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Agas_load_u64_sync(XPI_NULL, &dest), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(dest, 0ull);

    HPX_TEST_EQ(XPI_Agas_load_u64_sync(mem, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Agas_store_u64_sync(mem, src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Agas_load_u64_sync(mem, &dest), XPI_SUCCESS);
    HPX_TEST_EQ(dest, src);

    HPX_TEST_EQ(XPI_Process_global_free_sync(process, XPI_NULL), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Process_global_free_sync(process, mem), XPI_SUCCESS);
}

void test_load_store_u128()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(XPI_Addr),
        XPI_LOCAL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(XPI_Addr),
        XPI_LOCAL, &mem), XPI_SUCCESS);

    XPI_Addr src = { 0xfedcba9876543210ull, 0x0123456789abcdefull };
    XPI_Addr dest = XPI_NULL;

    HPX_TEST_EQ(XPI_Agas_store_addr_sync(XPI_NULL, src), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Agas_load_addr_sync(XPI_NULL, &dest), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(dest, XPI_NULL);

    HPX_TEST_EQ(XPI_Agas_load_addr_sync(mem, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Agas_store_addr_sync(mem, src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Agas_load_addr_sync(mem, &dest), XPI_SUCCESS);
    HPX_TEST_EQ(dest, src);

    HPX_TEST_EQ(XPI_Process_global_free_sync(process, XPI_NULL), XPI_ERR_INV_ADDR);
    HPX_TEST_EQ(XPI_Process_global_free_sync(process, mem), XPI_SUCCESS);
}

XPI_Addr create_future()
{
    XPI_Addr process = XPI_NULL;
    HPX_TEST_EQ(
        XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
        XPI_SUCCESS);
    XPI_Addr lco = XPI_NULL;
    HPX_TEST_EQ(
        XPI_Process_future_new_sync(process, 1, 0, XPI_LOCAL, &lco),
        XPI_SUCCESS);
    return lco;
}

void test_load_store_u8_actions()
{
    XPI_Addr lco = create_future();

    // actual test
    XPI_Parcel p;
    HPX_TEST_EQ(XPI_Parcel_create(&p), XPI_SUCCESS);

    uint8_t src = 0x55ul;
    uint8_t dest = 0ul;

    HPX_TEST_EQ(XPI_Parcel_set_addr(p, lco), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, XPI_LCO_TRIGGER_ACTION), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_push(p), XPI_SUCCESS);

    XPI_Addr dest_addr = XPI_NULL;
    HPX_TEST_EQ(XPI_Addr_init((uint64_t)&dest, &dest_addr), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_addr(p, dest_addr), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, XPI_AGAS_LOAD_U8_ACTION), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_push(p), XPI_SUCCESS);

    XPI_Addr src_addr = XPI_NULL;
    HPX_TEST_EQ(XPI_Addr_init((uint64_t)&dest, &src_addr), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_addr(p, src_addr), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_data(p, sizeof(uint8_t), &src), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, XPI_AGAS_STORE_U8_ACTION), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_Parcel_send(p, XPI_NULL, XPI_NULL), XPI_SUCCESS);

    // wait for test to finish
    HPX_TEST_EQ(XPI_Thread_wait(lco, 0, 0), XPI_SUCCESS);

    // release resources
    HPX_TEST_EQ(XPI_Parcel_free(p), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_LCO_free_sync(lco), XPI_SUCCESS);

    HPX_TEST_EQ(dest, src);
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    test_load_store_u8();
    test_load_store_u16();
    test_load_store_u32();
    test_load_store_u64();
    test_load_store_u128();

    test_load_store_u8_actions();

    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int result = 0;

    HPX_TEST_EQ(XPI_init(&argc, &argv, 0), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_run(argc, argv, &result), XPI_SUCCESS);
    HPX_TEST_EQ(result, XPI_SUCCESS);

    HPX_TEST_EQ(XPI_finalize(), XPI_SUCCESS);

    return hpx::util::report_errors();
}
