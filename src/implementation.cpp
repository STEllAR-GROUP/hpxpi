//  Copyright (c) 2013 Hartmut Kaiser
//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/hpx_start.hpp>
#include <hpx/hpx_finalize.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/thread.hpp>
#include <hpxpi/impl/addr.hpp>

namespace hpxpi
{
    int xpi_main_wrapper(int argc, char** argv, XPI_Err (*XPI_main_)(size_t, void**));
}

///////////////////////////////////////////////////////////////////////////////
extern "C" {

    // XPI_version queries the specification version number that the XPI
    // implementation conforms to.
    void XPI_version(size_t* major, size_t* minor, size_t* release)
    {
        if (major)
            *major = XPI_VERSION_MAJOR;
        if (minor)
            *minor = XPI_VERSION_MINOR;
        if (release)
            *release = XPI_VERSION_RELEASE;
    }

#if !defined(HPXPI_NO_EXTENSIONS)
    // Version of the HPXPI implementation
    void HPXPI_version(size_t* major, size_t* minor, size_t* release)
    {
        if (major)
            *major = HPXPI_VERSION_MAJOR;
        if (minor)
            *minor = HPXPI_VERSION_MINOR;
        if (release)
            *release = HPXPI_VERSION_SUBMINOR;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////////
    // XPI_init initializes the XPI runtime, using the passed arguments and the
    // environment. C applications usually simply forward pointers to argc, argv,
    // and envp (if available). XPI specific arguments are removed from argc and
    // argv, and the environment is updated as necessary.
    XPI_Err XPI_init(int* nargs, char*** args, char*** env)
    {
        // we don't do anything here
        return XPI_SUCCESS;
    }

    // XPI_run encapsulates the creation of the main process and its associated
    // initial thread, XPI_main. XPI_run also manages the future required for
    // XPI_main to return a value. This routine is synchronous, and will not return
    // until the main process terminates.
    XPI_Err XPI_run_internal(int argc, char** argv, int* result,
        XPI_Err (*XPI_main_)(size_t, void*[]))
    {
        try {
            using hpx::util::placeholders::_1;
            using hpx::util::placeholders::_2;

            int r = hpx::init(hpx::util::bind(hpxpi::xpi_main_wrapper, _1, _2, XPI_main_),
                "XPI application", argc, argv);

            if (result)
                *result = r;

            return XPI_SUCCESS;
        }
        catch (hpx::exception const&) {
            return XPI_ERR_ERROR;
        }
        catch (...) {
            return XPI_ERR_ERROR;
        }
    }

    // XPI_finalize terminates the execution of the XPI runtime, releasing resources
    // acquired in XPI_init and XPI_run.
    XPI_Err XPI_finalize()
    {
        // we don't do anything here
        return XPI_SUCCESS;
    }

    // XPI_abort unconditionally aborts the execution of an XPI application,
    // returning control to the XPI_run site with the result set to the passed
    // code, and optionally printing the message. This should clean up resources
    // associated with the running application.
    void XPI_abort(int code, char* message)
    {
        hpx::terminate();
    }
}

