//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#if !defined(HPXPI_INTERNAL_ADDR_HPP_MARCH_05_2014_0779PM)
#define HPXPI_INTERNAL_ADDR_HPP_MARCH_05_2014_0779PM

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/naming/name.hpp>

#include <hpxpi/xpi.h>

#include <ios>
#include <iomanip>
#include <iostream>

#include <boost/io/ios_state.hpp>
#include <boost/serialization/serialization.hpp>

///////////////////////////////////////////////////////////////////////////////
inline bool operator== (XPI_Addr lhs, XPI_Addr rhs)
{
    return lhs.msb == rhs.msb && lhs.lsb == rhs.lsb;
}

inline bool operator!= (XPI_Addr lhs, XPI_Addr rhs)
{
    return !(lhs == rhs);
}

inline bool operator< (XPI_Addr lhs, XPI_Addr rhs)
{
    if (lhs.msb < rhs.msb)
        return true;
    if (lhs.msb == rhs.msb)
        return lhs.lsb < rhs.lsb;
    return false;
}

inline bool operator>= (XPI_Addr lhs, XPI_Addr rhs)
{
    return !(lhs < rhs);
}

inline bool operator> (XPI_Addr lhs, XPI_Addr rhs)
{
    return !(lhs < rhs) && !(lhs == rhs);
}

inline bool operator<= (XPI_Addr lhs, XPI_Addr rhs)
{
    return !(lhs > rhs);
}

inline std::ostream& operator<< (std::ostream& os, XPI_Addr addr)
{
    boost::io::ios_flags_saver ifs(os);
    if (addr != XPI_NULL)
    {
        os << std::hex
            << "{" << std::right << std::setfill('0') << std::setw(16)
                    << addr.msb << ", "
                    << std::right << std::setfill('0') << std::setw(16)
                    << addr.lsb << "}";
    }
    else
    {
        os << "{invalid}";
    }
    return os;
}

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    // Return id_type representing given address, this does not take ownership
    // of any of the credits stored in the address
    inline hpx::id_type get_id(XPI_Addr addr)
    {
        return hpx::id_type(addr.msb, addr.lsb, hpx::id_type::unmanaged);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Release the credits stored in the given XPI_Addr.
    inline void release_id(XPI_Addr addr)
    {
        // The destructor of the temporary id_type created will trigger the
        // release of the referenced credits, which may result in the
        // destruction of the referenced object.
        hpx::id_type(addr.msb, addr.lsb, hpx::id_type::managed);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Return XPI_Addr representing given id, this releases the ownership to
    // the returned address.
    inline XPI_Addr from_id(hpx::id_type id)
    {
        XPI_Addr addr = { id.get_msb(), id.get_lsb() };
        id.make_unmanaged();        // release ownership
        return addr;
    }

    inline XPI_Addr from_id(hpx::naming::gid_type const& gid)
    {
        XPI_Addr addr = { gid.get_msb(), gid.get_lsb() };
        return addr;
    }
}

namespace boost { namespace serialization
{
    template <typename Archive>
    void serialize(Archive& ar, XPI_Addr addr, unsigned int version)
    {
        ar & addr.msb & addr.lsb;
    }
}}

#endif
