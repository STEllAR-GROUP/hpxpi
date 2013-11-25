//  Copyright (c) 2013 Hartmut Kaiser, Alexander Duchene
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_H_APR_27_2013_1135AM)
#define HPXPI_H_APR_27_2013_1135AM

#include <hpx/config/defines.hpp>

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
using std::size_t;
#include <cstdint>
using std::intptr_t;
#else
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
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
// Address Space Representation [5.4]
///////////////////////////////////////////////////////////////////////////////

// Implementation defined
typedef struct { intptr_t adddr; } XPI_Addr;

extern XPI_Addr XPI_NULL;

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
HPXPI_EXPORT XPI_Err XPI_register_action_with_key(XPI_Action action, char* key);
#define XPI_register_action(act) XPI_register_action_with_key(act, #act)

///////////////////////////////////////////////////////////////////////////////
// Parcel Generation [4.1]
///////////////////////////////////////////////////////////////////////////////
typedef intptr_t XPI_Parcel;

// Create an empty parcel
HPXPI_EXPORT XPI_Err XPI_Parcel_create(XPI_Parcel* parcel);

// Free an existing parcel
HPXPI_EXPORT XPI_Err XPI_Parcel_free(XPI_Parcel parcel);

// Set where the parcel action is invoked
// Can be ignored except for LCO targets
// XPI_NULL specifies no prefered address
HPXPI_EXPORT XPI_Err XPI_Parcel_set_addr(XPI_Parcel parcel, XPI_Addr addr);

// Set the parcel action
HPXPI_EXPORT XPI_Err XPI_Parcel_set_action(XPI_Parcel parcel, XPI_Action action);

// Set the parcel environment data
HPXPI_EXPORT XPI_Err XPI_Parcel_set_env(XPI_Parcel parcel, size_t bytes, void* data);

// Set the parcel argument data
HPXPI_EXPORT XPI_Err XPI_Parcel_set_data(XPI_Parcel parcel, size_t bytes, void* data);

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

// Sends a parcel, with future signalling completion
HPXPI_EXPORT XPI_Err XPI_Parcel_send(XPI_Parcel parcel, XPI_Addr future);

///////////////////////////////////////////////////////////////////////////////
// XPI Error Codes [Appendix B]
///////////////////////////////////////////////////////////////////////////////
XPI_Err const XPI_SUCCESS = 0;              // success
XPI_Err const XPI_ERR_ERROR = -1;           // generic error
XPI_Err const XPI_ERR_INV_TYPE = -2;        // the type is invalid
XPI_Err const XPI_ERR_INV_PARCEL = -3;      // the parcel descriptor handle is invalid
XPI_Err const XPI_ERR_NO_MEM = -4;          // not enough space to allocate memory
XPI_Err const XPI_ERR_INV_ADDR = -5;        // the target global address is invalid

#if defined(__cplusplus)
}
#endif

#endif //header guard
