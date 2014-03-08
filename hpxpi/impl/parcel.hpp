// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpxpi/xpi.h>

#include <string>
#include <vector>
#include <stack>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

namespace hpxpi
{
    struct parcel_frame
    {
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
        parcel() : records_(1) {}

        XPI_Addr get_target_address() const
        {
            return records_.front().get_target_address();
        }
        void set_target_address(XPI_Addr addr)
        {
            records_.front().set_target_address(addr);
        }

        std::string get_target_action() const
        {
            return records_.front().get_target_action();
        }
        void set_target_action(std::string const& action)
        {
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

        std::vector<uint8_t> const& get_environment_data() const
        {
            return records_.front().get_environment_data();
        }
        template <typename Iterator>
        void set_environment_data(Iterator begin, Iterator end)
        {
            records_.front().set_environment_data(begin, end);
        }

        void pop_frame()
        {
            records_.pop_front();
        }
        void push_frame()
        {
            records_.push_back(records_.front());
        }

        bool is_empty() const
        {
            return records_.empty();
        }

    private:
        std::vector<uint8_t> argument_data_;
        std::deque<parcel_frame> records_;

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
}
