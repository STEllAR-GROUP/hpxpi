//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpx/hpx_finalize.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// XPI_abort unconditionally aborts the execution of an XPI application,
// returning control to the XPI_run site with the result set to the passed
// code, and optionally printing the message. This should clean up resources
// associated with the running application.
HPXPI_ATTRIBUTE_NORETURN void XPI_abort(int code, char* message)
{
    hpx::terminate();
}

#if defined(__cplusplus)
}
#endif

