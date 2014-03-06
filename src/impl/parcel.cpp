//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>
#include <hpxpi/impl/parcel.hpp>

namespace hpxpi
{
    XPI_Addr parcel::addr() const
    {
        return records.front().addr;
    }

    std::string& parcel::target_action()
    {
        return records.front().target_action;
    }
    std::string const& parcel::target_action() const
    {
        return records.front().target_action;
    }

    std::vector<unsigned char>& parcel::environment_data()
    {
        return records.front().environment_data;
    }
    std::vector<unsigned char> const& parcel::environment_data() const
    {
        return records.front().environment_data;
    }
}
