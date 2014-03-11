//  Copyright (c) 2013-2014 Alexander Duchene
//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// The code below calculates 'fibonacci(fib_n)' and should yield 'fib_result'.
int const fib_n = 10;
int const fib_result = 55;

///////////////////////////////////////////////////////////////////////////////
XPI_Err fib_synchronous(void* data)
{
    int n = *(int*)data;

    int result = 0;
    if (n >= 2)
    {
        XPI_Addr process = XPI_NULL;
        HPX_TEST_EQ(
            XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
            XPI_SUCCESS);

        int n1 = n - 1, n2 = n - 2;
        int result1 = 0, result2 = 0;

        // Send parcels
        HPX_TEST_EQ(
            XPI_Parcel_apply_sync(process, &fib_synchronous, sizeof(int), &n1,
                sizeof(int), &result1),
            XPI_SUCCESS);
        HPX_TEST_EQ(
            XPI_Parcel_apply_sync(process, &fib_synchronous, sizeof(int), &n2,
                sizeof(int), &result2),
            XPI_SUCCESS);

        result = result1 + result2;
    }
    else
    {
        result = n;
    }

    // Send results
    XPI_continue(sizeof(int), &result);

    return XPI_SUCCESS;
}

int test_fib_synchronous(int n)
{
    XPI_Addr process = XPI_NULL;
    HPX_TEST_EQ(
        XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
        XPI_SUCCESS);

    int result = 0;
    HPX_TEST_EQ(
        XPI_Parcel_apply_sync(process, &fib_synchronous, sizeof(int), &n,
            sizeof(result), &result),
        XPI_SUCCESS);

    return result;
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err fib_asynchronous(void* data)
{
    int n = *(int*)data;

    int result = 0;
    if (n >= 2)
    {
        XPI_Addr process = XPI_NULL;
        HPX_TEST_EQ(
            XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
            XPI_SUCCESS);

        // Allocate futures
        XPI_Addr futures[2] = { XPI_NULL, XPI_NULL };
        HPX_TEST_EQ(
            XPI_Process_future_new_sync(process, 2, sizeof(int),
                XPI_LOCAL, futures),
            XPI_SUCCESS);

        // Send parcels
        int n1 = n - 1, n2 = n - 2;
        HPX_TEST_EQ(
            XPI_Parcel_apply(process, &fib_asynchronous, sizeof(int), &n1,
                futures[0]),
            XPI_SUCCESS);
        HPX_TEST_EQ(
            XPI_Parcel_apply(process, &fib_asynchronous, sizeof(int), &n2,
                futures[1]),
            XPI_SUCCESS);

        int result1 = 0, result2 = 0;

        size_t sizes[2] = { sizeof(int), sizeof(int) };
        void* results[2] = { &result1, &result2 };
        HPX_TEST_EQ(
            XPI_Thread_wait_all(2, futures, sizes, results),
            XPI_SUCCESS);

        result = result1 + result2;

        // Free the futures (FIXME: why do I have to do that separately?).
        HPX_TEST_EQ(XPI_LCO_free_sync(futures[0]), XPI_SUCCESS);
        HPX_TEST_EQ(XPI_LCO_free_sync(futures[1]), XPI_SUCCESS);
    }
    else
    {
        result = n;
    }

    // Send results
    XPI_continue(sizeof(int), &result);

    return XPI_SUCCESS;
}

int test_fib_asynchronous(int n)
{
    XPI_Addr process = XPI_NULL;
    HPX_TEST_EQ(
        XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
        XPI_SUCCESS);

    int result = 0;
    HPX_TEST_EQ(
        XPI_Parcel_apply_sync(process, &fib_asynchronous, sizeof(int), &n,
            sizeof(result), &result),
        XPI_SUCCESS);

    return result;
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    HPX_TEST_EQ(
        XPI_register_action_with_key(&fib_synchronous, "fib_synchronous"),
        XPI_SUCCESS);
    HPX_TEST_EQ(
        XPI_register_action_with_key(&fib_asynchronous, "fib_asynchronous"),
        XPI_SUCCESS);

    HPX_TEST_EQ(test_fib_synchronous(fib_n), fib_result);
    HPX_TEST_EQ(test_fib_asynchronous(fib_n), fib_result);

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
