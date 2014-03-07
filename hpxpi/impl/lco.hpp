// Copyright (c) 2014 Alexander Duchene
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.

#pragma once

#include <hpx/hpx_fwd.hpp>
#include <hpx/exception.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/util/serialize_buffer.hpp>

#include <boost/serialization/serialization.hpp>

#include <hpxpi/xpi.h>

#include <vector>

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T>
        void on_ready(hpx::unique_future<T> f, XPI_Addr cont)
        {
            hpx::error_code ec;
            T val = f.get(ec);
            if (!ec) {
                // Propagate value to given continuation.
                XPI_LCO_trigger_sync(cont, &val);
            }
            else {
                // FIXME: What should we do in this case? XPI does not have a
                //        way to propagate an error to the waiting LCO.
            }
        }

        void on_ready(hpx::unique_future<void> f, XPI_Addr cont);
    }

    template <typename T>
    void propagate(hpx::unique_future<T> f, XPI_Addr cont)
    {
        using hpx::util::placeholders::_1;
        f.then(hpx::util::bind(&detail::on_ready<T>, _1, cont));
    }

    inline void propagate(hpx::unique_future<void> f, XPI_Addr cont)
    {
        using hpx::util::placeholders::_1;
        f.then(hpx::util::bind(&detail::on_ready, _1, cont));
    }

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////////
        class custom_lco
          : public hpx::components::managed_component_base<custom_lco>
        {
        public:
            typedef hpx::util::serialize_buffer<boost::uint8_t> buffer_type;

            custom_lco()
              : desc_({ 0, 0, 0, 0, 0 }),
                had_get_value_(false)
            {}

            void init(XPI_LCO_Descriptor desc, std::size_t size,
                void const* const init_data)
            {
                desc_ = desc;
                if (0 != desc.init)
                    desc.init(this, size, init_data);
            }

            size_t get_size() const;
            buffer_type get_value() const;
            void set_value(buffer_type data);
            bool had_get_value() const;

            HPX_DEFINE_COMPONENT_CONST_ACTION(custom_lco, get_size,
                get_size_action);
            HPX_DEFINE_COMPONENT_CONST_ACTION(custom_lco, get_value,
                get_value_action);
            HPX_DEFINE_COMPONENT_ACTION(custom_lco, set_value,
                set_value_action);
            HPX_DEFINE_COMPONENT_CONST_ACTION(custom_lco, had_get_value,
                had_get_value_action);

        private:
            XPI_LCO_Descriptor desc_;
            mutable bool had_get_value_;
        };
    }

    typedef hpx::components::managed_component<detail::custom_lco> custom_lco;
}

HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::get_size_action, custom_lco_get_size_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::get_value_action, custom_lco_get_value_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::set_value_action, custom_lco_set_value_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::had_get_value_action, custom_lco_had_get_value_action);
