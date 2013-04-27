//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/include/naming.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// Loads a value from the global address space, and forwards it to a parcel 
// continuation, if one exists.
XPI_Err XPI_Agas_load1(XPI_Addr addr, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_load2(XPI_Addr addr, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_load4(XPI_Addr addr, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_load8(XPI_Addr addr, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_load1_sync(XPI_Addr addr, uint8_t* val)
{
    return 0;
}

XPI_Err XPI_Agas_load2_sync(XPI_Addr addr, uint16_t* val)
{
    return 0;
}

XPI_Err XPI_Agas_load4_sync(XPI_Addr addr, uint32_t* val)
{
    return 0;
}

XPI_Err XPI_Agas_load8_sync(XPI_Addr addr, uint64_t* val)
{
    return 0;
}

// Stores a value to the global address space.
XPI_Err XPI_Agas_store1(XPI_Addr addr, uint8_t val, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_store2(XPI_Addr addr, uint16_t val, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_store4(XPI_Addr addr, uint32_t val, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_store8(XPI_Addr addr, uint64_t val, XPI_Addr future)
{
    return 0;
}

XPI_Err XPI_Agas_store1_sync(XPI_Addr addr, uint8_t val)
{
    return 0;
}

XPI_Err XPI_Agas_store2_sync(XPI_Addr addr, uint16_t val)
{
    return 0;
}

XPI_Err XPI_Agas_store4_sync(XPI_Addr addr, uint32_t val)
{
    return 0;
}

XPI_Err XPI_Agas_store8_sync(XPI_Addr addr, uint64_t val)
{
    return 0;
}

#if defined(__cplusplus)
}
#endif

