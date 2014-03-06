// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <string>
#include <vector>
#include <stack>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "hpxpi/xpi.h"

struct parcel_frame{
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

struct parcel_struct{
    parcel_struct(): records( { parcel_frame() } ) {}
    std::vector<unsigned char> argument_data;
    std::deque<parcel_frame> records;

    XPI_Addr& addr();
    std::string& target_action();
    std::vector<unsigned char>& environment_data();

private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & argument_data & records;
    }
};
