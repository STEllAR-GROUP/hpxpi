//  Copyright (c) 2013-2014 Hartmut Kaiser
//  Copyright (c) 2013-2014 Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_H_APR_27_2013_1135AM)
#define HPXPI_H_APR_27_2013_1135AM

#include <hpxpi/config/config.h>
#include <hpxpi/config/version.h>
#include <hpxpi/config/no_return.h>
#include <hpxpi/config/export_definitions.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Error Handling [3.1]
///////////////////////////////////////////////////////////////////////////////

typedef int XPI_Err;

///////////////////////////////////////////////////////////////////////////////
// Address Space Representation [5.4]
///////////////////////////////////////////////////////////////////////////////

// Implementation defined
//
// Corresponds to hpx gid_type
typedef struct XPI_Addr {
    uint64_t msb;
    uint64_t lsb;
} XPI_Addr;

HPXPI_EXPORT extern XPI_Addr XPI_NULL;

typedef struct XPI_AddrDiff {
    int64_t msb;
    uint64_t lsb;
} XPI_AddrDiff;

// initialize an address structure
HPXPI_EXPORT XPI_Err XPI_Addr_init(uint64_t address, XPI_Addr *result);

///////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown [3.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_main is not implemented by XPI. It merely describes the action that
// XPI applications are required to provide as the initial action for the
// main process.
XPI_Err XPI_main(size_t nargs, void* args[]);

// XPI_init initializes the XPI runtime, using the passed arguments and the
// environment. C applications usually simply forward pointers to argc, argv,
// and envp (if available). XPI specific arguments are removed from argc and
// argv, and the environment is updated as necessary.
HPXPI_EXPORT XPI_Err XPI_init(int* nargs, char*** args, char*** env);

// XPI_run encapsulates the creation of the main process and its associated
// initial thread, XPI_main. XPI_run also manages the future required for
// XPI_main to return a value. This routine is synchronous, and will not return
// until the main process terminates.

HPXPI_EXPORT XPI_Err XPI_run_internal(int argc, char** argv, int* result,
    XPI_Err (*XPI_main_)(size_t, void**));

inline XPI_Err XPI_run(int argc, char** argv, int* result)
{
    return XPI_run_internal(argc, argv, result, &XPI_main);
}

// XPI_finalize terminates the execution of the XPI runtime, releasing resources
// acquired in XPI_init and XPI_run.
HPXPI_EXPORT XPI_Err XPI_finalize();

///////////////////////////////////////////////////////////////////////////////
// High Level Interface Routines [3.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_version queries the specification version number that the XPI
// implementation conforms to.
HPXPI_EXPORT void XPI_version(size_t* major, size_t* minor, size_t* release);

///////////////////////////////////////////////////////////////////////////////
// Action Management [3.4]
///////////////////////////////////////////////////////////////////////////////
typedef XPI_Err (*XPI_Action)(void* args) /** CONT(...) **/;

//Register an action with the runtime, must be done at all localities
HPXPI_EXPORT XPI_Err XPI_register_action_with_key(XPI_Action action, const char* key);

#define XPI_register_action(act) XPI_register_action_with_key(act, #act)

///////////////////////////////////////////////////////////////////////////////
// Parcel Generation [4.1]
///////////////////////////////////////////////////////////////////////////////
typedef struct XPI_Parcel { intptr_t p; } XPI_Parcel;
HPXPI_EXPORT extern XPI_Parcel XPI_PARCEL_NULL;

// Create an empty parcel
HPXPI_EXPORT XPI_Err XPI_Parcel_create(XPI_Parcel* parcel);

// Clone the given parcel
HPXPI_EXPORT XPI_Err XPI_Parcel_clone(XPI_Parcel parcel, XPI_Parcel *clone);

// Free an existing parcel
HPXPI_EXPORT XPI_Err XPI_Parcel_free(XPI_Parcel parcel);

