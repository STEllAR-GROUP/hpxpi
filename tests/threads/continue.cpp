//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

const int num_cont = 99;
int counter = 0;

///////////////////////////////////////////////////////////////////////////////
XPI_Err count(void* nothing)
{
    ++counter;
    XPI_continue1(0, 0);
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    XPI_Parcel p = XPI_PARCEL_NULL;

    XPI_register_action_with_key(count, "count");
    XPI_Parcel_create(&p);

    for (size_t i = 0; i != num_cont; ++i)
    {
        HPX_TEST_EQ(XPI_Parcel_set_addr(p, XPI_NULL), XPI_SUCCESS);
        HPX_TEST_EQ(XPI_Parcel_set_action(p, count), XPI_SUCCESS);
        HPX_TEST_EQ(XPI_Parcel_push(p), XPI_SUCCESS);
    }

    // last push is empty
    HPX_TEST_EQ(XPI_Parcel_pop(p, XPI_NULL), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_Parcel_send(p, XPI_NULL, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_free(p), XPI_SUCCESS);

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

    HPX_TEST_EQ(counter, num_cont);

    return hpx::util::report_errors();
}
