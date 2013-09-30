//  Copyright (c) 2013 Hartmut Kaiser, Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_H_APR_27_2013_1135AM)
#define HPXPI_H_APR_27_2013_1135AM

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
using std::size_t;
#else
#include <stdint.h>
#include <stddef.h>
#endif

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
// XPI Error Codes [Appendix B]
///////////////////////////////////////////////////////////////////////////////
XPI_Err const XPI_SUCCESS = 0;              // success
XPI_Err const XPI_ERR_ERROR = -1;           // generic error
XPI_Err const XPI_ERR_INV_TYPE = -2;        // the type is invalid
XPI_Err const XPI_ERR_INV_PARCEL = -3;      // the parcel descriptor handle is invalid
XPI_Err const XPI_ERR_NO_MEM = -4;          // not enough space to allocate memory
XPI_Err const XPI_ERR_INV_ADDR = -5;        // the target global address is invalid

///////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown [3.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_init initializes the XPI runtime, using the passed arguments and the
// environment. C applications usually simply forward pointers to argc, argv,
// and envp (if available). XPI specific arguments are removed from argc and
// argv, and the environment is updated as necessary.
HPXPI_EXPORT XPI_Err XPI_init(int* nargs, char*** args, char*** env);

// XPI_run encapsulates the creation of the main process and its associated
// initial thread, XPI_main. XPI_run also manages the future required for
// XPI_main to return a value. This routine is synchronous, and will not return
// until the main process terminates.
HPXPI_EXPORT XPI_Err XPI_run(int argc, char* argv[], int* result);

// XPI_finalize terminates the execution of the XPI runtime, releasing resources
// acquired in XPI_init and XPI_run.
HPXPI_EXPORT XPI_Err XPI_finalize();

///////////////////////////////////////////////////////////////////////////////
// High-Level Interface Routines [3.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_abort unconditionally aborts the execution of an XPI application,
// returning control to the XPI_run site with the result set to the passed
// code, and optionally printing the message. This should clean up resources
// associated with the running application.
HPXPI_EXPORT HPXPI_ATTRIBUTE_NORETURN void XPI_abort(int code, char* message);

// XPI_version queries the specification version number that the XPI
// implementation conforms to.
HPXPI_EXPORT void XPI_version(size_t* major, size_t* minor, size_t* release);

///////////////////////////////////////////////////////////////////////////////
// Data & Types [4.2]
///////////////////////////////////////////////////////////////////////////////

// XPI supplies a number of intrinsic types, and then provides programmers with
// the ability to compose these types to create complex types.
typedef struct XPI_Type { void* handle; } XPI_Type;

///////////////////////////////////////////////////////////////////////////////
// Intrinsic types [4.2.1]
HPXPI_EXPORT extern XPI_Type XPI_VOID;               // void
HPXPI_EXPORT extern XPI_Type XPI_ERROR;              // XPI_Err
HPXPI_EXPORT extern XPI_Type XPI_BOOL;               // bool
HPXPI_EXPORT extern XPI_Type XPI_CHAR;               // char
HPXPI_EXPORT extern XPI_Type XPI_SHORT;              // short
HPXPI_EXPORT extern XPI_Type XPI_INT;                // int
HPXPI_EXPORT extern XPI_Type XPI_LONG;               // long
HPXPI_EXPORT extern XPI_Type XPI_LONG_LONG;          // long long
HPXPI_EXPORT extern XPI_Type XPI_UNSIGNED_CHAR;      // unsigned char
HPXPI_EXPORT extern XPI_Type XPI_UNSIGNED_INT;       // unsigned int
HPXPI_EXPORT extern XPI_Type XPI_UNSIGNED_SHORT;     // unsigned short
HPXPI_EXPORT extern XPI_Type XPI_UNSIGNED_LONG;      // unsigned long
HPXPI_EXPORT extern XPI_Type XPI_UNSIGNED_LONG_LONG; // unsigned long long
HPXPI_EXPORT extern XPI_Type XPI_FLOAT;              // float
HPXPI_EXPORT extern XPI_Type XPI_DOUBLE;             // double
HPXPI_EXPORT extern XPI_Type XPI_LONG_DOUBLE;        // long double
HPXPI_EXPORT extern XPI_Type XPI_FLOAT_COMPLEX;      // Complex float
HPXPI_EXPORT extern XPI_Type XPI_DOUBLE_COMPLEX;     // Complex double
HPXPI_EXPORT extern XPI_Type XPI_LONG_DOUBLE_COMPLEX;    // Complex long double
HPXPI_EXPORT extern XPI_Type XPI_UINT8_T;            // uint8_t
HPXPI_EXPORT extern XPI_Type XPI_UINT16_T;           // uint16_t
HPXPI_EXPORT extern XPI_Type XPI_UINT32_T;           // uint32_t
HPXPI_EXPORT extern XPI_Type XPI_UINT64_T;           // uint64_t
HPXPI_EXPORT extern XPI_Type XPI_INT8_T;             // int8_t
HPXPI_EXPORT extern XPI_Type XPI_INT16_T;            // int16_t
HPXPI_EXPORT extern XPI_Type XPI_INT32_T;            // int32_t
HPXPI_EXPORT extern XPI_Type XPI_INT64_T;            // int64_t
HPXPI_EXPORT extern XPI_Type XPI_SIZE_T;             // size_t

