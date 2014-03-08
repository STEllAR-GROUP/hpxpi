//  Copyright (c) 2013 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>

#include <hpxpi/xpi.h>
#include <hpxpi/impl/lco.hpp>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Add factory registration functionality.
HPX_REGISTER_COMPONENT_MODULE();

///////////////////////////////////////////////////////////////////////////////
HPX_REGISTER_MINIMAL_COMPONENT_FACTORY(hpxpi::custom_lco, hpxpi_custom_lco);

///////////////////////////////////////////////////////////////////////////////
// Serialization support for managed_accumulator actions.
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::get_size_action,
    custom_lco_get_size_action);
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::get_value_action,
    custom_lco_get_value_action);
HPX_REGISTER_ACTION(hpxpi::detail::custom_lco::set_value_action,
    custom_lco_set_value_action);
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
            if (0 == desc_.get_size)
                return 0;
            return desc_.get_size(lco_);
        }

        custom_lco::buffer_type custom_lco::get_value() const
        {
            if (0 == desc_.get_value)
                return buffer_type();

            had_get_value_ = true;
            boost::uint8_t const* data =
                reinterpret_cast<boost::uint8_t const*>(
                    desc_.get_value(lco_));

            return buffer_type(data, get_size(), buffer_type::copy);
        }

        void custom_lco::set_value(custom_lco::buffer_type data)
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

    ///////////////////////////////////////////////////////////////////////
    // Callback functions needed for a XPI future
    void* future_init (size_t size, size_t init_data_size, void const* const data)
    {
        if (0 != size)
            return new future(size, init_data_size, data);
        return new trigger(init_data_size != 0);
    }

    // FIXME: added destroy method
    void future_destroy (void* const lco)
    {
        delete reinterpret_cast<future_base*>(lco);
    }

    // Handles the XPI_LCO_TRIGGER action, and should update the LCO’s state.
    void future_trigger (void* const lco, void const* const data)
    {
        reinterpret_cast<future_base*>(lco)->set_value(data);
    }

    // Called to evaluate the LCO’s predicate. It should not change the
    // state of the LCO. The implementation may cache the result once it
    // returns true.
    bool future_eval (void const* const lco)
    {
        return reinterpret_cast<future_base const*>(lco)->eval();
    }

    // This should return the address of the computed value of the LCO. This
    // will only be called when eval has returned true, and should point to
    // memory of at least get_size bytes. The return address or value may be
    // cached by the implementation.
    void const* future_get_value (void const* const lco)
    {
        return reinterpret_cast<future_base const*>(lco)->get_value();
    }

    // This should return the size of the value of the LCO.
    size_t future_get_size (void const* const lco)
    {
        return reinterpret_cast<future_base const*>(lco)->get_size();
    }
}

