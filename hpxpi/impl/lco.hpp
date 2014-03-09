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
#include <hpxpi/impl/xpi_addr.hpp>

#include <vector>

namespace hpxpi
{
    ///////////////////////////////////////////////////////////////////////////
    void register_lco_actions();

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

    ///////////////////////////////////////////////////////////////////////////
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
    template <typename T>
    inline void set_lco_value(XPI_Addr lco, T const& data)
    {
        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;
        buffer_type buffer = buffer_type(reinterpret_cast<uint8_t const*>(&data),
            sizeof(T), buffer_type::copy);
        hpx::set_lco_value(get_id(lco), buffer);
    }

    inline void set_lco_value(XPI_Addr lco, size_t size, void const* data)
    {
        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;
        buffer_type buffer = buffer_type(reinterpret_cast<uint8_t const*>(data),
            size, buffer_type::copy);
        hpx::set_lco_value(get_id(lco), buffer);
    }

    template <typename T>
    inline void set_lco_value(XPI_Addr lco, T const& data, XPI_Addr cont)
    {
        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;
        buffer_type buffer = buffer_type(reinterpret_cast<uint8_t const*>(&data),
            sizeof(T), buffer_type::copy);
        hpx::set_lco_value(get_id(lco), buffer, get_id(cont));
    }

