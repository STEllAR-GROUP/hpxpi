//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_H_APR_27_2013_1135AM)
#define HPXPI_H_APR_27_2013_1135AM

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <hpxpi/config/no_return.h>

///////////////////////////////////////////////////////////////////////////////
// Error Handling [3.1]
///////////////////////////////////////////////////////////////////////////////

typedef int XPI_Err;

XPI_Err const XPI_ERR_TYPE = -1;            // the type is invalid
XPI_Err const XPI_ERR_PARCEL = -2;          // the parcel descriptor handle is invalid
XPI_Err const XPI_ERR_NOMEM = -3;           // not enough space to allocate memory
XPI_Err const XPI_ERR_ADDR = -4;            // the target global address is invalid

///////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown [3.2]
///////////////////////////////////////////////////////////////////////////////

// XPI_init initializes the XPI runtime, using the passed arguments and the
// environment. C applications usually simply forward pointers to argc, argv,
// and envp (if available). XPI specific arguments are removed from argc and
// argv, and the environment is updated as necessary.
XPI_Err XPI_init(int* nargs, char*** args, char*** env);

// XPI_run encapsulates the creation of the main process and its associated
// initial thread, XPI_main. XPI_run also manages the future required for
// XPI_main to return a value. This routine is synchronous, and will not return
// until the main process terminates.
XPI_Err XPI_run(int argc, char* argv[], int* result);

// XPI_finalize terminates the execution of the XPI runtime, releasing resources
// acquired in XPI_init and XPI_run.
XPI_Err XPI_finalize();

///////////////////////////////////////////////////////////////////////////////
// High-Level Interface Routines [3.3]
///////////////////////////////////////////////////////////////////////////////

// XPI_abort unconditionally aborts the execution of an XPI application,
// returning control to the XPI_run site with the result set to the passed
// code, and optionally printing the message. This should clean up resources
// associated with the running application.
HPXPI_ATTRIBUTE_NORETURN void XPI_abort(int code, char* message);

// XPI_version queries the specification version number that the XPI
// implementation conforms to.
void XPI_version(size_t* major, size_t* minor, size_t* release);

///////////////////////////////////////////////////////////////////////////////
// Data & Types [4.2]
///////////////////////////////////////////////////////////////////////////////

// XPI supplies a number of intrinsic types, and then provides programmers with
// the ability to compose these types to create complex types.
typedef int XPI_Type;

///////////////////////////////////////////////////////////////////////////////
// Intrinsic types [4.2.1]
typedef void XPI_VOID;
typedef XPI_Err XPI_ERROR;
typedef bool XPI_BOOL;
typedef char XPI_CHAR;
typedef short XPI_SHORT;
typedef int XPI_INT;
typedef long XPI_LONG;
typedef long long XPI_LONG_LONG;
typedef unsigned char XPI_UNSIGNED_CHAR;
typedef unsigned int XPI_UNSIGNED_INT;
typedef unsigned short XPI_UNSIGNED_SHORT;
typedef unsigned long XPI_UNSIGNED_LONG;
typedef unsigned long long XPI_UNSIGNED_LONG_LONG;
typedef float XPI_FLOAT;
typedef double XPI_DOUBLE;
typedef long double XPI_LONG_DOUBLE;
#if !defined(__cplusplus)
typedef Complex float XPI_FLOAT_COMPLEX;
typedef Complex double XPI_DOUBLE_COMPLEX;
typedef Complex long double XPI_LONG_DOUBLE_COMPLEX;
#endif
typedef uint8_t XPI_UINT8_T;
typedef uint16_t XPI_UINT16_T;
typedef uint32_t XPI_UINT32_T;
typedef uint64_t XPI_UINT64_T;
typedef int8_t XPI_INT8_T;
typedef int16_t XPI_INT16_T;
typedef int32_t XPI_INT32_T;
typedef int64_t XPI_INT64_T;
typedef size_t XPI_SIZE_T;

///////////////////////////////////////////////////////////////////////////////
// Derived types [4.2.2]

