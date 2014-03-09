//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>

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
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::get_value_continue_action,
    custom_lco_get_value_continue_action);
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::had_get_value_action,
    custom_lco_had_get_value_action);

///////////////////////////////////////////////////////////////////////////////
namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        void on_ready(hpx::unique_future<void> f, XPI_Addr cont)
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
            }
        }

        ///////////////////////////////////////////////////////////////////////
        size_t custom_lco::get_size() const
        {
            assert(0 != desc_.get_size);
            return desc_.get_size(lco_);
        }

        // Send stored continuation parcel on its way
        void custom_lco::trigger_continuation(XPI_Parcel cont, bool owns_parcel)
        {
            assert(0 != desc_.eval && desc_.eval(lco_));
            assert(0 != desc_.get_value);

            XPI_Parcel_set_data(cont, get_size(), desc_.get_value(lco_));

            XPI_Addr process = XPI_NULL;
            XPI_Addr complete = XPI_NULL;

            XPI_Thread_get_process_sync(XPI_Thread_get_self(), &process) != XPI_SUCCESS &&
            XPI_Process_future_new_sync(process, 1, 0, XPI_LOCAL, &complete) != XPI_SUCCESS &&
            XPI_Parcel_send(cont, complete, XPI_NULL) != XPI_SUCCESS &&
            XPI_Thread_wait(complete, 0, 0);

            XPI_LCO_free_sync(complete);
            if (owns_parcel)
                XPI_Parcel_free(cont);

            had_get_value_ = true;
        }

        // This implements the XPI get_value functionality which attaches a
        // continuation to the LCO and returns without suspension.
        void custom_lco::get_value_continue()
        {
            assert(0 != desc_.eval);

            if (!desc_.eval(lco_))
            {
                XPI_Parcel cont;
                if (XPI_Parcel_clone(XPI_Thread_get_cont(), &cont) != XPI_SUCCESS)
                {
                    future_.then(hpx::util::bind(
                        &custom_lco::trigger_continuation, this,
                        cont, true));
                }
            }
            else
            {
                trigger_continuation(XPI_Thread_get_cont(), false);
            }
        }

        // This is the 'conventional' HPX get_value function, will be never called
        custom_lco::buffer_type const& custom_lco::get_value(hpx::error_code &ec)
        {
            static buffer_type data;
            assert(false);
            return data;
        }

        void custom_lco::set_value(custom_lco::buffer_type&& data)
        {
            assert(0 == desc_.trigger && 0 == desc_.eval);

            desc_.trigger(lco_, data.data());
            if (desc_.eval(lco_))
            {
                // trigger continuations
                promise_.set_value();
            }
        }

        bool custom_lco::had_get_value() const
        {
            return had_get_value_;
        }
    }
}

