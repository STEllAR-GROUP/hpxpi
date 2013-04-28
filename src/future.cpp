//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Future [7.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Future_new is for future allocation. As with all LCOs, futures are
// allocated in the global namespace. For local allocation, process can be set 
// to XPI_NULL in which case both of these routines behave as if they are 
// implemented with XPI_PROCESS_LCO_MALLOC_ACTION, using the parent process of 
// the calling thread (see XPI_THREAD_GET_PROCESS) as the allocating process.
// 
// XPI_FUTURE_NEW_SYNC is of particular interest, as it solves the chicken-and-egg 
// problem of needing to allocate a future in order to read the global address of 
// the newly allocated future by hiding this process inside the library.
//
// XPI_Err XPI_FUTURE_NEW_ACTION(XPI_Type type); // CONTINUE(XPI_Addr address)
XPI_Type XPI_FUTURE_NEW_ACTION = { 0 };

XPI_Err XPI_Future_new(XPI_Type type, XPI_Type future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Future_new_sync(XPI_Type type, XPI_Addr* address)
{
    return XPI_SUCCESS;
}

// XPI_Future_get_buffer_type allows programmers to query the type of a future, 
// to make sure that it can buffer, and correctly extract, a particular type.
//
// XPI_Err XPI_FUTURE_GET_BUFFER_TYPE_ACTION(XPI_Addr address); // CONTINUE(XPI_Type type)
XPI_Type XPI_FUTURE_GET_BUFFER_TYPE_ACTION = { 0 };

XPI_Err XPI_Future_get_buffer_type(XPI_Addr address, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Future_get_buffer_type_sync(XPI_Addr address, XPI_Type* type)
{
    return XPI_SUCCESS;
}

// XPI_Future_get_value_sync, along with XPI_FUTURE_NEW_SYNC, is the second 
// magic synchronous function related to futures. It transfers the value of 
// the data begin buffered to the passed local address. This routine will fail 
// if the global address of the future is not in the same synchronous
// domain as that of the calling thread. The buffer must be large enough to 
// receive the data described by the futures type returned by 
// XPI_FUTURE_GET_BUFFER_TYPE.
XPI_Err XPI_Future_get_value_sync(XPI_Addr address, void* buffer)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