///////////////////////////////////////////////////////////////////////////////
// Derived types [4.2.2]

// Given an intrinsic or derived type, XPI TYPE CONTIGUOUS will create a new
// type that replicates the type as a sequence of count contiguous instances.
//
// XPI_Type old;        // old type handle, already constructed
// XPI_Type new;        // new type handle
// size t count;        // number of replications of old required
// contiguous type construction
HPXPI_EXPORT XPI_Err XPI_Type_contiguous(size_t count, XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Type_vector(size_t count, size_t blocklength, size_t stride,
    XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Type_indexed(size_t n, size_t blocklens[], size_t offsets[],
    XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Type_struct(size_t n, size_t blocklens[], size_t offsets[],
    XPI_Type types[], XPI_Type* new_type);

// XPI_Type_lco creates an LCO type. This type is used to mark the first
// parameter of an LCO action, and is only useful as the first list entry in
// the XPI_TYPE_ACTION constructor.
HPXPI_EXPORT XPI_Err XPI_Type_lco(XPI_Type type, XPI_Type* new_type);

// XPI_Type_global_pointer creates a typed pointer into the global address
// space. It can be used in any composite type, and has a special meaning in
// the XPI_TYPE_ACTION constructor, where its use as a first parameter type
// implies that the action is a normal user action.
HPXPI_EXPORT XPI_Err XPI_Type_global_pointer(XPI_Type type, XPI_Type* new_type);

// XPI_Type_action creates an XPI_TYPE representing an action. This is suitable
// for use in either XPI_PROCESS_REGISTER_ACTION or XPI_PARCEL_SEND. Note that
// the return type of all actions is XPI_ERR and does not need to be specified.
// The first parameter type has special semantics in XPI. If the first parameter
// is a global pointer type, returned from the XPI_TYPE_GLOBAL_POINTER
// constructor, then the action is interpreted as a normal user action [4.1.2].
// If, on the other hand, it is an LCO type returned from XPI_TYPE_LCO, then
// the action is interpreted as an LCO action, meeting the requirements laid
// out in Section [4.1.4].
HPXPI_EXPORT XPI_Err XPI_Type_action(size_t n, XPI_Type types[], XPI_Type* new_type);

// XPI_Type_get_size can be used to allocate a buffer for data in the advanced
// XPI_PARCEL_GET_DATA routine.
HPXPI_EXPORT XPI_Err XPI_Type_get_size(XPI_Type type, size_t* size);

// While types are managed by the runtime as handles, they are compared for
// equality structurally. This routine returns TRUE if the structure of the two
// types is equal, and FALSE otherwise. It is not an error to use an invalid
// type in this operation - an invalid type always generates a FALSE result.
HPXPI_EXPORT bool XPI_Type_equals(XPI_Type lhs, XPI_Type rhs);

///////////////////////////////////////////////////////////////////////////////
// Parcel Continuations [4.3]
///////////////////////////////////////////////////////////////////////////////

// When XPI threads initialize parcels, they designate the parcel's action,
// along with a continuation action. XPI_CONTINUE is the low level, XPI
// intrinsic function that an action uses to communicate with it's dynamically
// specified continuation action.
// The C implementation provides three options for continuing, XPI_CONTINUE
// behaves as specified in the XPI declaration. Many actions will only continue
// simple scalar values and can use XPI_continue1 directly for this purpose.
// Finally, XPI continueV allows an XPI_VOID terminated list of (type, val)
// pairs to be passed as varargs parameters.
// When XPI threads initialize parcels, they designate the parcel's action,
// along with a continuation action. XPI CONTINUE is the low level, XPI 
// intrinsic function that an action uses to communicate with it's dynamically 
// specified continuation action.
HPXPI_EXPORT HPXPI_ATTRIBUTE_NORETURN void XPI_continue(size_t n,
    XPI_Type types[], void* vals[]);
HPXPI_EXPORT HPXPI_ATTRIBUTE_NORETURN void XPI_continue1(XPI_Type type, void* val);
HPXPI_EXPORT HPXPI_ATTRIBUTE_NORETURN void XPI_continueV(XPI_Type type, ...);

///////////////////////////////////////////////////////////////////////////////
// Parcel Generation [4.4]
///////////////////////////////////////////////////////////////////////////////

typedef struct XPI_Parcel { void* handle; } XPI_Parcel;
typedef struct XPI_Addr { void* handle; } XPI_Addr;

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_create(XPI_Parcel* handle);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_free(XPI_Parcel handle);

// The target address must have a valid mapping at the time that
// XPI_PARCEL_SEND is called.
HPXPI_EXPORT XPI_Err XPI_Parcel_set_target(XPI_Parcel handle, XPI_Addr target);

// The action must correspond to an action that was registered with the XPI
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_PARCEL_SEND being
// called.
HPXPI_EXPORT XPI_Err XPI_Parcel_set_action(XPI_Parcel handle, char* action);

// The continuation target address must have a valid mapping at the time that
// XPI_CONTINUE is called.
HPXPI_EXPORT XPI_Err XPI_Parcel_set_cont_target(XPI_Parcel handle, XPI_Addr target);

// The action must correspond to an action that was registered with the XPI
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_CONTINUE being called.
HPXPI_EXPORT XPI_Err XPI_Parcel_set_cont_action(XPI_Parcel handle, char* action);

// Bind the individual action arguments to the action’s data.
HPXPI_EXPORT XPI_Err XPI_Parcel_set_data(XPI_Parcel handle, XPI_Type type,
    size_t n, void* arguments[]);

// XPI_Parcel_send send a parcel.
HPXPI_EXPORT XPI_Err XPI_Parcel_send(XPI_Parcel handle, XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// Apply [4.5]
///////////////////////////////////////////////////////////////////////////////

// XPI_APPLY encapsulates the steps required to assemble and send a parcel and
// continuation to effect a remote-procedure call. If the action has a type
// registered with XPI_PROCESSREGISTER_ACTION then the type argument should be
// set to XPI VOID. If the action does not continue a value, or if the continued
// value is to be ignored, then future should be set to XPI_NULL.
HPXPI_EXPORT XPI_Err XPI_Parcel_apply(XPI_Addr target, char* action,
    XPI_Type type, size_t nargs, void* args[]);

///////////////////////////////////////////////////////////////////////////////
// Advanced [4.6]
///////////////////////////////////////////////////////////////////////////////

// XPI_Parcel_select can be used to explicitly receive parcel handles from the
// network layer.
HPXPI_EXPORT XPI_Err XPI_Parcel_select(char* match, size_t n,
    XPI_Parcel parcels[], size_t* matched);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_get_target(XPI_Parcel handle, XPI_Addr* target);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_get_action(XPI_Parcel handle, char** action);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_get_cont_target(XPI_Parcel handle, XPI_Addr* target);

// Spec misses documentation
HPXPI_EXPORT XPI_Err XPI_Parcel_get_cont_action(XPI_Parcel handle, char** action);

// This returns the XPI TYPE for the parcel's action. It will never return
// XPI_VOID, If the type was specified as part of XPI_PARCEL_SET_DATA, it is
// returned, otherwise, the type set as part of XPI_PROCESS_REGISTER_ACTION is
// returned. This can be combined with XPI_TYPE_GET_SIZE to allocate a buffer
// for XPI_PARCEL_GET_DATA.
HPXPI_EXPORT XPI_Err XPI_Parcel_get_data_type(XPI_Parcel handle, XPI_Type* type);

// XPI_Parcel_get_n_args retrieves the number of arguments encoded in the
// parcel's data segment. This information can be used to allocate an
// appropriately sized argument list buffer for use in XPI_PARCEL_GET_DATA.
HPXPI_EXPORT XPI_Err XPI_Parcel_get_n_args(XPI_Parcel handle, size_t* n);

// XPI_Parcel_get_data unpacks the data segment into buffer and fills the
// arguments array with the appropriate argument addresses from within the
// buffer. The buffers must be large enough to accept the parcels' data (see
// XPI_PARCEL_GET_DATA_TYPE and XPI_PARCEL_GET_N_ARGS).
HPXPI_EXPORT XPI_Err XPI_Parcel_get_data(XPI_Parcel handle, void** arguments,
    void* data);

///////////////////////////////////////////////////////////////////////////////
// Address Space Representation [5.1]
///////////////////////////////////////////////////////////////////////////////

typedef struct uint128_t { uint64_t msb; uint64_t lsb; } uint128_t;

// The NULL global virtual address is defined such that, when compared to an
// address initialized with the integer, 0.
HPXPI_EXPORT extern XPI_Addr XPI_NULL;

// This interprets the integer as an index into the byte array representation
// of the global virtual address space, and initializes a structure suitable
// for use in the parcel subsystem. Arbitrarily large addresses cannot be
// generated this way, further address arithmetic will be required to produce
// very large addresses.
HPXPI_EXPORT XPI_Err XPI_Addr_init(uint128_t address, XPI_Addr* result);

// This compares two addresses, setting result to -1, 0, or 1 if lhs is
// less-than, equal-to, or greater-than rhs, respectively.
HPXPI_EXPORT int XPI_Addr_cmp(XPI_Addr lhs, XPI_Addr rhs);

// This converts the integer into an address difference structure. Arbitrarily
// large differences cannot be generated this way, and will need to be
// generated using the address difference structure’s math interface.
// Differences are not valid parcel targets.
typedef struct XPI_Addr_Diff { void* handle; } XPI_Addr_Diff;

HPXPI_EXPORT XPI_Err XPI_Addr_Diff_init(uint128_t address, XPI_Addr_Diff* result);

// This adds two differences to produce a sum.
HPXPI_EXPORT XPI_Addr_Diff XPI_Addr_Diff_add(XPI_Addr_Diff lhs, XPI_Addr_Diff rhs);

// This scales an address difference by a given factor, which may be a fraction.
// Rounding rules follow C integer rounding.
HPXPI_EXPORT XPI_Addr_Diff XPI_Addr_Diff_mult(XPI_Addr_Diff base, float n);

// This subtracts two differences to produce a difference.
HPXPI_EXPORT XPI_Addr_Diff XPI_Addr_sub(XPI_Addr lhs, XPI_Addr rhs);

// This add an address and a difference to produce another address.
HPXPI_EXPORT XPI_Addr XPI_Addr_add(XPI_Addr lhs, XPI_Addr_Diff offset);

///////////////////////////////////////////////////////////////////////////////
// Native Interface [5.2]
///////////////////////////////////////////////////////////////////////////////

// Loads a value from the global address space, and forwards it to a parcel
// continuation, if one exists.

// XPI_Err XPI_AGAS_LOAD1_ACTION(XPI_Addr addr); // CONTINUE(uint8_t val)
HPXPI_EXPORT extern XPI_Type XPI_AGAS_LOAD1_ACTION;

// XPI_Err XPI_AGAS_LOAD2_ACTION(XPI_Addr addr); // CONTINUE(uint16_t val)
HPXPI_EXPORT extern XPI_Type XPI_AGAS_LOAD2_ACTION;

// XPI_Err XPI_AGAS_LOAD4_ACTION(XPI_Addr addr); // CONTINUE(uint32_t val)
HPXPI_EXPORT extern XPI_Type XPI_AGAS_LOAD4_ACTION;

// XPI_Err XPI_AGAS_LOAD8_ACTION(XPI_Addr addr); // CONTINUE(uint64_t val)
HPXPI_EXPORT extern XPI_Type XPI_AGAS_LOAD8_ACTION;

HPXPI_EXPORT XPI_Err XPI_Agas_load1(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load2(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load4(XPI_Addr addr, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_load8(XPI_Addr addr, XPI_Addr future);

HPXPI_EXPORT XPI_Err XPI_Agas_load1_sync(XPI_Addr addr, uint8_t* val);
HPXPI_EXPORT XPI_Err XPI_Agas_load2_sync(XPI_Addr addr, uint16_t* val);
HPXPI_EXPORT XPI_Err XPI_Agas_load4_sync(XPI_Addr addr, uint32_t* val);
HPXPI_EXPORT XPI_Err XPI_Agas_load8_sync(XPI_Addr addr, uint64_t* val);

// Stores a value to the global address space.
// XPI_Err XPI_AGAS_STORE1_ACTION(XPI_Addr addr, uint8_t val); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_AGAS_STORE1_ACTION;

// XPI_Err XPI_AGAS_STORE2_ACTION(XPI_Addr addr, uint16_t val); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_AGAS_STORE2_ACTION;

// XPI_Err XPI_AGAS_STORE4_ACTION(XPI_Addr addr, uint32_t val); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_AGAS_STORE4_ACTION;

// XPI_Err XPI_AGAS_STORE8_ACTION(XPI_Addr addr, uint64_t val); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_AGAS_STORE8_ACTION;

HPXPI_EXPORT XPI_Err XPI_Agas_store1(XPI_Addr addr, uint8_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store2(XPI_Addr addr, uint16_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store4(XPI_Addr addr, uint32_t val, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Agas_store8(XPI_Addr addr, uint64_t val, XPI_Addr future);

HPXPI_EXPORT XPI_Err XPI_Agas_store1_sync(XPI_Addr addr, uint8_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store2_sync(XPI_Addr addr, uint16_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store4_sync(XPI_Addr addr, uint32_t val);
HPXPI_EXPORT XPI_Err XPI_Agas_store8_sync(XPI_Addr addr, uint64_t val);

///////////////////////////////////////////////////////////////////////////////
// Threads [6]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_get_self can be used to get the global address corresponding to
// the local thread, and can be used in conjunction with the thread object API
// to query details of threads.
HPXPI_EXPORT XPI_Err XPI_Thread_get_self(XPI_Addr* addr);

///////////////////////////////////////////////////////////////////////////////
// Thread Synchronization [6.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_wait_any allows a threads execution to block until any one of the
// specified LCOs fires. This should behave as if it is implemented in terms of
// XPI_LCO_LINK, along with a scheduler transition of the thread to a depleted
// state (which will be reset by the threads XPI_LCO_TRIGGER_ACTION handler).
HPXPI_EXPORT XPI_Err XPI_Thread_wait_any(size_t n, XPI_Addr lcos[], XPI_Addr* lco);

// XPI_Thread_wait_all blocks until all of the LCOs in lcos have fired. This is
// a convenience interface to the XPI_THREAD_WAIT_ANY routine, that behaves as
// if it loops waiting for each LCO to fire, however it may have its own distinct
// implementation for performance reasons.
HPXPI_EXPORT XPI_Err XPI_Thread_wait_all(size_t n, XPI_Addr lcos[]);

// XPI_Thread_wait blocks until the LCO fires. This is a convenience interface
// to the XPI_THREAD_WAIT_ANY routine however it may have its own distinct
// implementation for performance reasons.
HPXPI_EXPORT XPI_Err XPI_Thread_wait(XPI_Addr obj);

///////////////////////////////////////////////////////////////////////////////
// Thread Scheduling [6.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_set_priority will set the priority for the target thread.
//
// XPI_Err XPI_THREAD_SET_PRIORITY_ACTION(XPI_Addr address, size_t priority); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_THREAD_SET_PRIORITY_ACTION;

HPXPI_EXPORT XPI_Err XPI_Thread_set_priority(XPI_Addr address, size_t priority,
    XPI_Addr future);

// XPI_Thread_set_state changes the state of the target thread.
//
// XPI_Err XPI_THREAD_SET_STATE_ACTION(XPI_Addr address, XPI_Thread_state state); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_THREAD_SET_STATE_ACTION;

typedef enum XPI_Thread_State {
    XPI_THREAD_STATE_ACTIVE,
    XPI_THREAD_STATE_SUSPENDED,
    XPI_THREAD_STATE_DEPLETED,
    XPI_THREAD_STATE_TERMINATED
} XPI_Thread_State;

HPXPI_EXPORT XPI_Err XPI_Thread_set_state(XPI_Addr address,
    XPI_Thread_State state, XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// Thread Resources [6.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Thread_get_process can be used to get the global address corresponding
// to a thread's process.
//
// XPI_Err XPI_THREAD_GET_PROCESS_ACTION(XPI_Addr address); // CONTINUE(XPI_Addr process)
HPXPI_EXPORT extern XPI_Type XPI_THREAD_GET_PROCESS_ACTION;

HPXPI_EXPORT XPI_Err XPI_Thread_get_process(XPI_Addr address, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Thread_get_process_sync(XPI_Addr address, XPI_Addr* process);

///////////////////////////////////////////////////////////////////////////////
// Fixed Actions [7.2.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_LCO_link adds the target LCO to the notification list for the source
// LCO. When the source LCO's predicate evaluates as true, a parcel will be
// generated targeting each linked LCO's trigger routine.
//
// XPI_Err XPI_LCO_LINK_ACTION(XPI_Addr source, XPI_Addr target); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_LCO_LINK_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_link(XPI_Addr source, XPI_Addr target,
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_link_sync(XPI_Addr source, XPI_Addr target);

// XPI_LCO_unlink unlinks the target LCO from the source LCO, assuming that
// the target LCO has been linked. It is not an error to attempt to unlink
// a target that was not linked to the source.
//
// XPI_Err XPI_LCO_UNLINK_ACTION(XPI_Addr source, XPI_Addr target); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_LCO_UNLINK_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_unlink(XPI_Addr source, XPI_Addr target,
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_LCO_unlink_sync(XPI_Addr source, XPI_Addr target);

///////////////////////////////////////////////////////////////////////////////
// Polymorphic Actions [7.2.2]
///////////////////////////////////////////////////////////////////////////////

// This is used to initialize an LCO structure after it has been allocated
// by the process. The lco address must have been allocated using
// XPI_PROCESS_LCO_MALLOC in order to ensure that it has enough space for
// the XPI implementation to correctly provide the LCO properties.
//
// XPI_Err XPI_LCO_INIT_ACTION(XPI_Addr lco); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_LCO_INIT_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_init(XPI_Addr lco, XPI_Addr future);

// XPI_LCO_fini can be used to clean up any resources before an
// LCO is freed using XPI_PROCESS_GLOBAL_FREE.
//
// XPI_Err XPI_LCO_FINI_ACTION(XPI_Addr lco); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_LCO_FINI_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_fini(XPI_Addr lco, XPI_Addr future);

// XPI_LCO_trigger action triggers the LCO to potentially change
// state. There are special semantics with respect to the LCO trigger
// action; the LCO's predicate is automatically tested after the trigger
// executes.
//
// XPI_Err XPI_LCO_TRIGGER_ACTION(XPI_Addr lco); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_LCO_TRIGGER_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_trigger(XPI_Addr lco, XPI_Addr future);

// XPI_LCO_get_size action is used to read the size, in bytes, of
// the LCO structure. This will not include any additional bytes
// allocated by the process in order to provide support for LCO
// semantics for this object.
//
// XPI_Err XPI_LCO_GET_SIZE_ACTION(XPI_Addr lco); // CONTINUE(size t size)
HPXPI_EXPORT extern XPI_Type XPI_LCO_GET_SIZE_ACTION;

HPXPI_EXPORT XPI_Err XPI_LCO_get_size(XPI_Addr lco, XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// Future [7.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Future_new is for future allocation. As with all LCOs, futures are
// allocated in the global namespace. For local allocation, process can be set
// to XPI_NULL in which case both of these routines behave as if they are
// implemented with XPI_PROCESS_LCO_MALLOC_ACTION, using the parent process of
// the calling thread (see XPI_THREAD_GET_PROCESS) as the allocating process.
//
// XPI_FUTURE_NEW_SYNC is of particular interest, as it solves the chicken-and-egg
// problem of needing to allocate a future in order to read the global address of
// the newly allocated future by hiding this process inside the library.
//
// XPI_Err XPI_FUTURE_NEW_ACTION(XPI_Type type); // CONTINUE(XPI_Addr address)
HPXPI_EXPORT extern XPI_Type XPI_FUTURE_NEW_ACTION;

HPXPI_EXPORT XPI_Err XPI_Future_new(XPI_Type type, XPI_Type future);
HPXPI_EXPORT XPI_Err XPI_Future_new_sync(XPI_Type type, XPI_Addr* address);

// XPI_Future_get_buffer_type allows programmers to query the type of a future,
// to make sure that it can buffer, and correctly extract, a particular type.
//
// XPI_Err XPI_FUTURE_GET_BUFFER_TYPE_ACTION(XPI_Addr address); // CONTINUE(XPI_Type type)
HPXPI_EXPORT extern XPI_Type XPI_FUTURE_GET_BUFFER_TYPE_ACTION;

HPXPI_EXPORT XPI_Err XPI_Future_get_buffer_type(XPI_Addr address, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Future_get_buffer_type_sync(XPI_Addr address, XPI_Type* type);

// XPI_Future_get_value_sync, along with XPI_FUTURE_NEW_SYNC, is the second
// magic synchronous function related to futures. It transfers the value of
// the data begin buffered to the passed local address. This routine will fail
// if the global address of the future is not in the same synchronous
// domain as that of the calling thread. The buffer must be large enough to
// receive the data described by the futures type returned by
// XPI_FUTURE_GET_BUFFER_TYPE.
HPXPI_EXPORT XPI_Err XPI_Future_get_value_sync(XPI_Addr address, void* buffer);

///////////////////////////////////////////////////////////////////////////////
// User LCOs [7.4]
///////////////////////////////////////////////////////////////////////////////

// The user LCO interface is used to describe the required action handlers,
// and the LCO predicate specification. Note that the handler types conform
// to the specification for LCO actions outlined in Section 4.1.4.

typedef XPI_Err (*XPI_LCO_Init_Handler)(void* lco);
typedef XPI_Err (*XPI_LCO_Fini_Handler)(void* lco);
typedef XPI_Err (*XPI_LCO_Trigger_Handler)(void* lco, size_t n, void* args[]);
typedef XPI_Err (*XPI_LCO_Get_Size_Handler)(void* lco);
typedef bool (*XPI_LCO_Predicate)(void* lco);

typedef struct XPI_LCO_Subtype_Descriptor {
    XPI_LCO_Init_Handler init;
    XPI_LCO_Fini_Handler fini;
    XPI_LCO_Trigger_Handler trigger;
    XPI_LCO_Get_Size_Handler get_size;
    XPI_LCO_Predicate predicate;
} XPI_LCO_Subtype_Descriptor;

///////////////////////////////////////////////////////////////////////////////
// Process Instantiation & Termination [8.1.1]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_create_child creates a child process. This child will have its
// parent process set to the passed process, and its designated result value
// may be written to the result future. If no result is required by the
// instantiating thread, i.e., the process is being instantiated only for its
// side effects, then result should be set to XPI_NULL.
//
// XPI_Err XPI_PROCESS_CREATE_CHILD_ACTION(XPI_Addr process, char* initial,
//     XPI_Type type, size_t nargs, void* args[], XPI_Addr result); // CONTINUE(XPI_Addr child)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_CREATE_CHILD_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_create_child(XPI_Addr process, char* initial,
    XPI_Type type, size_t nargs, void* args[], XPI_Addr result, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_create_child_sync(XPI_Addr process, char* initial,
    XPI_Type type, size_t nargs, void* args[], XPI_Addr result, XPI_Addr* child);

// XPI_Process_get_result gets the designated result for a process, in the form
// of a future. The is the future that is passed into XPI_PROCESS_CREATE_CHILD,
// and can be XPI_NULL if this was not set.
//
// XPI_Err XPI_PROCESS_GET_RESULT_ACTION(XPI_Addr process); // CONTINUE(XPI_Addr result)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GET_RESULT_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_get_result(XPI_Addr process, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_get_result_sync(XPI_Addr process, XPI_Addr* result);

// XPI_Process_terminate unconditionally terminates the target process. This
// terminates execution of any threads. XPI_PROCESS_TERMINATE_ACTION or
// XPI_PROCESS_TERMINATE may be performed by a thread in the context of the
// to-be-terminated process, however performing any additional operations within
// the context of this thread's execution results in implementation-defined
// behavior, as does using XPI_PROCESS_TERMINATE_SYNC from within the context
// of the to-beterminated process.
//
// XPI_Err XPI_PROCESS_TERMINATE_ACTION(XPI_Addr process); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_TERMINATE_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_terminate(XPI_Addr process, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_terminate_sync(XPI_Addr process);

///////////////////////////////////////////////////////////////////////////////
// The Main Process [8.1.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_main is not implemented by XPI. It merely describes the action that
// XPI applications are required to provide as the initial action for the
// main process.
HPXPI_APPLICATION_EXPORT XPI_Err XPI_main(size_t nargs, void* args[]);

///////////////////////////////////////////////////////////////////////////////
// Hierarchy Inspection [8.1.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_get_parent gets the address of a processes' parent process. The
// main process will return XPI_NULL. An orphaned process will return the address
// of the main process.
//
// XPI_Err XPI_PROCESS_GET_PARENT_ACTION(XPI_Addr process); // CONTINUE(XPI_Addr parent)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GET_PARENT_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_get_parent(XPI_Addr process, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_get_parent_sync(XPI_Addr process, XPI_Addr* parent);

// This retrieves the number of children for a process.
//
// XPI_Err XPI_PROCESS_GET_N_CHILDREN_ACTION(XPI_Addr process); // CONTINUE(size_t n)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GET_N_CHILDREN_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_get_n_children(XPI_Addr process, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_get_n_children_sync(XPI_Addr process, size_t* n);

// XPI_Process_get_child retrieves the i-th child of a process. If i is out of the
// range of valid child indices, this will return XPI_NULL.
//
// XPI_Err XPI_PROCESS_GET_CHILD(XPI_Addr process, size_t i); // CONTINUE(XPI_Addr child)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GET_CHILD;

HPXPI_EXPORT XPI_Err XPI_Process_get_child(XPI_Addr process, size_t i,
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_get_child_sync(XPI_Addr process, size_t i,
    XPI_Addr* child);

///////////////////////////////////////////////////////////////////////////////
// Action Management [8.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_Process_register_action registers an action with the runtime. It must be
// performed in order to send parcels to the action. It must be performed in a
// native C thread run on each locality, and may be performed as part of a static
// constructor.
//
// XPI_Err XPI_PROCESS_REGISTER_ACTION_ACTION(XPI_Addr process, char* id,
//     void* function, XPI Type type, size t stack); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_REGISTER_ACTION_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_register_action(XPI_Addr process, char* id,
    void* function, XPI_Type type, size_t stack, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_register_action_sync(XPI_Addr process,
    char* id, void* function, XPI_Type type, size_t stack);

///////////////////////////////////////////////////////////////////////////////
// Memory Management [8.3]
///////////////////////////////////////////////////////////////////////////////

// Allocates a size-byte region in global memory. The distribution parameter
// provides a hint to the implementation of how this allocation should be
// initially distributed.
//
// XPI_Err XPI_PROCESS_GLOBAL_MALLOC_ACTION(XPI_Addr process, size_t size,
//     XPI_Distribution distribution); // CONTINUE(XPI_Addr address);
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GLOBAL_MALLOC_ACTION;

typedef struct XPI_Distribution { void* handle; } XPI_Distribution;

HPXPI_EXPORT XPI_Err XPI_Process_global_malloc(XPI_Addr process, size_t size,
    XPI_Distribution distribution, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_global_malloc_sync(XPI_Addr process,
    size_t size, XPI_Distribution distribution, XPI_Addr* address);

// XPI_Process_global_free frees a region of globally allocated memory. The
// address must be the result of an XPI_PROCESS_GLOBAL_MALLOC_ACTION call.
// This call is asynchronous, however the future can be used to wait until
// the operation has completed globally. It is not an error to free XPI_NULL.
//
// XPI_Err XPI_PROCESS_GLOBAL_FREE_ACTION(XPI_Addr process, 
//      XPI_Addr address); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_GLOBAL_FREE_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_global_free(XPI_Addr process, XPI_Addr address,
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_global_free_sync(XPI_Addr process,
    XPI_Addr address);

// Pins a global address range. The entire range should be part of a single
// allocation, and resident locally.
//
// XPI_Err XPI_PROCESS_PIN_ACTION(XPI_Addr process, XPI_Addr base, 
//      XPI_Addr_Diff extent); // CONTINUE(void address)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_PIN_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_pin(XPI_Addr process, XPI_Addr base,
    XPI_Addr_Diff extent, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_pin_sync(XPI_Addr process, XPI_Addr base,
    XPI_Addr_Diff extent, void** result);

// This action releases a previously pinned region to the system. The address 
// must correspond to the global base address of a previously pinned region.
//
// XPI_Err XPI_PROCESS_UNPIN_ACTION(XPI_Addr process, XPI_Addr address); // CONTINUE()
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_UNPIN_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_unpin(XPI_Addr process, XPI_Addr address, 
    XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_unpin_sync(XPI_Addr process, XPI_Addr address);

// XPI_Process_memcpy copies bytes in the global address space asynchronously. 
// The from and to ranges may overlap. The future provides strong ordering if 
// needed.
//
// XPI_Err XPI_PROCESS_MEMCPY_ACTION(XPI_Addr process, XPI_Addr from, 
//      XPI_Addr to, size_t bytes); // CONTINUE(XPI Addr from)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_MEMCPY_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_memcpy(XPI_Addr process, XPI_Addr from, 
    XPI_Addr to, size_t bytes, XPI_Addr future);
HPXPI_EXPORT XPI_Err XPI_Process_memcpy_sync(XPI_Addr process, XPI_Addr from, 
    XPI_Addr to, size_t bytes, XPI_Addr* result);

///////////////////////////////////////////////////////////////////////////////
// LCO Management [8.4]
///////////////////////////////////////////////////////////////////////////////

// This action, and its corresponding asynchronous routine, deal with the 
// special allocation requirements for LCOs. In particular, LCOs may need 
// extra, implementation-specific space in order to implement some of the 
// required LCO properties including strict serializability and polymorphic 
// action handlers.
//
// XPI_Err XPI_PROCESS_LCO_MALLOC_ACTION(XPI_Addr process, size_t size, 
//      XPI_LCO_Subtype_Descriptor actions); // CONTINUE(XPI_Addr address)
HPXPI_EXPORT extern XPI_Type XPI_PROCESS_LCO_MALLOC_ACTION;

HPXPI_EXPORT XPI_Err XPI_Process_lco_malloc(XPI_Addr process, size_t size, 
    XPI_LCO_Subtype_Descriptor actions, XPI_Addr future);

#if defined(__cplusplus)
}
#endif

#endif
