// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpxpi/xpi.h>
#include <hpxpi/impl/addr.hpp>

#include <string>
#include <vector>
#include <stack>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

#define HPXPI_REGISTER_ACTION(action)                                         \
    hpxpi::registry_register_action(                                          \
        reinterpret_cast<XPI_Action>(action), BOOST_PP_STRINGIZE(action))     \
/**/

#define HPXPI_REGISTER_DIRECT_ACTION(action)                                  \
    hpxpi::registry_register_direct_action(                                   \
        reinterpret_cast<XPI_Action>(action), BOOST_PP_STRINGIZE(action))     \
/**/

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    std::string registry_get_key(XPI_Action);
    XPI_Action registry_get_action(std::string const&);
    bool registry_is_direct_action(std::string const& key);

    void registry_register_action(XPI_Action action, std::string const& key);
    void registry_register_direct_action(XPI_Action action, std::string const& key);

    ///////////////////////////////////////////////////////////////////////////
    struct parcel_frame
    {
        parcel_frame()
          : addr_(XPI_NULL),
            target_action_("XPI_ACTION_NULL")
        {}

        XPI_Addr get_target_address() const
        {
            return addr_;
        }
        void set_target_address(XPI_Addr addr)
        {
            addr_ = addr;
        }

        std::string get_target_action() const
        {
            return target_action_;
        }
        void set_target_action(std::string const& action)
        {
            target_action_ = action;
        }

        std::vector<uint8_t> const& get_environment_data() const
        {
            return environment_data_;
        }
        template <typename Iterator>
        void set_environment_data(Iterator begin, Iterator end)
        {
            environment_data_.resize(0);
            environment_data_.insert(environment_data_.begin(), begin, end);
        }

        bool is_empty() const
        {
            return target_action_.empty() || target_action_ == "XPI_ACTION_NULL";
        }

    private:
        XPI_Addr addr_;
        std::string target_action_;
        std::vector<uint8_t> environment_data_;

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & addr_ & target_action_ & environment_data_;
        }
    };

    struct parcel
    {
    public:
        parcel()
          : records_(1), count_(1)
        {}

        // this intentionally creates a deep copy
        parcel(parcel const& rhs)
          : argument_data_(rhs.argument_data_),
            records_(rhs.records_),
            count_(1)
        {}

        XPI_Addr get_target_address() const
        {
            assert(!records_.empty());
            return records_.front().get_target_address();
        }
        void set_target_address(XPI_Addr addr)
        {
            assert(!records_.empty());
            records_.front().set_target_address(addr);
        }

        std::string get_target_action() const
        {
            assert(!records_.empty());
            return records_.front().get_target_action();
        }
        void set_target_action(std::string const& action)
        {
            assert(!records_.empty());
            records_.front().set_target_action(action);
        }

        void const* get_argument_data() const
        {
            return argument_data_.data();
        }
        template <typename Iterator>
        void set_argument_data(Iterator begin, Iterator end)
        {
            argument_data_.resize(0);
            argument_data_.insert(argument_data_.begin(), begin, end);
        }
        void reset_argument_data()
        {
            argument_data_.resize(0);
        }

        std::vector<uint8_t> const& get_environment_data() const
        {
            assert(!records_.empty());
            return records_.front().get_environment_data();
        }
        template <typename Iterator>
        void set_environment_data(Iterator begin, Iterator end)
        {
            assert(!records_.empty());
            records_.front().set_environment_data(begin, end);
        }

        void pop_frame()
        {
            assert(!records_.empty());
            records_.pop_front();
            if (records_.empty())
                records_.push_front(parcel_frame());
        }
        void push_frame()
        {
            records_.push_front(records_.front());
        }

        bool is_empty() const
        {
            return records_.empty() || records_.front().is_empty();
        }

        // reference counting
        friend void intrusive_ptr_add_ref(parcel* p);
        friend void intrusive_ptr_release(parcel* p);

    private:
        std::vector<uint8_t> argument_data_;
        std::list<parcel_frame> records_;

        boost::atomic_int count_;

    private:
        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & argument_data_ & records_;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    inline bool is_parcel_valid(XPI_Parcel parcel)
    {
        return 0 != parcel.p;
    }

    inline hpxpi::parcel* get_parcel(XPI_Parcel parcel)
    {
        return reinterpret_cast<hpxpi::parcel*>(parcel.p);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Main action entry point
    XPI_Err receive_parcel(parcel ps, XPI_Addr future);

    HPX_DEFINE_PLAIN_ACTION(receive_parcel, receive_parcel_action);
    HPX_DEFINE_PLAIN_DIRECT_ACTION(receive_parcel, receive_parcel_direct_action);

    ///////////////////////////////////////////////////////////////////////////
    void apply_parcel(hpx::id_type const& targetid, XPI_Parcel p,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id);

    void apply_parcel_colocated(hpx::id_type const& targetid, XPI_Parcel p,
        std::string const& action, XPI_Addr complete, XPI_Addr thread_id);
}

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct action_is_target_valid<hpxpi::receive_parcel_action>
    {
        static bool call(naming::id_type const& id)
        {
            return hpx::naming::is_locality(id) ||
                   hpx::naming::refers_to_virtual_memory(id.get_gid());
        }
    };

    template <>
    struct action_is_target_valid<hpxpi::receive_parcel_direct_action>
    {
        static bool call(naming::id_type const& id)
        {
            return hpx::naming::is_locality(id) ||
                   hpx::naming::refers_to_virtual_memory(id.get_gid());
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct component_type_is_compatible<
        hpxpi::receive_parcel_action::component_type>
    {
        static bool call(naming::address const& addr)
        {
            return
                addr.type_ == hpx::components::component_memory ||
                components::types_are_compatible(
                    addr.type_, components::get_component_type<
                        hpxpi::receive_parcel_action::component_type>());
        }
    };

    template <>
    struct component_type_is_compatible<
        hpxpi::receive_parcel_direct_action::component_type>
    {
        static bool call(naming::address const& addr)
        {
            return
                addr.type_ == hpx::components::component_memory ||
                components::types_are_compatible(
                    addr.type_, components::get_component_type<
                        hpxpi::receive_parcel_direct_action::component_type>());
        }
    };
}}

