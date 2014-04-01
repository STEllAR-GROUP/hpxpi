//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>
#include <hpxpi/impl/parcel.hpp>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Add factory registration functionality for this module.
HPX_REGISTER_COMPONENT_MODULE();

///////////////////////////////////////////////////////////////////////////////
HPX_REGISTER_BASE_LCO_WITH_VALUE(hpx::util::serialize_buffer<uint8_t>,
    serialize_buffer_uin8_t_type)
HPX_REGISTER_DERIVED_COMPONENT_FACTORY(hpxpi::custom_lco, hpxpi_custom_lco,
    "serialize_buffer_uin8_t_type", hpx::components::factory_enabled)

HPX_DEFINE_GET_COMPONENT_TYPE(hpxpi::detail::custom_lco)

///////////////////////////////////////////////////////////////////////////////
// Serialization support for hpxpi::custom_lco actions.
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::get_size_action,
    custom_lco_get_size_action);
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::get_value_action,
    custom_lco_get_value_action);
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::had_get_value_action,
    custom_lco_had_get_value_action);

///////////////////////////////////////////////////////////////////////////////
namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    void register_lco_actions()
    {
        HPXPI_REGISTER_DIRECT_ACTION(XPI_LCO_TRIGGER_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_LCO_GET_SIZE_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_LCO_HAD_GET_VALUE_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_LCO_FREE_ACTION);
        HPXPI_REGISTER_DIRECT_ACTION(XPI_LCO_GET_VALUE_ACTION);
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        void on_ready_void(hpx::future<void> f, XPI_Addr cont)
        {
            hpx::error_code ec;
            f.get(ec);
            if (!ec) {
                // Propagate event to given continuation.
                // FIXME: Is this the right way to trigger an event?
                XPI_LCO_trigger_sync(cont, 0);
            }
            else {
                // FIXME: What should we do in this case? XPI does not have a
                //        way to propagate an error to the waiting LCO.
                assert(false);
            }
        }

        ///////////////////////////////////////////////////////////////////////
        size_t custom_lco::get_size() const
        {
            if (0 == desc_.get_size)
                return 0;
            return desc_.get_size(lco_);
        }

        custom_lco::buffer_type custom_lco::get_value_() const
        {
            return const_cast<custom_lco*>(this)->get_value(hpx::throws);
        }

        custom_lco::buffer_type const& custom_lco::get_value(hpx::error_code &ec)
        {
            if (0 == desc_.get_value)
                return data_;

            if (!had_get_value_)
            {
                had_get_value_ = true;
                boost::uint8_t* data = const_cast<uint8_t*>(
                    reinterpret_cast<boost::uint8_t const*>(
                        desc_.get_value(lco_)));
                data_ = buffer_type(data, get_size(), buffer_type::reference);
            }

            return data_;
        }

        void custom_lco::set_value(custom_lco::buffer_type&& data)
        {
            if (0 == desc_.trigger || 0 == desc_.eval)
                return;

            desc_.trigger(lco_, data.data());
            desc_.eval(lco_);
        }

        bool custom_lco::had_get_value() const
        {
            return had_get_value_;
        }
    }
}

