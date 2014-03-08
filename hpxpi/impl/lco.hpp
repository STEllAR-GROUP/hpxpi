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

    ///////////////////////////////////////////////////////////////////////////
    struct future_base
    {
        virtual ~future_base() {}

        virtual size_t get_size() const = 0;
        virtual void set_value(void const* data) = 0;
        virtual bool eval() const = 0;
        virtual void const* get_value() const = 0;
    };

    struct future : future_base
    {
        typedef std::vector<uint8_t> value_type;

        future(size_t size, size_t init_data_size, void const* data)
          : size_(size)
        {
            if (0 != init_data_size && 0 != data)
            {
                uint8_t const* d = reinterpret_cast<uint8_t const*>(data);
                data_ = value_type(d, d + init_data_size);
                if (size > init_data_size)
                    data_.resize(size_);
                future_ = hpx::make_ready_future(boost::ref(data_));
            }
            else
            {
                future_ = promise_.get_future();
            }
        }

        size_t get_size() const
        {
            return size_;
        }

        void set_value(void const* data)
        {
            uint8_t const* d = reinterpret_cast<uint8_t const*>(data);
            data_ = value_type(d, d + size_);
            promise_.set_value(data_);
        }

        bool eval() const
        {
            return future_.is_ready();
        }

        void const* get_value() const
        {
            return data_.data();
        }

    private:
        size_t size_;
        value_type data_;
        hpx::lcos::local::promise<value_type&> promise_;
        hpx::unique_future<value_type&> future_;
    };

    ///////////////////////////////////////////////////////////////////////////
    struct trigger : future_base
    {
        trigger(bool ready)
        {
            if (ready)
            {
                future_ = hpx::make_ready_future();
            }
            else
            {
                future_ = promise_.get_future();
            }
        }

        size_t get_size() const
        {
            return 0;
        }

        void set_value(void const*)
        {
            promise_.set_value();
        }

        bool eval() const
        {
            return future_.is_ready();
        }

        void const* get_value() const
        {
            return 0;
        }

        hpx::lcos::local::promise<void> promise_;
        hpx::unique_future<void> future_;
    };

    ///////////////////////////////////////////////////////////////////////
    // Callback functions needed for a XPI future
    void* future_init (size_t size, size_t init_size, void const* const data);

    // FIXME: added destroy method
    void future_destroy (void* const lco);

    // Handles the XPI_LCO_TRIGGER action, and should update the LCO’s state.
    void future_trigger (void* const lco, void const* const data);

    // Called to evaluate the LCO’s predicate. It should not change the
    // state of the LCO. The implementation may cache the result once it
    // returns true.
    bool future_eval (void const* const lco);

    // This should return the address of the computed value of the LCO. This
    // will only be called when eval has returned true, and should point to
    // memory of at least get_size bytes. The return address or value may be
    // cached by the implementation.
    void const* future_get_value (void const* const lco);

    // This should return the size of the value of the LCO.
    size_t future_get_size (void const* const lco);

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        class custom_lco
          : public hpx::components::managed_component_base<custom_lco>
        {
        public:
            typedef hpx::util::serialize_buffer<uint8_t> buffer_type;

            custom_lco()
              : desc_({ 0, 0, 0, 0, 0, 0 }),
                lco_(0),
                had_get_value_(false)
            {}

            void init(XPI_LCO_Descriptor desc, std::size_t size,
                std::size_t init_data_size, void const* const init_data)
            {
                desc_ = desc;
                if (0 != desc.init)
                {
                    lco_ = desc.init(size, init_data_size, init_data);
                }
            }

            void finalize()
            {
                if (0 != desc_.destroy)
                    desc_.destroy(lco_);
                lco_ = 0;
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
            void* lco_;
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
