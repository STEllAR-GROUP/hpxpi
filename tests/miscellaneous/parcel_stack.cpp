//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    XPI_Parcel parcel;
    HPX_TEST_EQ(XPI_Parcel_create(&parcel), XPI_SUCCESS);

    char data[2] = { 'a', 'b' };
    HPX_TEST_EQ(XPI_Parcel_set_env(parcel, 2*sizeof(char), data), XPI_SUCCESS);
    HPX_TEST_EQ(XPI_Parcel_push(parcel), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_Parcel_pop(parcel, XPI_NULL), XPI_SUCCESS);

    HPX_TEST_EQ(XPI_Parcel_free(parcel), XPI_SUCCESS);

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
