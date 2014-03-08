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

        action_registry()
        {
            register_action(XPI_ACTION_NULL, "__XPI_ACTION_NULL__");
        }

        void register_action(XPI_Action action, std::string key)
        {
            mutex_type::scoped_lock l(mtx_);
            key_to_action[key] = action;
            action_to_key[action] = key;
        }

        XPI_Action get_action(std::string key) const
        {
            mutex_type::scoped_lock l(mtx_);
            std::map<std::string, XPI_Action>::const_iterator it =
                key_to_action.find(key);
            if (it == key_to_action.end()) {
                // FIXME: what do?
            }
            return it->second;
        }

        std::string get_key(XPI_Action action) const
        {
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
        void const* data = ps.get_argument_data();
        XPI_Action action = registry.get_action(ps.get_target_action());

        // Create thread struct
        hpxpi::thread new_thread(ps);
        ps.pop_frame();

        // activate future
        if (XPI_NULL != future)
            hpx::trigger_lco_event(hpxpi::get_id(future));

        // Pass new thread
        XPI_Err status = XPI_SUCCESS;
        {
            detail::thread_data reset(&new_thread);
            status = action(const_cast<void*>(data));
        }

        // Send continuation
        if (!ps.is_empty())
        {
            XPI_Parcel parcel;
            parcel.p = reinterpret_cast<intptr_t>(&ps);
            XPI_Parcel_send(parcel, XPI_NULL, XPI_NULL);
        }
        return status;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Will be called if parcel sent requires local confirmation
    void parcel_sent(XPI_Addr complete)
    {
        hpx::trigger_lco_event(hpxpi::get_id(complete));
    }
}

HPX_PLAIN_ACTION(hpxpi::receive_parcel, receive_parcel_action);

///////////////////////////////////////////////////////////////////////////////
extern "C"
{
    XPI_Parcel XPI_PARCEL_NULL = { 0 };

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_register_action_with_key(XPI_Action action, char const* key)
    {
        hpxpi::registry.register_action(action, key);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Parcel_create(XPI_Parcel* parcel)
    {
        if (0 == parcel)
            return XPI_ERR_BAD_ARG;

        XPI_Parcel new_parcel;
        new_parcel.p = reinterpret_cast<intptr_t>(new hpxpi::parcel);
        *parcel = new_parcel;
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_free(XPI_Parcel parcel)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = reinterpret_cast<hpxpi::parcel*>(parcel.p);
        delete ps;

        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_addr(XPI_Parcel parcel, XPI_Addr addr)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->set_target_address(addr);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_action(XPI_Parcel parcel, XPI_Action action)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == action)
            return XPI_ERR_BAD_ARG;

        hpxpi::get_parcel(parcel)->set_target_action(hpxpi::registry.get_key(action));
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_set_env(XPI_Parcel parcel, size_t bytes, void* data)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == bytes || 0 == data)
            return XPI_ERR_BAD_ARG;

        uint8_t* cast_data = static_cast<uint8_t*>(data);
        hpxpi::get_parcel(parcel)->set_environment_data(cast_data, cast_data + bytes);
        return XPI_SUCCESS;
    }

    // Should we be copying the data?
    XPI_Err XPI_Parcel_set_data(XPI_Parcel parcel, size_t bytes, void const* data)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;
        if (0 == bytes || 0 == data)
            return XPI_ERR_BAD_ARG;

        uint8_t const* cast_data = static_cast<uint8_t const*>(data);
        hpxpi::get_parcel(parcel)->set_argument_data(cast_data, cast_data + bytes);
        return XPI_SUCCESS;
    }

    XPI_Err XPI_Parcel_push(XPI_Parcel parcel)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->push_frame();
        return XPI_SUCCESS;
    }

    // What is this actually supposed to do?
    // Currently sync, future not used
    XPI_Err XPI_Parcel_pop(XPI_Parcel parcel, XPI_Addr complete)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->pop_frame();
        return XPI_SUCCESS;
    }

    // Send parcel
    XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr complete, XPI_Addr thread_id)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        if (ps->get_target_action() == hpxpi::registry.get_key(XPI_ACTION_NULL))
            return XPI_SUCCESS;

        if (XPI_NULL != complete)
        {
            hpx::apply_cb<receive_parcel_action>(
                hpxpi::get_id(ps->get_target_address()),
                hpx::util::bind(&hpxpi::parcel_sent, complete),
                *ps, thread_id);
        }
        else
        {
            hpx::apply<receive_parcel_action>(
                hpxpi::get_id(ps->get_target_address()), *ps, thread_id);
        }

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Parcel_apply(XPI_Addr target, XPI_Action action,
        size_t bytes, void const*data, XPI_Addr complete)
    {
        XPI_Err error = XPI_SUCCESS;

        if (XPI_NULL == target)
            target = hpxpi::from_id(hpx::find_here());

        // create a parcel
        XPI_Parcel parcel;
        error = XPI_Parcel_create(&parcel);
        if (error != XPI_SUCCESS) return error;

        // set target address
        error = XPI_Parcel_set_addr(parcel, target);
        if (error != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            return error;
        }

        // set action
        error = XPI_Parcel_set_action(parcel, action);
        if (error != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            return error;
        }

        // set argument data
        error = XPI_Parcel_set_data(parcel, bytes, data);
        if (error != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            return error;
        }

        // send parcel
        error = XPI_Parcel_send(parcel, complete, XPI_NULL);
        if (error != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            return error;
        }

        return XPI_Parcel_free(parcel);
    }

    XPI_Err XPI_Parcel_apply_sync(XPI_Addr target, XPI_Action action,
        size_t bytes, const void *data)
    {
        XPI_Err error = XPI_SUCCESS;

        // create completion future
        XPI_Addr complete = XPI_NULL;
        error = XPI_Process_future_new_sync(XPI_NULL, 1, 0,
            XPI_DISTRIBUTION_NULL, &complete);
        if (error != XPI_SUCCESS)
        {
            return error;
        }

        error = XPI_Parcel_apply(target, action, bytes, data, complete);
        if (error != XPI_SUCCESS)
        {
            XPI_LCO_free_sync(complete);
            return error;
        }

        // wait for the parcel to be sent
        error = XPI_Thread_wait(complete, 0);

        // free all resources
        XPI_LCO_free_sync(complete);

        return error;
    }
}