    inline void set_lco_value(XPI_Addr lco, size_t size, void const* data,
        XPI_Addr cont)
    {
        typedef hpx::util::serialize_buffer<uint8_t> buffer_type;
        buffer_type buffer = buffer_type(reinterpret_cast<uint8_t const*>(data),
            size, buffer_type::copy);
        hpx::set_lco_value(get_id(lco), buffer, get_id(cont));
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void trigger_lco_event(XPI_Addr lco)
    {
        hpx::trigger_lco_event(get_id(lco));
    }

    inline void trigger_lco_event(XPI_Addr lco, XPI_Addr cont)
    {
        hpx::trigger_lco_event(get_id(lco), get_id(cont));
    }

    ///////////////////////////////////////////////////////////////////////////
    struct future
    {
        typedef std::vector<uint8_t> value_type;

        future(size_t size, size_t init_data_size, void const* data)
          : size_(size), has_value_(false)
        {
            if (0 != init_data_size && 0 != data)
            {
                uint8_t const* d = reinterpret_cast<uint8_t const*>(data);
                data_ = value_type(d, d + init_data_size);
                if (size > init_data_size)
                    data_.resize(size_);
                has_value_ = true;
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
            has_value_ = true;
        }

        bool eval() const
        {
            return has_value_;
        }

        void const* get_value() const
        {
            assert(has_value_);
            return data_.data();
        }

    private:
        size_t size_;
        value_type data_;
        bool has_value_;
    };

    ///////////////////////////////////////////////////////////////////////////
    struct trigger
    {
        trigger(size_t size, size_t init_data_size, void const* data)
          : has_value_(0 != init_data_size)
        {
        }

        size_t get_size() const
        {
            return 0;
        }

        void set_value(void const*)
        {
            has_value_ = true;
        }

        bool eval() const
        {
            return has_value_;
        }

        void const* get_value() const
        {
            return 0;
        }

        bool has_value_;
    };

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        // Callback functions needed for an XPI LCO
        template <typename Implementation>
        struct lco_descriptor
        {
            static void* init (size_t size, size_t init_data_size,
                void const* const data)
            {
                return new Implementation(size, init_data_size, data);
            }

            // FIXME: added destroy method
            static void destroy (void* const lco)
            {
                delete reinterpret_cast<Implementation*>(lco);
            }

            // Handles the XPI_LCO_TRIGGER action, and should update the LCO’s state.
            static void trigger (void* const lco, void const* const data)
            {
                reinterpret_cast<Implementation*>(lco)->set_value(data);
            }

            // Called to evaluate the LCO’s predicate. It should not change the
            // state of the LCO. The implementation may cache the result once it
            // returns true.
            static bool eval (void const* const lco)
            {
                return reinterpret_cast<Implementation const*>(lco)->eval();
            }

            // This should return the address of the computed value of the LCO. This
            // will only be called when eval has returned true, and should point to
            // memory of at least get_size bytes. The return address or value may be
            // cached by the implementation.
            static void const* get_value (void const* const lco)
            {
                return reinterpret_cast<Implementation const*>(lco)->get_value();
            }

            // This should return the size of the value of the LCO.
            static size_t get_size (void const* const lco)
            {
                return reinterpret_cast<Implementation const*>(lco)->get_size();
            }

            static XPI_LCO_Descriptor handlers;

            static XPI_LCO_Descriptor get_handlers()
            {
                return handlers;
            }
        };

        template <typename Implementation>
        XPI_LCO_Descriptor lco_descriptor<Implementation>::handlers =
        {
            &lco_descriptor::init,
            &lco_descriptor::destroy,
            &lco_descriptor::trigger,
            &lco_descriptor::eval,
            &lco_descriptor::get_value,
            &lco_descriptor::get_size
        };

        ///////////////////////////////////////////////////////////////////////
        class custom_lco
          : public hpx::lcos::base_lco_with_value<
                hpx::util::serialize_buffer<uint8_t> >,
            public hpx::components::locking_hook<
                hpx::components::managed_component_base<custom_lco> >
        {
        private:
            typedef hpx::components::locking_hook<
                hpx::components::managed_component_base<custom_lco>
            > base_type;

        public:
            typedef hpx::lcos::base_lco_with_value<
                hpx::util::serialize_buffer<uint8_t>
            > base_lco_type;

            typedef hpx::util::serialize_buffer<uint8_t> buffer_type;

            typedef base_lco_type::base_type_holder base_type_holder;
            typedef base_type::wrapping_type wrapping_type;

            custom_lco()
              : desc_({ 0, 0, 0, 0, 0, 0 }),
                lco_(0),
                had_get_value_(false)
            {}

            void init(XPI_LCO_Descriptor desc, std::size_t size,
                std::size_t init_data_size, void const* const init_data)
            {
                desc_ = desc;
                assert(0 != desc.init);

                lco_ = desc.init(size, init_data_size, init_data);
                future_ = promise_.get_future().share();
            }

            void finalize()
            {
                assert(0 != desc_.destroy);
                desc_.destroy(lco_);

                lco_ = 0;
                base_type::finalize();
            }

            static hpx::components::component_type get_component_type()
            {
                return hpx::components::get_component_type<custom_lco>();
            }
            static void set_component_type(hpx::components::component_type type)
            {
                hpx::components::set_component_type<custom_lco>(type);
            }
            using base_type::wrap_action;

            size_t get_size() const;
            void get_value_continue();
            void set_value(buffer_type&& data);
            bool had_get_value() const;

            buffer_type const& get_value(hpx::error_code &ec);

            HPX_DEFINE_COMPONENT_CONST_DIRECT_ACTION(custom_lco,
                get_size, get_size_action);
            HPX_DEFINE_COMPONENT_DIRECT_ACTION(custom_lco,
                get_value_continue, get_value_continue_action);
            HPX_DEFINE_COMPONENT_CONST_DIRECT_ACTION(custom_lco,
                had_get_value, had_get_value_action);

        protected:
            void trigger_continuation(XPI_Parcel cont, bool owns_parcel);

        private:
            XPI_LCO_Descriptor desc_;
            void* lco_;
            bool had_get_value_;
            hpx::lcos::local::promise<void> promise_;
            hpx::shared_future<void> future_;
        };
    }

    typedef hpx::components::managed_component<detail::custom_lco> custom_lco;
}

HPX_REGISTER_BASE_LCO_WITH_VALUE_DECLARATION(
    hpx::util::serialize_buffer<uint8_t>, serialize_buffer_uin8_t_type)

HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::get_size_action,
    custom_lco_get_size_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::get_value_continue_action,
    custom_lco_get_value_continue_action);
HPX_REGISTER_ACTION_DECLARATION(
    hpxpi::detail::custom_lco::had_get_value_action,
    custom_lco_had_get_value_action);
