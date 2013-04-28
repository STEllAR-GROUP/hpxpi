//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/include/parcelset.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// XPI_Parcel_select can be used to explicitly receive parcel handles from the 
// network layer.
XPI_Err XPI_Parcel_select(char* match, size_t n, XPI_Parcel parcels[], size_t* matched)
{
    return XPI_SUCCESS;
}

// Spec misses documentation
XPI_Err XPI_Parcel_get_target(XPI_Parcel handle, XPI_Addr* target)
{
    return XPI_SUCCESS;
}

// Spec misses documentation
XPI_Err XPI_Parcel_get_action(XPI_Parcel handle, char** action)
{
    return XPI_SUCCESS;
}

// Spec misses documentation
XPI_Err XPI_Parcel_get_cont_target(XPI_Parcel handle, XPI_Addr* target)
{
    return XPI_SUCCESS;
}

// Spec misses documentation
XPI_Err XPI_Parcel_get_cont_action(XPI_Parcel handle, char** action)
{
    return XPI_SUCCESS;
}

// This returns the XPI TYPE for the parcel's action. It will never return 
// XPI_VOID, If the type was specified as part of XPI_PARCEL_SET_DATA, it is 
// returned, otherwise, the type set as part of XPI_PROCESS_REGISTER_ACTION is 
// returned. This can be combined with XPI_TYPE_GET_SIZE to allocate a buffer 
// for XPI_PARCEL_GET_DATA.
XPI_Err XPI_Parcel_get_data_type(XPI_Parcel handle, XPI_Type* type)
{
    return XPI_SUCCESS;
}

// XPI_Parcel_get_n_args retrieves the number of arguments encoded in the 
// parcel's data segment. This information can be used to allocate an 
// appropriately sized argument list buffer for use in XPI_PARCEL_GET_DATA.
XPI_Err XPI_Parcel_get_n_args(XPI_Parcel handle, size_t* n)
{
    return XPI_SUCCESS;
}

// XPI_Parcel_get_data unpacks the data segment into buffer and fills the 
// arguments array with the appropriate argument addresses from within the 
// buffer. The buffers must be large enough to accept the parcels' data (see 
// XPI_PARCEL_GET_DATA_TYPE and XPI_PARCEL_GET_N_ARGS).
XPI_Err XPI_Parcel_get_data(XPI_Parcel handle, void** arguments, void* data)
{
    return XPI_SUCCESS;
}

#if defined(__cplusplus)
}
#endif

