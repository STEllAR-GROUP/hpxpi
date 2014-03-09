//  Copyright (c) 2014 Hartmut Kaiser
//  Copyright (c) 2014 Luke K. D'Allesandro
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/parcel.hpp>
#include <hpxpi/impl/thread.hpp>
#include <hpxpi/impl/lco.hpp>

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
        typedef std::map<std::string, std::pair<XPI_Action, bool> > key_to_action_type;
        typedef std::map<XPI_Action, std::string> action_to_key_type;

        action_registry()
        {
            HPXPI_REGISTER_ACTION(XPI_ACTION_NULL);
            register_lco_actions();
        }

        void register_action(XPI_Action action, std::string key, bool direct)
        {
            mutex_type::scoped_lock l(mtx_);
            key_to_action_[key] = std::make_pair(action, direct);
            action_to_key_[action] = key;
        }

        XPI_Action get_action(std::string key) const
        {
            mutex_type::scoped_lock l(mtx_);
            key_to_action_type::const_iterator it = key_to_action_.find(key);
            if (it == key_to_action_.end()) {
                // FIXME: what do?
            }
            return it->second.first;
        }

        bool is_direct_action(std::string key) const
        {
            mutex_type::scoped_lock l(mtx_);
            key_to_action_type::const_iterator it = key_to_action_.find(key);
            if (it == key_to_action_.end()) {
                // FIXME: what do?
            }
            return it->second.second;
        }

        std::string get_key(XPI_Action action) const
        {
            mutex_type::scoped_lock l(mtx_);
            action_to_key_type::const_iterator it = action_to_key_.find(action);
            if (it == action_to_key_.end()) {
                // FIXME: what do?
            }
            return it->second;
        }

    private:
        mutable mutex_type mtx_;
        key_to_action_type key_to_action_;
        action_to_key_type action_to_key_;
    };

    action_registry registry;

    ///////////////////////////////////////////////////////////////////////////
    std::string registry_get_key(XPI_Action action)
    {
        return registry.get_key(action);
    }

    XPI_Action registry_get_action(std::string const& key)
    {
        return registry.get_action(key);
    }

    bool registry_is_direct_action(std::string const& key)
    {
        return registry.is_direct_action(key);
    }

    void registry_register_action(XPI_Action action, std::string const& key)
    {
        registry.register_action(action, key, false);
    }

    void registry_register_direct_action(XPI_Action action, std::string const& key)
    {
        registry.register_action(action, key, true);
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err receive_parcel(parcel ps, XPI_Addr future)
    {
        void const* data = ps.get_argument_data();
        XPI_Action action = registry_get_action(ps.get_target_action());

        // Create thread struct, it modifies ps to refer to this thread's
        // continuation parcel only.
        hpxpi::thread t(ps);

        // activate future
        if (XPI_NULL != future)
            hpxpi::set_lco_value(future, t.get_thread_id());

        // Pass new thread
        XPI_Err status = XPI_SUCCESS;
        {
            detail::thread_data reset(&t);
            status = action(const_cast<void*>(data));
        }

        // Send continuation
        if (XPI_SUCCESS == status && !ps.is_empty())
        {
            XPI_Parcel parcel = { reinterpret_cast<intptr_t>(&ps) };
            XPI_Parcel_send(parcel, XPI_NULL, XPI_NULL);
        }

        return status;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Will be called if parcel-send requires local confirmation
    void parcel_sent(XPI_Addr complete)
    {
        hpx::trigger_lco_event(hpxpi::get_id(complete));
    }
}

HPX_PLAIN_ACTION(hpxpi::receive_parcel, receive_parcel_action);
HPX_PLAIN_DIRECT_ACTION(hpxpi::receive_parcel, receive_parcel_direct_action);

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    void apply_parcel(hpx::id_type const& targetid, parcel& ps,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id)
    {
        if (hpxpi::registry_is_direct_action(action))
        {
            receive_parcel_direct_action act;
            if (XPI_NULL != complete)
            {
                hpx::apply_cb(act, targetid,
                    hpx::util::bind(&hpxpi::parcel_sent, complete),
                    ps, thread_id);
            }
            else
            {
                hpx::apply(act, targetid, ps, thread_id);
            }
        }
        else
        {
            receive_parcel_action act;
            if (XPI_NULL != complete)
            {
                hpx::apply_cb(act, targetid,
                    hpx::util::bind(&hpxpi::parcel_sent, complete),
                    ps, thread_id);
            }
            else
            {
                hpx::apply(act, targetid, ps, thread_id);
            }
        }
    }

    void apply_parcel_colocated(hpx::id_type const& targetid, parcel& ps,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id)
    {
        if (hpxpi::registry_is_direct_action(action))
        {
            receive_parcel_direct_action act;
            if (XPI_NULL != complete)
            {
                hpx::apply_colocated_cb(act, targetid,
                    hpx::util::bind(&hpxpi::parcel_sent, complete),
                    ps, thread_id);
            }
            else
            {
                hpx::apply_colocated(act, targetid, ps, thread_id);
            }
        }
        else
        {
            receive_parcel_action act;
            if (XPI_NULL != complete)
            {
                hpx::apply_cb(act, targetid,
                    hpx::util::bind(&hpxpi::parcel_sent, complete),
                    ps, thread_id);
            }
            else
            {
                hpx::apply(act, targetid, ps, thread_id);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
extern "C"
{
    XPI_Parcel XPI_PARCEL_NULL = { 0 };

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_register_action_with_key(XPI_Action action, char const* key)
    {
        hpxpi::registry_register_action(action, key);
        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    XPI_Err XPI_Parcel_create(XPI_Parcel* parcel)
    {
        if (0 == parcel)
            return XPI_ERR_BAD_ARG;

        XPI_Parcel new_parcel { reinterpret_cast<intptr_t>(new hpxpi::parcel) };
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

        hpxpi::get_parcel(parcel)->set_target_action(hpxpi::registry_get_key(action));
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

    // FIXME: What is this actually supposed to do?
    //        Currently sync, future not used
    XPI_Err XPI_Parcel_pop(XPI_Parcel parcel, XPI_Addr complete)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::get_parcel(parcel)->pop_frame();
        return XPI_SUCCESS;
    }

    // Send parcel
    XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr complete,
        XPI_Addr thread_id)
    {
        if (!hpxpi::is_parcel_valid(parcel))
            return XPI_ERR_INV_PARCEL;

        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        std::string action(ps->get_target_action());
        if (action == "__XPI_ACTION_NULL__")
            return XPI_SUCCESS;

        XPI_Addr target = ps->get_target_address();
        hpx::id_type targetid = (XPI_NULL != target) ?
            hpxpi::get_id(target) : hpx::find_here();

        if (hpx::naming::is_locality(targetid))
        {
            hpxpi::apply_parcel(targetid, *ps, action, complete, thread_id);
        }
        else
        {
            hpxpi::apply_parcel_colocated(targetid, *ps, action, complete, thread_id);
        }

        return XPI_SUCCESS;
    }

    ///////////////////////////////////////////////////////////////////////////
    // FIXME: how can we avoid synchronizing locally and still delete the
    //        created parcel?
    XPI_Err XPI_Parcel_apply(XPI_Addr target, XPI_Action action,
        size_t data_bytes, void const*data, XPI_Addr cont)
    {
        XPI_Err error = XPI_SUCCESS;

        // get the current process
        XPI_Addr process = XPI_NULL;
        error = XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process);
        if (error != XPI_SUCCESS) return error;

        // create a parcel
        XPI_Parcel parcel;
        error = XPI_Parcel_create(&parcel);
        if (error != XPI_SUCCESS) return error;

        // if needed, set up the continuation that will trigger the allocated
        // future
        if (XPI_NULL != cont)
        {
            if ((error = XPI_Parcel_set_addr(parcel, cont)) != XPI_SUCCESS ||
                (error = XPI_Parcel_set_action(parcel, XPI_LCO_TRIGGER_ACTION)) != XPI_SUCCESS ||
                (error = XPI_Parcel_push(parcel)) != XPI_SUCCESS)
            {
                XPI_Parcel_free(parcel);
                return error;
            }
        }

        // set up the primary action the user wants, and copy in the user's
        // argument data
        if (XPI_NULL == target)
            target = hpxpi::from_id(hpx::find_here());

        if ((error = XPI_Parcel_set_addr(parcel, target)) != XPI_SUCCESS ||
            (error = XPI_Parcel_set_action(parcel, action)) != XPI_SUCCESS ||
            (error = XPI_Parcel_set_data(parcel, data_bytes, data)) != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
        }

        // create completion future
        XPI_Addr complete = XPI_NULL;
        error = XPI_Process_future_new_sync(process, 1, 0, XPI_LOCAL, &complete);
        if (error != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            return error;
        }

        // send parcel
        error = XPI_Parcel_send(parcel, complete, XPI_NULL);
        if (error != XPI_SUCCESS)
        {
            XPI_LCO_free_sync(complete);
            XPI_Parcel_free(parcel);
            return error;
        }

        // wait for the parcel to be sent
        error = XPI_Thread_wait(complete, 0, 0);

        // free all resources
        XPI_LCO_free_sync(complete);
        XPI_Parcel_free(parcel);

        return error;
    }

    // Luke D'Allessandro posted this 'canonical' version of how this function
    // could be implemented:
    XPI_Err XPI_Parcel_apply_sync(XPI_Addr target, XPI_Action action,
        size_t data_bytes, void const* data, size_t result_bytes, void* result)
    {
        XPI_Err error = XPI_SUCCESS;

        // get the current process
        XPI_Addr process = XPI_NULL;
        error = XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process);
        if (error != XPI_SUCCESS) return error;

        // create completion future
        XPI_Addr complete = XPI_NULL;
        error = XPI_Process_future_new_sync(process, 1, result_bytes,
            XPI_LOCAL, &complete);
        if (error != XPI_SUCCESS) return error;

        // create a parcel
        XPI_Parcel parcel;
        error = XPI_Parcel_create(&parcel);
        if (error != XPI_SUCCESS)
        {
            XPI_LCO_free(complete, XPI_NULL);
            return error;
        }

        // set up the continuation that will trigger the allocated future
        if ((error = XPI_Parcel_set_addr(parcel, complete)) != XPI_SUCCESS ||
            (error = XPI_Parcel_set_action(parcel, XPI_LCO_TRIGGER_ACTION)) != XPI_SUCCESS ||
            (error = XPI_Parcel_push(parcel)) != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            XPI_LCO_free(complete, XPI_NULL);
            return error;
        }

        // set up the primary action the user wants, and copy in the user's
        // argument data
        if (XPI_NULL == target)
            target = hpxpi::from_id(hpx::find_here());

        if ((error = XPI_Parcel_set_addr(parcel, target)) != XPI_SUCCESS ||
            (error = XPI_Parcel_set_action(parcel, action)) != XPI_SUCCESS ||
            (error = XPI_Parcel_set_data(parcel, data_bytes, data)) != XPI_SUCCESS)
        {
            XPI_Parcel_free(parcel);
            XPI_LCO_free(complete, XPI_NULL);
        }

        // send parcel
        error = XPI_Parcel_send(parcel, XPI_NULL, XPI_NULL);
        if (error != XPI_SUCCESS)
        {
            XPI_LCO_free_sync(complete);
            XPI_Parcel_free(parcel);
            return error;
        }

        // wait for the future
        error = XPI_Thread_wait(complete, result_bytes, result);

        // free all resources
        XPI_LCO_free_sync(complete);
        XPI_Parcel_free(parcel);

        return error;
    }
}
