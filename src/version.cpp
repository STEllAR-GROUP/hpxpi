//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/config/version.h>

#if defined(__cplusplus)
extern "C" {
#endif

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

#if defined(__cplusplus)
}
#endif

