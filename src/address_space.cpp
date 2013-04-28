//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/include/naming.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

// This interprets the integer as an index into the byte array representation 
// of the global virtual address space, and initializes a structure suitable 
// for use in the parcel subsystem. Arbitrarily large addresses cannot be 
// generated this way, further address arithmetic will be required to produce 
// very large addresses.
XPI_Err XPI_Addr_init(uint128_t address, XPI_Addr* result)
{
    return XPI_SUCCESS;
}

// This compares two addresses, setting result to -1, 0, or 1 if lhs is 
// less-than, equal-to, or greater-than rhs, respectively.
int XPI_Addr_cmp(XPI_Addr lhs, XPI_Addr rhs)
{
    return XPI_SUCCESS;
}

// This converts the integer into an address difference structure. Arbitrarily 
// large differences cannot be generated this way, and will need to be 
// generated using the address difference structure’s math interface. 
// Differences are not valid parcel targets.
XPI_Err XPI_Addr_Diff_init(uint128_t address, XPI_Addr_Diff* result)
{
    return XPI_SUCCESS;
}

// This adds two differences to produce a sum.
XPI_Addr_Diff XPI_Addr_Diff_add(XPI_Addr_Diff lhs, XPI_Addr_Diff rhs)
{
    XPI_Addr_Diff diff = { 0 };
    return diff;
}

// This scales an address difference by a given factor, which may be a fraction. 
// Rounding rules follow C integer rounding.
XPI_Addr_Diff XPI_Addr_Diff_mult(XPI_Addr_Diff base, float n)
{
    XPI_Addr_Diff diff = { 0 };
    return diff;
}

// This subtracts two differences to produce a difference.
XPI_Addr_Diff XPI_Addr_sub(XPI_Addr lhs, XPI_Addr rhs)
{
    XPI_Addr_Diff diff = { 0 };
    return diff;
}

// This add an address and a difference to produce another address.
XPI_Addr XPI_Addr_add(XPI_Addr lhs, XPI_Addr_Diff offset)
{
    XPI_Addr addr = { 0 };
    return addr;
}

#if defined(__cplusplus)
}
#endif

