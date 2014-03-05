//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "headers/lco.hpp"

using namespace std;
using namespace hpx;

xpi_future::xpi_future(size_t buffer_size): buffer(buffer_size){
    hpx_future=hpx_promise.get_future();
}

//Should this return a copy? How do we know how big?
void* xpi_future::value(){
    gotten=true;
    return hpx_future.get();
}

void xpi_future::trigger(void* data){
    //should we set the bufer? How big?
    hpx_promise.set_value(data);
}

size_t xpi_future::size(){
    return buffer.size();
}

bool xpi_future::had_get(){
    return gotten;
}
