//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/config/version.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Threads [6]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_get_self can be used to get the global address corresponding to 
// the local thread, and can be used in conjunction with the thread object API 
// to query details of threads.
XPI_Err XPI_Thread_get_self(XPI_Addr* addr)
{
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Thread Synchronization [6.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_wait_any allows a threads execution to block until any one of the 
// specified LCOs fires. This should behave as if it is implemented in terms of 
// XPI_LCO_LINK, along with a scheduler transition of the thread to a depleted 
// state (which will be reset by the threads XPI_LCO_TRIGGER_ACTION handler).
XPI_Err XPI_Thread_wait_any(size_t n, XPI_Addr lcos[], XPI_Addr* lco)
{
    return XPI_SUCCESS;
}

// XPI_Thread_wait_all blocks until all of the LCOs in lcos have fired. This is 
// a convenience interface to the XPI_THREAD_WAIT_ANY routine, that behaves as 
// if it loops waiting for each LCO to fire, however it may have its own distinct 
// implementation for performance reasons.
XPI_Err XPI_Thread_wait_all(size_t n, XPI_Addr lcos[])
{
    return XPI_SUCCESS;
}

// XPI_Thread_wait blocks until the LCO fires. This is a convenience interface 
// to the XPI_THREAD_WAIT_ANY routine however it may have its own distinct 
// implementation for performance reasons.
XPI_Err XPI_Thread_wait(XPI_Addr obj)
{
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Thread Scheduling [6.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_set_priority will set the priority for the target thread.

// XPI_Err XPI_THREAD_SET_PRIORITY_ACTION(XPI_Addr address, size_t priority); // CONTINUE()
XPI_Type XPI_THREAD_SET_PRIORITY_ACTION = { 0 };

XPI_Err XPI_Thread_set_priority(XPI_Addr address, size_t priority, 
    XPI_Addr future)
{
    return XPI_SUCCESS;
}

// XPI_Thread_set_state changes the state of the target thread.

// XPI_Err XPI_THREAD_SET_STATE_ACTION(XPI_Addr address, XPI_Thread_State state); // CONTINUE()
XPI_Type XPI_THREAD_SET_STATE_ACTION = { 0 };

XPI_Err XPI_Thread_set_state(XPI_Addr address, XPI_Thread_State state, 
    XPI_Addr future)
{
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Thread Resources [6.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_get_process can be used to get the global address corresponding 
// to a thread's process.

// XPI_Err XPI_THREAD_GET_PROCESS_ACTION(XPI_Addr address); // CONTINUE(XPI_Addr process)
XPI_Type XPI_THREAD_GET_PROCESS_ACTION = { 0 };

XPI_Err XPI_Thread_get_process(XPI_Addr address, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Thread_get_process_sync(XPI_Addr address, XPI_Addr* process)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

