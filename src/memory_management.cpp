//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Memory Management [8.3]
///////////////////////////////////////////////////////////////////////////////

// Allocates a size-byte region in global memory. The distribution parameter
// provides a hint to the implementation of how this allocation should be
// initially distributed.
//
// XPI_Err XPI_PROCESS_GLOBAL_MALLOC_ACTION(XPI_Addr process, size_t size,
//     XPI_Distribution distribution); // CONTINUE(XPI_Addr address);
XPI_Type XPI_PROCESS_GLOBAL_MALLOC_ACTION = { 0 };

XPI_Err XPI_Process_global_malloc(XPI_Addr process, size_t size,
    XPI_Distribution distribution, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_global_malloc_sync(XPI_Addr process,
    size_t size, XPI_Distribution distribution, XPI_Addr* address)
{
    return XPI_SUCCESS;
}

// XPI_Process_global_free frees a region of globally allocated memory. The
// address must be the result of an XPI_PROCESS_GLOBAL_MALLOC_ACTION call.
// This call is asynchronous, however the future can be used to wait until
// the operation has completed globally. It is not an error to free XPI_NULL.
//
// XPI_Err XPI_PROCESS_GLOBAL_FREE_ACTION(XPI_Addr process, 
//      XPI_Addr address); // CONTINUE()
XPI_Type XPI_PROCESS_GLOBAL_FREE_ACTION = { 0 };

XPI_Err XPI_Process_global_free(XPI_Addr process, XPI_Addr address,
    XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_global_free_sync(XPI_Addr process,
    XPI_Addr address)
{
    return XPI_SUCCESS;
}

// Pins a global address range. The entire range should be part of a single
// allocation, and resident locally.
//
// XPI_Err XPI_PROCESS_PIN_ACTION(XPI_Addr process, XPI_Addr base, 
//      XPI_Addr_Diff extent); // CONTINUE(void address)
XPI_Type XPI_PROCESS_PIN_ACTION = { 0 };

XPI_Err XPI_Process_pin(XPI_Addr process, XPI_Addr base,
    XPI_Addr_Diff extent, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_pin_sync(XPI_Addr process, XPI_Addr base,
    XPI_Addr_Diff extent, void** result)
{
    return XPI_SUCCESS;
}

// This action releases a previously pinned region to the system. The address 
// must correspond to the global base address of a previously pinned region.
//
// XPI_Err XPI_PROCESS_UNPIN_ACTION(XPI_Addr process, XPI_Addr address); // CONTINUE()
XPI_Type XPI_PROCESS_UNPIN_ACTION = { 0 };

XPI_Err XPI_Process_unpin(XPI_Addr process, XPI_Addr address, 
    XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_unpin_sync(XPI_Addr process, XPI_Addr address)
{
    return XPI_SUCCESS;
}

// XPI_Process_memcpy copies bytes in the global address space asynchronously. 
// The from and to ranges may overlap. The future provides strong ordering if 
// needed.
//
// XPI_Err XPI_PROCESS_MEMCPY_ACTION(XPI_Addr process, XPI_Addr from, 
//      XPI_Addr to, size_t bytes); // CONTINUE(XPI Addr from)
XPI_Type XPI_PROCESS_MEMCPY_ACTION = { 0 };

XPI_Err XPI_Process_memcpy(XPI_Addr process, XPI_Addr from, 
    XPI_Addr to, size_t bytes, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_memcpy_sync(XPI_Addr process, XPI_Addr from, 
    XPI_Addr to, size_t bytes, XPI_Addr* result)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

