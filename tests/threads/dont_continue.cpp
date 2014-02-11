//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

bool executed=false;

XPI_Parcel p;

///////////////////////////////////////////////////////////////////////////////
XPI_Err some_action(void* nothing)
{
    executed=true;
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    XPI_register_action(some_action);
    XPI_register_action(XPI_ACTION_NULL); //Shouuldn't need to do this
    XPI_Parcel_create(&p);
    XPI_Parcel_set_addr(p, XPI_NULL);
    XPI_Parcel_set_action(p, some_action);
    XPI_Parcel_push(p);
    XPI_Parcel_set_addr(p, XPI_NULL);
    XPI_Parcel_set_action(p, XPI_ACTION_NULL);
    HPX_TEST_EQ(XPI_Parcel_send(p, XPI_NULL), XPI_SUCCESS);

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

    HPX_TEST_EQ(executed, false);

    return hpx::util::report_errors();
}
