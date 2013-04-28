//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Process Management [8.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_create_child creates a child process. This child will have its 
// parent process set to the passed process, and its designated result value 
// may be written to the result future. If no result is required by the 
// instantiating thread, i.e., the process is being instantiated only for its 
// side effects, then result should be set to XPI_NULL.
//
// XPI_Err XPI_PROCESS_CREATE_CHILD_ACTION(XPI_Addr process, char* initial,
//     XPI_Type type, size_t nargs, void* args[], XPI_Addr result); // CONTINUE(XPI_Addr child)
XPI_Type XPI_PROCESS_CREATE_CHILD_ACTION = { 0 };

XPI_Err XPI_Process_create_child(XPI_Addr process, char* initial,
    XPI_Type type, size_t nargs, void* args[], XPI_Addr result, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_create_child_sync(XPI_Addr process, char* initial,
    XPI_Type type, size_t nargs, void* args[], XPI_Addr result, XPI_Addr* child)
{
    return XPI_SUCCESS;
}

// XPI_Process_get_result gets the designated result for a process, in the form 
// of a future. The is the future that is passed into XPI_PROCESS_CREATE_CHILD, 
// and can be XPI_NULL if this was not set.
//
// XPI_Err XPI_PROCESS_GET_RESULT_ACTION(XPI_Addr process); // CONTINUE(XPI_Addr result)
XPI_Type XPI_PROCESS_GET_RESULT_ACTION = { 0 };

XPI_Err XPI_Process_get_result(XPI_Addr process, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_get_result_sync(XPI_Addr process, XPI_Addr* result)
{
    return XPI_SUCCESS;
}

// XPI_Process_terminate unconditionally terminates the target process. This 
// terminates execution of any threads. XPI_PROCESS_TERMINATE_ACTION or 
// XPI_PROCESS_TERMINATE may be performed by a thread in the context of the 
// to-be-terminated process, however performing any additional operations within 
// the context of this thread's execution results in implementation-defined
// behavior, as does using XPI_PROCESS_TERMINATE_SYNC from within the context 
// of the to-beterminated process.
//
// XPI_Err XPI_PROCESS_TERMINATE_ACTION(XPI_Addr process); // CONTINUE()
XPI_Type XPI_PROCESS_TERMINATE_ACTION = { 0 };

XPI_Err XPI_Process_terminate(XPI_Addr process, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_terminate_sync(XPI_Addr process)
{
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Hierarchy Inspection [8.1.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_get_parent gets the address of a processes' parent process. The 
// main process will return XPI_NULL. An orphaned process will return the address 
// of the main process.
//
// XPI_Err XPI_PROCESS_GET_PARENT_ACTION(XPI_Addr process); // CONTINUE(XPI_Addr parent)
XPI_Type XPI_PROCESS_GET_PARENT_ACTION = { 0 };

XPI_Err XPI_Process_get_parent(XPI_Addr process, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_get_parent_sync(XPI_Addr process, XPI_Addr* parent)
{
    return XPI_SUCCESS;
}

// This retrieves the number of children for a process.
//
// XPI_Err XPI_PROCESS_GET_N_CHILDREN_ACTION(XPI_Addr process); // CONTINUE(size_t n)
XPI_Type XPI_PROCESS_GET_N_CHILDREN_ACTION = { 0 };

XPI_Err XPI_Process_get_n_children(XPI_Addr process, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_get_n_children_sync(XPI_Addr process, size_t* n)
{
    return XPI_SUCCESS;
}

// XPI_Process_get_child retrieves the i-th child of a process. If i is out of the 
// range of valid child indices, this will return XPI_NULL.
//
// XPI_Err XPI_PROCESS_GET_CHILD(XPI_Addr process, size_t i); // CONTINUE(XPI_Addr child)
XPI_Type XPI_PROCESS_GET_CHILD = { 0 };

XPI_Err XPI_Process_get_child(XPI_Addr process, size_t i, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_get_child_sync(XPI_Addr process, size_t i, XPI_Addr* child)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

