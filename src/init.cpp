//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx_finalize.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// XPI_init initializes the XPI runtime, using the passed arguments and the
// environment. C applications usually simply forward pointers to argc, argv,
// and envp (if available). XPI specific arguments are removed from argc and
// argv, and the environment is updated as necessary.
XPI_Err XPI_init(int* nargs, char*** args, char*** env)
{
    return XPI_SUCCESS;
}

// XPI_run encapsulates the creation of the main process and its associated
// initial thread, XPI_main. XPI_run also manages the future required for
// XPI_main to return a value. This routine is synchronous, and will not return
// until the main process terminates.
XPI_Err XPI_run(int argc, char* argv[], int* result)
{
    return XPI_SUCCESS;
}

// XPI_abort unconditionally aborts the execution of an XPI application,
// returning control to the XPI_run site with the result set to the passed
// code, and optionally printing the message. This should clean up resources
// associated with the running application.
XPI_Err XPI_finalize()
{
    return XPI_SUCCESS;
}

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

