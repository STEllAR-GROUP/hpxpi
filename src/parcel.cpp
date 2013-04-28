//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/include/parcelset.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// Spec misses documentation
XPI_Err XPI_Parcel_create(XPI_Parcel* handle)
{
    return XPI_SUCCESS;
}

// Spec misses documentation
XPI_Err XPI_Parcel_free(XPI_Parcel handle)
{
    return XPI_SUCCESS;
}

// The target address must have a valid mapping at the time that 
// XPI_PARCEL_SEND is called.
XPI_Err XPI_Parcel_set_target(XPI_Parcel handle, XPI_Addr target)
{
    return XPI_SUCCESS;
}

// The action must correspond to an action that was registered with the XPI 
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_PARCEL_SEND being 
// called.
XPI_Err XPI_Parcel_set_action(XPI_Parcel handle, char* action)
{
    return XPI_SUCCESS;
}

// The continuation target address must have a valid mapping at the time that 
// XPI_CONTINUE is called.
XPI_Err XPI_Parcel_set_cont_target(XPI_Parcel handle, XPI_Addr target)
{
    return XPI_SUCCESS;
}

// The action must correspond to an action that was registered with the XPI 
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_CONTINUE being called.
XPI_Err XPI_Parcel_set_cont_action(XPI_Parcel handle, char* action)
{
    return XPI_SUCCESS;
}

// Bind the individual action arguments to the action's data.
XPI_Err XPI_Parcel_set_data(XPI_Parcel handle, XPI_Type type, size_t n, 
    void* arguments[])
{
    return XPI_SUCCESS;
}

// XPI_Parcel_send send a parcel.
XPI_Err XPI_Parcel_send(XPI_Parcel handle, XPI_Addr future)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

