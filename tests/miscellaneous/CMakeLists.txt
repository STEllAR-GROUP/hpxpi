# Copyright (c) 2011 Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(miscellaneous_tests
    version)

set(version_FLAGS DEPENDENCIES iostreams_component)

add_definitions("-DHPXPI_APPLICATION_EXPORTS")

foreach(miscellaneous ${miscellaneous_tests})
  set(sources
      ${miscellaneous}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${miscellaneous}
                     SOURCES ${sources}
                     ${${benchmark}_FLAGS}
                     FOLDER "Tests/Miscellaneous/${miscellaneous}"
                     DEPENDENCIES hpxpi_lib)

  # add a custom target for this example
  add_hpx_pseudo_target(tests.miscellaneous.${miscellaneous})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.miscellaneous
                              tests.miscellaneous.${miscellaneous})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.miscellaneous.${miscellaneous}
                              ${miscellaneous}_exe)
endforeach()

