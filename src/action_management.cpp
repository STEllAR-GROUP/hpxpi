//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Action Management [8.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_register_action registers an action with the runtime. It must be 
// performed in order to send parcels to the action. It must be performed in a 
// native C thread run on each locality, and may be performed as part of a static 
// constructor. 
//
// XPI_Err XPI_PROCESS_REGISTER_ACTION_ACTION(XPI_Addr process, char* id, 
//     void* function, XPI Type type, size t stack); // CONTINUE()
XPI_Type XPI_PROCESS_REGISTER_ACTION_ACTION = { 0 };

XPI_Err XPI_Process_register_action(XPI_Addr process, char* id, void* function, 
    XPI_Type type, size_t stack, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_Process_register_action_sync(XPI_Addr process, char* id, 
    void* function, XPI_Type type, size_t stack)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

