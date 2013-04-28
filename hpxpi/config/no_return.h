//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPXPI_CONFIG_NO_RETURN_H_APR_27_2013_1249PM)
#define HPXPI_CONFIG_NO_RETURN_H_APR_27_2013_1249PM

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_MSC_VER)
#define HPXPI_ATTRIBUTE_NORETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define HPXPI_ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#else
#define HPXPI_ATTRIBUTE_NORETURN
#endif

#if defined(__cplusplus)
}
#endif

#endif
