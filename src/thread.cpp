//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "headers/thread.hpp"

using namespace std;

thread_struct::thread_struct(parcel_struct& creator):
    continuation(creator),
    addr(creator.addr()),
    target_action(creator.target_action()),
    environment_data(creator.environment_data())
{
}
