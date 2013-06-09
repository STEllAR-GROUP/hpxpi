//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/util/lightweight_test.hpp>

bool executed_XPI_main = false;

///////////////////////////////////////////////////////////////////////////////
XPI_Err XPI_main(size_t nargs, void* args[])
{
    size_t major = 0;
    size_t minor = 0;
    size_t release = 0;

    XPI_version(&major, &minor, &release);

    HPX_TEST_EQ(major, XPI_VERSION_MAJOR);
    HPX_TEST_EQ(minor, XPI_VERSION_MINOR);
    HPX_TEST_EQ(release, XPI_VERSION_RELEASE);

    executed_XPI_main = true;

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

    HPX_TEST(executed_XPI_main);

    return hpx::util::report_errors();
}
