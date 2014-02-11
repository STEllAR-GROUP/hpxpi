//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

#include <iostream>
using namespace std;

const int fib_n=10;

typedef struct{
    int n;
    XPI_Addr future;
} fib_data;

XPI_Err fib_naive(void* data){
    fib_data cast_args=(fib_data*)data;
    //Alocate futures
    XPI_Addr futures[2];
    //Ignore distribution
    HPX_TEST_EQ(XPI_Process_future_new_sync(2,sizeof(int),XPI_NULL,&futures), XPI_SUCESS);
    //Create structs
    fib_data d1={cast_data->n-1, futures[0]};
    fib_data d2={cast_data->n-2, futures[1]};
    //Send parcels
    XPI_Parcel_appy(XPI_NULL, fib_naive, sizeof(fib_data), &d1, XPI_NULL);
    XPI_Parcel_appy(XPI_NULL, fib_naive, sizeof(fib_data), &d2, XPI_NULL);
    //Wait on futures
    void* results[2];
    HPX_TEST_EQ(XPI_Thread_wait_all(2,futures,results), XPI_SUCESS);
    int result=*(int*)results[0]+*(int*)results[1];
    //Send results
    HPX_TEST_EQ(XPI_LCO_trigger(cast_args->future, &result, XPI_NULL), XPI_SUCESS);
    return XPI_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    XPI_Addr result;
    int r;
    HPX_TEST_EQ(XPI_Process_future_new_sync(1,sizeof(int),XPI_NULL,&result), XPI_SUCESS);
    fib_data init={fib_n, result};
    XPI_Parcel_appy(XPI_NULL, fib_naive, sizeof(fib_data), &init, XPI_NULL);
    HPX_TEST_EQ(XPI_Thread_wait(result,&r),XPI_SUCESS);
    cout<<r<<endl;
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    HPX_TEST_EQ(XPI_init(&argc, &argv, 0), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_run(argc, argv, &result), XPI_SUCCESS);
    HPX_TEST_EQ(result, XPI_SUCCESS);

    HPX_TEST_EQ(XPI_finalize(), XPI_SUCCESS);

    return hpx::util::report_errors();
}
