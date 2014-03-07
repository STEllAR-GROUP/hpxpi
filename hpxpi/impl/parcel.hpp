// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpxpi/xpi.h>

#include <string>
#include <vector>
#include <stack>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

namespace hpxpi
{
    struct parcel_frame
    {
        XPI_Addr addr;
        std::string target_action;
        std::vector<unsigned char> environment_data;

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int version){
            ar & addr & target_action & environment_data;
        }
    };

    struct parcel
    {
    public:
        parcel() : records(1) {}

        XPI_Addr addr() const;

        std::string& target_action();
        std::string const& target_action() const;

        std::vector<unsigned char>& environment_data();
        std::vector<unsigned char> const& environment_data() const;

        std::vector<unsigned char> argument_data;
        std::deque<parcel_frame> records;

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int version){
            ar & argument_data & records;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    inline bool is_parcel_valid(XPI_Parcel parcel)
    {
        return 0 != parcel.p;
    }

    inline hpxpi::parcel* get_parcel(XPI_Parcel parcel)
    {
        return reinterpret_cast<hpxpi::parcel*>(parcel.p);
    }
}
