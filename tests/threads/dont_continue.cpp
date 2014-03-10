//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

int executed = 0;

///////////////////////////////////////////////////////////////////////////////
XPI_Err some_action(void* nothing)
{
    ++executed;
    XPI_continue1(0, 0);
    return XPI_SUCCESS;
}

void test_simple_target()
{
    XPI_Parcel p;

    HPX_TEST_EQ(XPI_Parcel_create(&p), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_addr(p, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, &some_action), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_send(p, XPI_NULL, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_free(p), XPI_SUCCESS);
}

void test_empty_continuation()
{
    XPI_Parcel p = XPI_PARCEL_NULL;

    HPX_TEST_EQ(XPI_Parcel_create(&p), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_addr(p, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, &some_action), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_push(p), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_addr(p, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_set_action(p, XPI_ACTION_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_send(p, XPI_NULL, XPI_NULL), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_free(p), XPI_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    HPX_TEST_EQ(
        XPI_register_action_with_key(some_action, "some_action"),
        XPI_SUCCESS);

    test_simple_target();
    test_empty_continuation();

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

    HPX_TEST_EQ(executed, 2);

    return hpx::util::report_errors();
}
