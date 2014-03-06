//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/parcel.hpp>
#include <hpxpi/impl/thread.hpp>

#include <hpxpi/impl/xpi_addr.hpp>

#include <map>
#include <string>

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
   namespace detail
    {
        struct thread_data
        {
            thread_data(thread* new_thread)
            {
                hpx::threads::set_thread_data(
                    hpx::threads::get_self_id(),
                    reinterpret_cast<size_t>(new_thread));
            }
            ~thread_data()
            {
                hpx::threads::set_thread_data(
                    hpx::threads::get_self_id(), 0);
            }
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    struct action_registry
    {
        typedef hpx::lcos::local::spinlock mutex_type;

        void register_action(XPI_Action action, std::string key){
            mutex_type::scoped_lock l(mtx_);
            key_to_action[key] = action;
            action_to_key[action] = key;
        }

        XPI_Action get_action(std::string key) const{
            mutex_type::scoped_lock l(mtx_);
            std::map<std::string, XPI_Action>::const_iterator it =
                key_to_action.find(key);
            if (it == key_to_action.end()) {
                // FIXME: what do?
            }
            return it->second;
        }

        std::string get_key(XPI_Action action) const{
            mutex_type::scoped_lock l(mtx_);
            std::map<XPI_Action, std::string>::const_iterator it =
                action_to_key.find(action);
            if (it == action_to_key.end()) {
                // FIXME: what do?
            }
            return it->second;
        }

    private:
        mutable mutex_type mtx_;
        std::map<std::string, XPI_Action> key_to_action;
        std::map<XPI_Action, std::string> action_to_key;
    };

    action_registry registry;

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err receive_parcel(parcel ps, XPI_Addr future)
    {
        void* data = static_cast<void*>(ps.argument_data.data());
        XPI_Action action = registry.get_action(ps.target_action());

        // Create thread struct
        hpxpi::thread new_thread(ps);
        ps.records.pop_front();

        // activate future
        if (XPI_NULL != future) {
            hpx::trigger_lco_event(hpxpi::get_id(future));
        }

        // Pass new thread
        XPI_Err status = XPI_SUCCESS;
        {
            detail::thread_data reset(&new_thread);
            status = action(data);
        }

        // Send continuation
        if(!ps.records.empty()){
            XPI_Parcel parcel;
            parcel.p = reinterpret_cast<intptr_t>(&ps);
            XPI_Parcel_send(parcel, XPI_NULL, XPI_NULL);
        }
        return status;
    }
}

HPX_PLAIN_ACTION(hpxpi::receive_parcel, recieve_parcel_action);

namespace hpxpi
{
    recieve_parcel_action parcel_receiver;
}

///////////////////////////////////////////////////////////////////////////////
extern "C"
{
    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_register_action_with_key(XPI_Action action, char* key)
    {
        hpxpi::registry.register_action(action, const_cast<char const*>(key));
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Parcel_create(XPI_Parcel* parcel){
        if (0 == parcel)
            return XPI_ERR_BAD_ARG;

        XPI_Parcel new_parcel;
        new_parcel.p = reinterpret_cast<intptr_t>(new hpxpi::parcel);
        *parcel = new_parcel;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_free(XPI_Parcel parcel){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = reinterpret_cast<hpxpi::parcel*>(parcel.p);
        delete ps;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_addr(XPI_Parcel parcel, XPI_Addr addr){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->addr() = addr;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_action(XPI_Parcel parcel, XPI_Action action){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == action)
            return XPI_ERR_BAD_ARG;

        hpxpi::get_parcel(parcel)->target_action() = hpxpi::registry.get_key(action);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_env(XPI_Parcel parcel, size_t bytes, void* data){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == bytes || 0 == data)
            return XPI_ERR_BAD_ARG;

        unsigned char* cast_data = static_cast<unsigned char*>(data);
        (hpxpi::get_parcel(parcel)->environment_data()).assign(cast_data, cast_data+bytes);
        return XPI_SUCCESS;
    }

    // Should we be copying the data?
    XPI_Err XPI_Parcel_set_data(XPI_Parcel parcel, size_t bytes, void* data){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == bytes || 0 == data)
            return XPI_ERR_BAD_ARG;

        unsigned char* cast_data = static_cast<unsigned char*>(data);
        (hpxpi::get_parcel(parcel)->argument_data).assign(cast_data, cast_data+bytes);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_push(XPI_Parcel parcel){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        ps->records.push_back(ps->records.front());
        return XPI_SUCCESS;
    }

    // What is this actually supposed to do?
    // Currently sync, future not used
    XPI_Err XPI_Parcel_pop(XPI_Parcel parcel, XPI_Addr complete){
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->records.pop_front();
        return XPI_SUCCESS;
    }

    // Local only for now since serialization isn't finished
    XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr complete, XPI_Addr thread_id)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        if(ps->target_action() == hpxpi::registry.get_key(XPI_ACTION_NULL)){
            return XPI_SUCCESS;
        }

        hpx::apply(&hpxpi::receive_parcel, *ps, thread_id);
        if (XPI_NULL != complete) {
            hpx::trigger_lco_event(hpxpi::get_id(complete));
        }
        return XPI_SUCCESS;
    }
}
