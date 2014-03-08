//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

#include <iostream>
using namespace std;

int const fib_n = 10;

typedef struct fib_data {
    int n;
    XPI_Addr future;
} fib_data;

XPI_Err fib_naive(void* data)
{
    fib_data* cast_args = (fib_data*)data;

    int result = 0;
    if (cast_args->n > 2)
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

        // Create arguments
        fib_data d1 = { cast_args->n - 1, futures[0] };
        fib_data d2 = { cast_args->n - 2, futures[1] };

        // Send parcels
        HPX_TEST_EQ(
            XPI_Parcel_apply_sync(process, &fib_naive, sizeof(fib_data), &d1),
            XPI_SUCCESS);
        HPX_TEST_EQ(
            XPI_Parcel_apply_sync(process, &fib_naive, sizeof(fib_data), &d2),
            XPI_SUCCESS);

        // Wait on futures
        int result1 = 0, result2 = 0;
        void* results[2] = { &result1, &result2 };
        HPX_TEST_EQ(XPI_Thread_wait_all(2, futures, results), XPI_SUCCESS);

        result = result1 + result2;

        // Free the futures (FIXME: why do I have to do that separately?).
        HPX_TEST_EQ(XPI_LCO_free_sync(futures[0]), XPI_SUCCESS);
        HPX_TEST_EQ(XPI_LCO_free_sync(futures[1]), XPI_SUCCESS);
    }
    else
    {
        result = cast_args->n;
    }

    // Send results
    HPX_TEST_EQ(XPI_LCO_trigger_sync(cast_args->future, &result), XPI_SUCCESS);

    return XPI_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    HPX_TEST_EQ(XPI_register_action_with_key(&fib_naive, "fib_naive"),
        XPI_SUCCESS);

    XPI_Addr process = XPI_NULL;

//     FIXME: Calling XPI_Thread_get_self in XPI_main is not supported yet
//     HPX_TEST_EQ(
//         XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process),
//         XPI_SUCCESS);

    XPI_Addr result = XPI_NULL;
    HPX_TEST_EQ(
        XPI_Process_future_new_sync(process, 1, sizeof(int),
            XPI_LOCAL, &result),
        XPI_SUCCESS);

    fib_data init = { fib_n, result };
    HPX_TEST_EQ(
        XPI_Parcel_apply_sync(process, &fib_naive, sizeof(fib_data), &init),
        XPI_SUCCESS);

    int r = 0;
    HPX_TEST_EQ(XPI_Thread_wait(result, &r), XPI_SUCCESS);

    std::cout << "fib(" << fib_n << ") = " << r << std::endl;

    HPX_TEST_EQ(XPI_LCO_free_sync(result), XPI_SUCCESS);

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
