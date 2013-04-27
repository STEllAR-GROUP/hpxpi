//  Copyright (c) 2007-2013 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_EXPORT_DEFINITIONS_SEP_18_2012_0356PM)
#define HPXPI_EXPORT_DEFINITIONS_SEP_18_2012_0356PM

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# define HPXPI_SYMBOL_EXPORT      __declspec(dllexport)
# define HPXPI_SYMBOL_IMPORT      __declspec(dllimport)
# define HPXPI_SYMBOL_INTERNAL    /* empty */
# define HPXPI_APISYMBOL_EXPORT   __declspec(dllexport)
# define HPXPI_APISYMBOL_IMPORT   __declspec(dllimport)
#else
# define HPXPI_SYMBOL_EXPORT      __attribute__((visibility("default")))
# define HPXPI_SYMBOL_IMPORT      __attribute__((visibility("default")))
# define HPXPI_SYMBOL_INTERNAL    __attribute__((visibility("hidden")))
# define HPXPI_APISYMBOL_EXPORT   __attribute__((visibility("default")))
# define HPXPI_APISYMBOL_IMPORT   __attribute__((visibility("default")))
#endif

// make sure we have reasonable defaults
#if !defined(HPXPI_SYMBOL_EXPORT)
# define HPXPI_SYMBOL_EXPORT      /* empty */
#endif
#if !defined(HPXPI_SYMBOL_IMPORT)
# define HPXPI_SYMBOL_IMPORT      /* empty */
#endif
#if !defined(HPXPI_SYMBOL_INTERNAL)
# define HPXPI_SYMBOL_INTERNAL    /* empty */
#endif
#if !defined(HPXPI_APISYMBOL_EXPORT)
# define HPXPI_APISYMBOL_EXPORT   /* empty */
#endif
#if !defined(HPXPI_APISYMBOL_IMPORT)
# define HPXPI_APISYMBOL_IMPORT   /* empty */
#endif

///////////////////////////////////////////////////////////////////////////////
// define the export/import helper macros used by the core library
#if defined(HPXPI_EXPORTS)
# define  HPXPI_EXPORT             HPXPI_SYMBOL_EXPORT
# define  HPXPI_EXCEPTION_EXPORT   HPXPI_SYMBOL_EXPORT
# define  HPXPI_API_EXPORT         HPXPI_APISYMBOL_EXPORT
#else
# define  HPXPI_EXPORT             HPXPI_SYMBOL_IMPORT
# define  HPXPI_EXCEPTION_EXPORT   HPXPI_SYMBOL_IMPORT
# define  HPXPI_API_EXPORT         HPXPI_APISYMBOL_IMPORT
#endif

///////////////////////////////////////////////////////////////////////////////
// define the export/import helper macros used by applications 
#if defined(HPXPI_APPLICATION_EXPORTS)
# define  HPXPI_APPLICATION_EXPORT HPXPI_SYMBOL_EXPORT
#else
# define  HPXPI_APPLICATION_EXPORT HPXPI_SYMBOL_IMPORT
#endif

///////////////////////////////////////////////////////////////////////////////
// define the export/import helper macros for exceptions 
#if defined(HPXPI_EXPORTS)
# define HPXPI_ALWAYS_EXPORT       HPXPI_SYMBOL_EXPORT
#else
# define HPXPI_ALWAYS_EXPORT       HPXPI_SYMBOL_IMPORT
#endif

#endif
