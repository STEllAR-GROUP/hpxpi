//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/impl/xpi_addr.hpp>

#include <hpx/util/lightweight_test.hpp>

///////////////////////////////////////////////////////////////////////////////
void test_load_store_u32()
{
    XPI_Addr mem = XPI_NULL, process = XPI_NULL;

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint32_t),
        XPI_DISTRIBUTION_NULL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint32_t),
        XPI_DISTRIBUTION_NULL, &mem), XPI_SUCCESS);

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
        XPI_DISTRIBUTION_NULL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(uint64_t),
        XPI_DISTRIBUTION_NULL, &mem), XPI_SUCCESS);

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
        XPI_DISTRIBUTION_NULL, 0), XPI_ERR_BAD_ARG);

    HPX_TEST_EQ(XPI_Process_global_malloc_sync(process, 1, sizeof(XPI_Addr),
        XPI_DISTRIBUTION_NULL, &mem), XPI_SUCCESS);

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

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    test_load_store_u32();
    test_load_store_u64();
    test_load_store_u128();
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
