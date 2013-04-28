//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/util/find_prefix.hpp>

#include <boost/function.hpp>
#include <hpx/util/plugin.hpp>

///////////////////////////////////////////////////////////////////////////////
int hpx_main(int argc, char* argv[])
{
    // for now we just forward the call to XPI_main, pretending to invoke it 
    // as an action

    {
        typedef XPI_Err (*function_type)(size_t, void*[]);
        typedef boost::function<void(function_type)> deleter_type;

        // Bind the XPI_main symbol dynamically and invoke it.
        hpx::util::plugin::dll this_exe(hpx::util::get_executable_filename());
        std::pair<function_type, deleter_type> p = 
            this_exe.get<function_type, deleter_type>("XPI_main");

        p.first(argc, (void**)argv);
    }

    return hpx::finalize();
}

