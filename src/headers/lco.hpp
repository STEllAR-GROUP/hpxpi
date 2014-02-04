// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <vector>

#include <boost/serialization/serialization.hpp>

#include <hpx/include/actions.hpp>

#include "hpxpi/xpi.h"

struct xpi_future{
    hpx::shared_future<void*> hpx_future;
    hpx::promise<void*> hpx_promise;
    vector<unsigned char> buffer;
    bool gotten;

    void* value();
    void trigger(void* data);
    size_t size();
    bool had_get();
};

extern "C"{

//Spec doesn't have void* argument required for action type?
XPI_Err XPI_LCO_GET_VALUE(void* nothing);

//How do we know how big the data is?
XPI_Err XPI_LCO_TRIGGER_ACTION(void* data);

//Spec doesn't have void* argument required for action type?
XPI_Err XPI_LCO_GET_SIZE_ACTION(void* nothing);

}
