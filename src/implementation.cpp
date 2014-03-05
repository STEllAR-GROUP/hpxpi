//  Copyright (c) 2013 Hartmut Kaiser
//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#include <hpx/include/runtime.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/util.hpp>
#include <hpx/include/local_lcos.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/hpx_start.hpp>
#include <hpx/hpx_finalize.hpp>

#include <string>
#include <map>
#include <vector>
#include <stack>
using namespace std;

#include "headers/parcel.hpp"
#include "headers/thread.hpp"

namespace hpxpi
{
    int xpi_main_wrapper(int argc, char** argv, XPI_Err (*XPI_main_)(size_t, void**));
}

struct action_registry{
    typedef hpx::lcos::local::spinlock mutex_type;

    void register_action(XPI_Action action, string key){
        mutex_type::scoped_lock l(mtx_);
        key_to_action[key] = action;
        action_to_key[action] = key;
    }

    XPI_Action get_action(string key) const{
        mutex_type::scoped_lock l(mtx_);
        map<string, XPI_Action>::const_iterator it = key_to_action.find(key);
        if (it == key_to_action.end()) {
            // FIXME: what do?
        }
        return it->second;
    }

    string get_key(XPI_Action action) const{
        mutex_type::scoped_lock l(mtx_);
        map<XPI_Action, string>::const_iterator it = action_to_key.find(action);
        if (it == action_to_key.end()) {
            // FIXME: what do?
        }
        return it->second;
    }

    mutable mutex_type mtx_;
    map<string, XPI_Action> key_to_action;
    map<XPI_Action, string> action_to_key;
};
action_registry registry;

//XPI_Err receive_parcel(parcel_struct ps, intptr_t future){
//Forget future for now
XPI_Err receive_parcel(parcel_struct ps){
    void* data = static_cast<void*>(ps.argument_data.data());
    XPI_Action action = registry.get_action(ps.target_action());
    //Create thread struct
    thread_struct new_thread(ps);
    ps.records.pop();
    //Pass new thread
    hpx::threads::set_thread_data(
        hpx::threads::get_self_id(), reinterpret_cast<size_t>(&new_thread));
    XPI_Err status = action(data);
    // TODO: activate future
    //Send continuation
    if(!ps.records.empty()){
        XPI_Parcel parcel;
        parcel.p = reinterpret_cast<intptr_t>(&ps);
        XPI_Parcel_send(parcel,XPI_NULL);
    }
    return status;
}
HPX_PLAIN_ACTION(receive_parcel, recieve_parcel_action);
recieve_parcel_action parcel_reciever;

thread_struct* get_self_thread(){
    hpx::threads::thread_self* self=hpx::threads::get_self_ptr();
    thread_struct* ts=reinterpret_cast<thread_struct*>(
        hpx::threads::get_thread_data(hpx::threads::get_self_id()));
    return ts;
}

extern "C" {

    XPI_Addr XPI_NULL = {0,0};
    XPI_Action XPI_ACTION_NULL=NULL;

    // XPI_version queries the specification version number that the XPI
    // implementation conforms to.
    void XPI_version(size_t* major, size_t* minor, size_t* release)
    {
        if (major)
            *major = XPI_VERSION_MAJOR;
        if (minor)
            *minor = XPI_VERSION_MINOR;
        if (release)
            *release = XPI_VERSION_RELEASE;
    }

    XPI_Err XPI_register_action_with_key(XPI_Action action, char* key){
        registry.register_action(action, string(key));
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // XPI_init initializes the XPI runtime, using the passed arguments and the
    // environment. C applications usually simply forward pointers to argc, argv,
    // and envp (if available). XPI specific arguments are removed from argc and
    // argv, and the environment is updated as necessary.
    XPI_Err XPI_init(int* nargs, char*** args, char*** env)
    {
        // we don't do anything here
        return XPI_SUCCESS;
    }

    // XPI_run encapsulates the creation of the main process and its associated
    // initial thread, XPI_main. XPI_run also manages the future required for
    // XPI_main to return a value. This routine is synchronous, and will not return
    // until the main process terminates.
    XPI_Err XPI_run_internal(int argc, char** argv, int* result,
        XPI_Err (*XPI_main_)(size_t, void*[]))
    {
        try {
            using hpx::util::placeholders::_1;
            using hpx::util::placeholders::_2;

            int r = hpx::init(hpx::util::bind(hpxpi::xpi_main_wrapper, _1, _2, XPI_main_),
                "XPI application", argc, argv);

            if (result)
                *result = r;

            return XPI_SUCCESS;
        }
        catch (hpx::exception const&) {
            return XPI_ERR_ERROR;
        }
        catch (...) {
            return XPI_ERR_ERROR;
        }
    }

    // XPI_finalize terminates the execution of the XPI runtime, releasing resources
    // acquired in XPI_init and XPI_run.
    XPI_Err XPI_finalize()
    {
        // we don't do anything here
        return XPI_SUCCESS;
    }

    // XPI_abort unconditionally aborts the execution of an XPI application,
    // returning control to the XPI_run site with the result set to the passed
    // code, and optionally printing the message. This should clean up resources
    // associated with the running application.
    HPXPI_ATTRIBUTE_NORETURN void XPI_abort(int code, char* message)
    {
        hpx::terminate();
    }

    XPI_Err XPI_Parcel_create(XPI_Parcel* parcel){
        if (0 == parcel)
            return XPI_ERR_BAD_ARG;

        XPI_Parcel new_parcel;
        new_parcel.p = reinterpret_cast<intptr_t>(new parcel_struct);
        *parcel = new_parcel;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_free(XPI_Parcel parcel){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        delete ps;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_addr(XPI_Parcel parcel, XPI_Addr addr){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        ps->addr() = addr;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_action(XPI_Parcel parcel, XPI_Action action){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        ps->target_action() = registry.get_key(action);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_env(XPI_Parcel parcel, size_t bytes, void* data){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        unsigned char* cast_data = static_cast<unsigned char*>(data);
        (ps->environment_data()).assign(cast_data, cast_data+bytes);
        return XPI_SUCCESS;
    }

    // Should we be copying the data?
    XPI_Err XPI_Parcel_set_data(XPI_Parcel parcel, size_t bytes, void* data){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        unsigned char* cast_data = static_cast<unsigned char*>(data);
        (ps->argument_data).assign(cast_data, cast_data+bytes);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_push(XPI_Parcel parcel){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        ps->records.push(ps->records.top());
        return XPI_SUCCESS;
    }

    // What is this actually supposed to do?
    // Currently sync, future not used
    XPI_Err XPI_Parcel_pop(XPI_Parcel parcel, XPI_Addr complete){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        ps->records.pop();
        return XPI_SUCCESS;
    }

    // Local only for now since serialization isn't finished
    XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr future){
        if (0 == parcel.p)
            return XPI_ERR_INV_PARCEL;

        parcel_struct* ps = reinterpret_cast<parcel_struct*>(parcel.p);
        if(ps->target_action() == registry.get_key(XPI_ACTION_NULL)){
            return XPI_SUCCESS;
        }
        //hpx::async(recieve_parcel, ps, future.addr);
        hpx::async(receive_parcel, *ps);
        return XPI_SUCCESS;
    }

    void* XPI_Thread_get_env(){
        thread_struct* self=get_self_thread();
        return &(self->environment_data[0]);
    }

}

