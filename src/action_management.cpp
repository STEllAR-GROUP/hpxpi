//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <map>
#include <string>
using namespace std;

map<string, XPI_Action>& action_registry(){
    static map<string, XPI_Action> registry;
    return registry;
}

extern "C" {
    XPI_Err XPI_register_action_with_key(XPI_Action action, char* key){
        action_registry()[string(key)] = action;
        return XPI_SUCCESS;
    }
}