// Set where the parcel action is invoked
// Can be ignored except for LCO targets
// XPI_NULL specifies no preferred address
HPXPI_EXPORT XPI_Err XPI_Parcel_set_addr(XPI_Parcel parcel, XPI_Addr addr);

// Set the parcel action
HPXPI_EXPORT XPI_Err XPI_Parcel_set_action(XPI_Parcel parcel,
    XPI_Action action);

// Set the parcel environment data
HPXPI_EXPORT XPI_Err XPI_Parcel_set_env(XPI_Parcel parcel, size_t bytes,
    void* data);

// Set the parcel argument data
// FIXME: added const to last argument
HPXPI_EXPORT XPI_Err XPI_Parcel_set_data(XPI_Parcel parcel, size_t bytes,
    void const* data);

///////////////////////////////////////////////////////////////////////////////
// High-level function call interface (Apply) [4.5]
///////////////////////////////////////////////////////////////////////////////

HPXPI_EXPORT XPI_Err XPI_Parcel_apply(XPI_Addr target, XPI_Action action,
    size_t bytes, const void *data, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Parcel_apply_sync(XPI_Addr target, XPI_Action action,
    size_t data_bytes, void const* data, size_t result_bytes, void* result);

///////////////////////////////////////////////////////////////////////////////
// Continuation Stack Management [4.3]
///////////////////////////////////////////////////////////////////////////////

// Push a new record (target action, address, and environment) onto the stack
// New top is copy of previous top
HPXPI_EXPORT XPI_Err XPI_Parcel_push(XPI_Parcel parcel);

// Pop a closure off of continuation stack
// Done async, with complete indicating success
HPXPI_EXPORT XPI_Err XPI_Parcel_pop(XPI_Parcel parcel, XPI_Addr complete);

///////////////////////////////////////////////////////////////////////////////
// Sending a Parcel [4.4]
///////////////////////////////////////////////////////////////////////////////

// Sends a parcel, with future signaling completion
HPXPI_EXPORT XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr complete,
    XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// Native Parcel Interface [5.2]
///////////////////////////////////////////////////////////////////////////////
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_U8_ACTION(void* args);      // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_U16_ACTION(void* args);     // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_U32_ACTION(void* args);     // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_U64_ACTION(void* args);     // CONT()
// HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_U128_ACTION(void* args);    // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_S8_ACTION(void* args);      // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_S16_ACTION(void* args);     // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_S32_ACTION(void* args);     // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_S64_ACTION(void* args);     // CONT()
// HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_S128_ACTION(void* args);    // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_F_ACTION(void* args);       // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_D_ACTION(void* args);       // CONT()
// HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_FC_ACTION(void* args);      // CONT()
// HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_DC_ACTION(void* args);      // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_ADDR_ACTION(void* args);    // CONT()
HPXPI_EXPORT XPI_Err XPI_AGAS_STORE_ADDRDIFF_ACTION(void* args);// CONT()

HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_U8_ACTION(void* args);       // CONT(uint8_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_U16_ACTION(void* args);      // CONT(uint16_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_U32_ACTION(void* args);      // CONT(uint32_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_U64_ACTION(void* args);      // CONT(uint64_t val)
// HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_U128_ACTION(void* args);     // CONT(__uint128_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_S8_ACTION(void* args);       // CONT(int8_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_S16_ACTION(void* args);      // CONT(int16_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_S32_ACTION(void* args);      // CONT(int32_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_S64_ACTION(void* args);      // CONT(int64_t val)
// HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_S128_ACTION(void* args);     // CONT(__int128_t val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_F_ACTION(void* args);        // CONT(float val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_D_ACTION(void* args);        // CONT(double val)
// HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_FC_ACTION(void* args);       // CONT(float _Complex val)
// HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_DC_ACTION(void* args);       // CONT(double _Complex val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_ADDR_ACTION(void* args);     // CONT(XPI_Addr val)
HPXPI_EXPORT XPI_Err XPI_AGAS_LOAD_ADDRDIFF_ACTION(void* args); // CONT(XPI_AddrDiff val)

///////////////////////////////////////////////////////////////////////////////
// Asynchronous memory access [5.3.1]
///////////////////////////////////////////////////////////////////////////////
HPXPI_EXPORT XPI_Err XPI_Agas_store_u8(XPI_Addr addr, uint8_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u16(XPI_Addr addr, uint16_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u32(XPI_Addr addr, uint32_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u64(XPI_Addr addr, uint64_t val, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_u128(XPI_Addr addr, __uint128_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s8(XPI_Addr addr, int8_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s16(XPI_Addr addr, int16_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s32(XPI_Addr addr, int32_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s64(XPI_Addr addr, int64_t val, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_s128(XPI_Addr addr, __int128_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_f(XPI_Addr addr, float val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_d(XPI_Addr addr, double val, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_fc(XPI_Addr addr, float _Complex val, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_dc(XPI_Addr addr, double _Complex val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_addr(XPI_Addr addr, XPI_Addr val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store_addrdiff(XPI_Addr addr, XPI_AddrDiff val, XPI_Addr future);

HPXPI_EXPORT XPI_Err XPI_Agas_load_u8(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u16(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u32(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u64(XPI_Addr addr, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_u128(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s8(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s16(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s32(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s64(XPI_Addr addr, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_s128(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_f(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_d(XPI_Addr addr, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_fc(XPI_Addr addr, XPI_Addr future);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_dc(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_addr(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load_addrdiff(XPI_Addr addr, XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// Synchronous memory access [5.3.2]
///////////////////////////////////////////////////////////////////////////////
HPXPI_EXPORT XPI_Err XPI_Agas_store_u8_sync(XPI_Addr addr, uint8_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u16_sync(XPI_Addr addr, uint16_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u32_sync(XPI_Addr addr, uint32_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_u64_sync(XPI_Addr addr, uint64_t val);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_u128_sync(XPI_Addr addr, __uint128_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s8_sync(XPI_Addr addr, int8_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s16_sync(XPI_Addr addr, int16_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s32_sync(XPI_Addr addr, int32_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_s64_sync(XPI_Addr addr, int64_t val);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_s128_sync(XPI_Addr addr, __int128_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_f_sync(XPI_Addr addr, float val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_d_sync(XPI_Addr addr, double val);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_fc_sync(XPI_Addr addr, float _Complex val);
// HPXPI_EXPORT XPI_Err XPI_Agas_store_dc_sync(XPI_Addr addr, double _Complex val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_addr_sync(XPI_Addr addr, XPI_Addr val);
HPXPI_EXPORT XPI_Err XPI_Agas_store_addrdiff_sync(XPI_Addr addr, XPI_AddrDiff val);

HPXPI_EXPORT XPI_Err XPI_Agas_load_u8_sync(XPI_Addr addr, uint8_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u16_sync(XPI_Addr addr, uint16_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u32_sync(XPI_Addr addr, uint32_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_u64_sync(XPI_Addr addr, uint64_t *val);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_u128_sync(XPI_Addr addr, __uint128_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s8_sync(XPI_Addr addr, int8_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s16_sync(XPI_Addr addr, int16_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s32_sync(XPI_Addr addr, int32_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_s64_sync(XPI_Addr addr, int64_t *val);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_s128_sync(XPI_Addr addr, __int128_t *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_f_sync(XPI_Addr addr, float *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_d_sync(XPI_Addr addr, double *val);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_fc_sync(XPI_Addr addr, float _Complex *val);
// HPXPI_EXPORT XPI_Err XPI_Agas_load_dc_sync(XPI_Addr addr, double _Complex *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_addr_sync(XPI_Addr addr, XPI_Addr *val);
HPXPI_EXPORT XPI_Err XPI_Agas_load_addrdiff_sync(XPI_Addr addr, XPI_AddrDiff *val);

///////////////////////////////////////////////////////////////////////////////
// Threads Actions [6.1]
///////////////////////////////////////////////////////////////////////////////

// Indicates processing of parcel including continuations should stop
HPXPI_EXPORT extern XPI_Action XPI_ACTION_NULL;

///////////////////////////////////////////////////////////////////////////////
// Thread Instantiation [6.2]
///////////////////////////////////////////////////////////////////////////////

// Get own global address
HPXPI_EXPORT XPI_Addr XPI_Thread_get_self();

// Gets target address of instantiating parcel
HPXPI_EXPORT XPI_Addr XPI_Thread_get_addr();

// Gets the environment data of instantiating parcel
HPXPI_EXPORT void* XPI_Thread_get_env();

// Gets the handle for continuation parcel
HPXPI_EXPORT XPI_Parcel XPI_Thread_get_cont();

///////////////////////////////////////////////////////////////////////////////
// Continuing [6.3]
///////////////////////////////////////////////////////////////////////////////

// the continue primitive
// HPXPI_EXPORT void XPI_continue(size_t n, size_t sizes[], const void * vals[]);
HPXPI_EXPORT void XPI_continue1(size_t size, const void *val);

///////////////////////////////////////////////////////////////////////////////
// Thread Suspension [6.5]
///////////////////////////////////////////////////////////////////////////////

// This blocks execution until the LCO fires.
// FIXME: added parameter: size_t bytes
HPXPI_EXPORT XPI_Err XPI_Thread_wait(XPI_Addr lco, size_t bytes, void *value);

// This blocks until all of the LCOs in lcos have fired.
// FIXME: added parameter: size_t bytes[]
HPXPI_EXPORT XPI_Err XPI_Thread_wait_all(size_t n, XPI_Addr lco[],
    size_t bytes[], void* values[]);

///////////////////////////////////////////////////////////////////////////////
// Thread Resources [6.6]
///////////////////////////////////////////////////////////////////////////////

// HPXPI_EXPORT XPI_Err XPI_Thread_get_process(XPI_Addr address, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Thread_get_process_sync(XPI_Addr address,
    XPI_Addr *process);

///////////////////////////////////////////////////////////////////////////////
// LCOs: Common Interface [7.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_LCO_GET_VALUE_ACTION CONTINUE(value)
//   CONT value the LCO value: CONT(void *data)
HPXPI_EXPORT XPI_Err XPI_LCO_GET_VALUE_ACTION(void* args);

// XPI_LCO_TRIGGER_ACTION(value)
//  IN value (optional) an optional trigger value
HPXPI_EXPORT XPI_Err XPI_LCO_TRIGGER_ACTION(void* args);

HPXPI_EXPORT XPI_Err XPI_LCO_trigger(XPI_Addr lco, void const* data,
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_trigger_sync(XPI_Addr lco, void const* data);

// XPI_LCO_GET_SIZE_ACTION CONTINUE(size)
//  CONT size the size of the user-portion of the LCO: CONT(size_t size)
HPXPI_EXPORT XPI_Err XPI_LCO_GET_SIZE_ACTION(void* args);

HPXPI_EXPORT XPI_Err XPI_LCO_get_size(XPI_Addr lco, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_get_size_sync(XPI_Addr lco, size_t *size);

// XPI_LCO_HAD_GET_VALUE_ACTION CONTINUE(value)
//   CONT value true if any threads have performed XPI_LCO_GET_VALUE on the
//        target LCO: CONT(bool)
HPXPI_EXPORT XPI_Err XPI_LCO_HAD_GET_VALUE_ACTION(void* args);

HPXPI_EXPORT XPI_Err XPI_LCO_had_get_value(XPI_Addr lco, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_had_get_value_sync(XPI_Addr lco, bool *value);

// XPI_LCO_FREE_ACTION
HPXPI_EXPORT XPI_Err XPI_LCO_FREE_ACTION(void* args);

HPXPI_EXPORT XPI_Err XPI_LCO_free(XPI_Addr lco, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_free_sync(XPI_Addr lco);

///////////////////////////////////////////////////////////////////////////////
// Futures [7.3.1]
///////////////////////////////////////////////////////////////////////////////
typedef enum XPI_Distribution {
    XPI_LOCAL = 0,      // refer to current locality
    XPI_CYCLIC = 1      // try to cyclically use localities
} XPI_Distribution;

// HPXPI_EXPORT XPI_Err XPI_Process_Future_New(XPI_Addr process,
//     size_t count, size_t bytes, XPI_Distribution distribution,
//     XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_future_new_sync(XPI_Addr process,
    size_t count, size_t bytes, XPI_Distribution distribution,
    XPI_Addr *address);

///////////////////////////////////////////////////////////////////////////////
// User LCOs [7.4]
///////////////////////////////////////////////////////////////////////////////

// FIXME: const parameters do not make too much sense
typedef struct XPI_LCO_Descriptor {
    // Handles initialization of the LCO.
    // FIXME: changed return type
    // FIXME: added size, init_size arguments
    void* (*init) (size_t size, size_t init_size, void const* const data);
    // FIXME: added destroy method
    void (*destroy) (void* const lco);
    // Handles the XPI_LCO_TRIGGER action, and should update the LCO’s state.
    void (*trigger) (void* const lco, void const* const data);
    // Called to evaluate the LCO’s predicate. It should not change the
    // state of the LCO. The implementation may cache the result once it
    // returns true.
    bool (*eval) (void const* const lco);
    // This should return the address of the computed value of the LCO. This
    // will only be called when eval has returned true, and should point to
    // memory of at least get_size bytes. The return address or value may be
    // cached by the implementation.
    void const* (*get_value) (void const* const lco);
    // This should return the size of the value of the LCO.
    size_t (*get_size) (void const* const lco);
} XPI_LCO_Descriptor;

// HPXPI_EXPORT XPI_Err XPI_Process_lco_malloc(XPI_Addr process,
//     size_t count, size_t size, XPI_LCO_Descriptor handlers,
//     XPI_Distribution distribution, size_t init_data_size,
//     const void * const init_data, XPI_Addr future);

HPXPI_EXPORT XPI_Err XPI_Process_lco_malloc_sync(XPI_Addr process,
    size_t count, size_t size, XPI_LCO_Descriptor handlers,
    XPI_Distribution distribution, size_t init_data_size,
    const void * const init_data, XPI_Addr *address);

///////////////////////////////////////////////////////////////////////////////
// Memory Management [8.6.1]
///////////////////////////////////////////////////////////////////////////////
// HPXPI_EXPORT XPI_Err XPI_Process_global_malloc(XPI_Addr process,
//     size_t count, size_t size, XPI_Distribution distribution,
//     XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_global_malloc_sync(XPI_Addr process,
    size_t count, size_t size, XPI_Distribution distribution,
    XPI_Addr* address);

// HPXPI_EXPORT XPI_Err XPI_Process_global_free(XPI_Addr process,
//     XPI_Addr address, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_global_free_sync(XPI_Addr process,
    XPI_Addr address);

///////////////////////////////////////////////////////////////////////////////
// XPI Error Codes [Appendix B]
///////////////////////////////////////////////////////////////////////////////
XPI_Err const XPI_SUCCESS = 0;              // success
XPI_Err const XPI_ERR_ERROR = -1;           // generic error
XPI_Err const XPI_ERR_INV_TYPE = -2;        // the type is invalid
XPI_Err const XPI_ERR_INV_PARCEL = -3;      // the parcel descriptor handle is invalid
XPI_Err const XPI_ERR_NO_MEM = -4;          // not enough space to allocate memory
XPI_Err const XPI_ERR_INV_ADDR = -5;        // the target global address is invalid
XPI_Err const XPI_ERR_BAD_ARG = -6;         // the given argument is invalid

#if defined(__cplusplus)
}
#endif

#endif //header guard
