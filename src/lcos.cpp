//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Fixed Actions [7.2.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_LCO_link adds the target LCO to the notification list for the source 
// LCO. When the source LCO's predicate evaluates as true, a parcel will be 
// generated targeting each linked LCO's trigger routine.
//
// XPI_Err XPI_LCO_LINK_ACTION(XPI_Addr source, XPI_Addr target); // CONTINUE()
XPI_Type XPI_LCO_LINK_ACTION = { 0 };

XPI_Err XPI_LCO_link(XPI_Addr source, XPI_Addr target, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_LCO_link_sync(XPI_Addr source, XPI_Addr target)
{
    return XPI_SUCCESS;
}

// XPI_LCO_unlink unlinks the target LCO from the source LCO, assuming that 
// the target LCO has been linked. It is not an error to attempt to unlink 
// a target that was not linked to the source.
//
// XPI_Err XPI_LCO_UNLINK_ACTION(XPI_Addr source, XPI_Addr target); // CONTINUE()
XPI_Type XPI_LCO_UNLINK_ACTION = { 0 };

XPI_Err XPI_LCO_unlink(XPI_Addr source, XPI_Addr target, XPI_Addr future)
{
    return XPI_SUCCESS;
}

XPI_Err XPI_LCO_unlink_sync(XPI_Addr source, XPI_Addr target)
{
    return XPI_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// Polymorphic Actions [7.2.2]
///////////////////////////////////////////////////////////////////////////////

// This is used to initialize an LCO structure after it has been allocated 
// by the process. The lco address must have been allocated using 
// XPI_PROCESS_LCO_MALLOC in order to ensure that it has enough space for 
// the XPI implementation to correctly provide the LCO properties.
//
// XPI_Err XPI_LCO_INIT_ACTION(XPI_Addr lco); // CONTINUE()
XPI_Type XPI_LCO_INIT_ACTION = { 0 };

XPI_Err XPI_LCO_init(XPI_Addr lco, XPI_Addr future)
{
    return XPI_SUCCESS;
}

// XPI_LCO_fini can be used to clean up any resources before an 
// LCO is freed using XPI_PROCESS_GLOBAL_FREE.
//
// XPI_Err XPI_LCO_FINI_ACTION(XPI_Addr lco); // CONTINUE()
XPI_Type XPI_LCO_FINI_ACTION = { 0 };

XPI_Err XPI_LCO_fini(XPI_Addr lco, XPI_Addr future)
{
    return XPI_SUCCESS;
}

// XPI_LCO_trigger action triggers the LCO to potentially change 
// state. There are special semantics with respect to the LCO trigger 
// action; the LCO's predicate is automatically tested after the trigger 
// executes.
//
// XPI_Err XPI_LCO_TRIGGER_ACTION(XPI_Addr lco); // CONTINUE()
XPI_Type XPI_LCO_TRIGGER_ACTION = { 0 };

XPI_Err XPI_LCO_trigger(XPI_Addr lco, XPI_Addr future)
{
    return XPI_SUCCESS;
}

// XPI_LCO_get_size action is used to read the size, in bytes, of 
// the LCO structure. This will not include any additional bytes 
// allocated by the process in order to provide support for LCO
// semantics for this object.
//
// XPI_Err XPI_LCO_GET_SIZE_ACTION(XPI_Addr lco); // CONTINUE(size t size)
XPI_Type XPI_LCO_GET_SIZE_ACTION = { 0 };

XPI_Err XPI_LCO_get_size(XPI_Addr lco, XPI_Addr future)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

