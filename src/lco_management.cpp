//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// LCO Management [8.4]
///////////////////////////////////////////////////////////////////////////////

// This action, and its corresponding asynchronous routine, deal with the 
// special allocation requirements for LCOs. In particular, LCOs may need 
// extra, implementation-specific space in order to implement some of the 
// required LCO properties including strict serializability and polymorphic 
// action handlers.
//
// XPI_Err XPI_PROCESS_LCO_MALLOC_ACTION(XPI_Addr process, size_t size, 
//      XPI_LCO_Subtype_Descriptor actions); // CONTINUE(XPI_Addr address)
XPI_Type XPI_PROCESS_LCO_MALLOC_ACTION = { 0 };

XPI_Err XPI_Process_lco_malloc(XPI_Addr process, size_t size, 
    XPI_LCO_Subtype_Descriptor actions, XPI_Addr future)
{
    return XPI_SUCCESS;
}


#if defined(__cplusplus)
}
#endif


