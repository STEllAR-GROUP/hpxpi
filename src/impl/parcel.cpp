//  Copyright (c) 2013-2014 Alexander Duchene
//  Copyright (c) 2014 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/parcel.hpp>
#include <hpxpi/impl/thread.hpp>
#include <hpxpi/impl/xpi_addr.hpp>

#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
HPX_REGISTER_PLAIN_ACTION(receive_parcel_action);
HPX_REGISTER_PLAIN_ACTION(receive_parcel_direct_action);

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
    void parcel_sent(hpxpi::parcel* ps, XPI_Addr complete)
    {
        if (XPI_NULL != complete)
            hpxpi::trigger_lco_event(complete);
        intrusive_ptr_release(ps);
    }

    ///////////////////////////////////////////////////////////////////////////
    void apply_parcel(hpx::id_type const& targetid, XPI_Parcel parcel,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id)
    {
        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        intrusive_ptr_add_ref(ps);      // keep parcel alive while sending

        if (hpxpi::registry_is_direct_action(action))
        {
            receive_parcel_direct_action act;
            hpx::apply_cb(act, targetid,
                hpx::util::bind(&hpxpi::parcel_sent, ps, complete),
                *ps, thread_id);
        }
        else
        {
            receive_parcel_action act;
            hpx::apply_cb(act, targetid,
                hpx::util::bind(&hpxpi::parcel_sent, ps, complete),
                *ps, thread_id);
        }
    }

    void apply_parcel_colocated(hpx::id_type const& targetid, XPI_Parcel parcel,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id)
    {
        hpxpi::parcel* ps = hpxpi::get_parcel(parcel);
        intrusive_ptr_add_ref(ps);      // keep parcel alive while sending

        if (hpxpi::registry_is_direct_action(action))
        {
            receive_parcel_direct_action act;
            hpx::apply_colocated_cb(act, targetid,
                hpx::util::bind(&hpxpi::parcel_sent, ps, complete),
                *ps, thread_id);
        }
        else
        {
            receive_parcel_action act;
            hpx::apply_colocated_cb(act, targetid,
                hpx::util::bind(&hpxpi::parcel_sent, ps, complete),
                *ps, thread_id);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    void intrusive_ptr_add_ref(parcel* p)
    {
        ++p->count_;
    }

    void intrusive_ptr_release(parcel* p)
    {
        if (0 == --p->count_)
            delete p;
    }
}