// Spec misses documentation
XPI_Err XPI_Type_contiguous(size_t count, XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
XPI_Err XPI_Type_vector(size_t count, size_t blocklength, size_t stride,
    XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
XPI_Err XPI_Type_indexed(size_t n, size_t blocklens[], size_t offsets[],
    XPI_Type old, XPI_Type* new_type);

// Spec misses documentation
XPI_Err XPI_Type_struct(size_t n, size_t blocklens[], size_t offsets[],
    XPI_Type types[], XPI_Type* new_type);

// XPI_Type_lco creates an LCO type. This type is used to mark the first
// parameter of an LCO action, and is only useful as the first list entry in
// the XPI_TYPE_ACTION constructor.
XPI_Err XPI_Type_lco(XPI_Type type, XPI_Type* new_type);

// XPI_Type_global_pointer creates a typed pointer into the global address
// space. It can be used in any composite type, and has a special meaning in
// the XPI_TYPE_ACTION constructor, where its use as a first parameter type
// implies that the action is a normal user action.
XPI_Err XPI_Type_global_pointer(XPI_Type type, XPI_Type* new_type);

// XPI_Type_action creates an XPI TYPE representing an action. This is suitable
// for use in either XPI_PROCESS_REGISTER_ACTION or XPI_PARCEL_SEND. Note that
// the return type of all actions is XPI_ERR and does not need to be specified.
// The first parameter type has special semantics in XPI. If the first parameter
// is a global pointer type, returned from the XPI_TYPE_GLOBAL_POINTER
// constructor, then the action is interpreted as a normal user action [4.1.2].
// If, on the other hand, it is an LCO type returned from XPI_TYPE_LCO, then
// the action is interpreted as an LCO action, meeting the requirements laid
// out in Section [4.1.4].
XPI_Err XPI_Type_action(size_t n, XPI_Type types[], XPI_Type* new_type);

// XPI_Type_get_size can be used to allocate a buffer for data in the advanced
// XPI_PARCEL_GET_DATA routine.
XPI_Err XPI_Type_get_size(XPI_Type type, size_t* size);

// While types are managed by the runtime as handles, they are compared for 
// equality structurally. This routine returns TRUE if the structure of the two 
// types is equal, and FALSE otherwise. It is not an error to use an invalid 
// type in this operation - an invalid type always generates a FALSE result.
bool XPI_Type_equals(XPI_Type lhs, XPI_Type rhs);

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
HPXPI_ATTRIBUTE_NORETURN void XPI_continue(size_t n, XPI_Type types[], void* vals[]);
HPXPI_ATTRIBUTE_NORETURN void XPI_continue1(XPI_Type type, void* val);
HPXPI_ATTRIBUTE_NORETURN void XPI_continueV(XPI_Type type, ...);

///////////////////////////////////////////////////////////////////////////////
// Parcel Generation [4.4]
///////////////////////////////////////////////////////////////////////////////

typedef struct XPI_Parcel { void* handle; } XPI_Parcel;
typedef struct XPI_Addr { uint64_t msb; uint64_t lsb; } XPI_Addr;

// Spec misses documentation
XPI_Err XPI_Parcel_create(XPI_Parcel* handle);

// Spec misses documentation
XPI_Err XPI_Parcel_free(XPI_Parcel handle);

// The target address must have a valid mapping at the time that 
// XPI_PARCEL_SEND is called.
XPI_Err XPI_Parcel_set_target(XPI_Parcel handle, XPI_Addr target);

// The action must correspond to an action that was registered with the XPI 
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_PARCEL_SEND being 
// called.
XPI_Err XPI_Parcel_set_action(XPI_Parcel handle, char* action);

// The continuation target address must have a valid mapping at the time that 
// XPI_CONTINUE is called.
XPI_Err XPI_Parcel_set_cont_target(XPI_Parcel handle, XPI_Addr target);

// The action must correspond to an action that was registered with the XPI 
// runtime using XPI_PROCESS_REGISTER_ACTION prior to XPI_CONTINUE being called.
XPI_Err XPI_Parcel_set_cont_action(XPI_Parcel handle, char* action);

// Bind the individual action arguments to the action’s data.
XPI_Err XPI_Parcel_set_data(XPI_Parcel handle, XPI_Type type, size_t n, 
    void* arguments[]);

// XPI_Parcel_send send a parcel.
XPI_Err XPI_Parcel_send(XPI_Parcel handle, XPI_Addr future);

#if defined(__cplusplus)
}
#endif

#endif
