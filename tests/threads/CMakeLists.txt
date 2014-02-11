# Copyright (c) 2013 Bryce Adelstein-Lelbach, Alexander Duchene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(actions_tests
    continue
    get_parcel)

set(version_FLAGS DEPENDENCIES iostreams_component)

add_definitions("-DHPXPI_APPLICATION_EXPORTS")

foreach(actions ${actions_tests})
  set(sources
      ${actions}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${actions}
                     SOURCES ${sources}
                     ${${benchmark}_FLAGS}
                     FOLDER "Tests/actions/${actions}"
                     DEPENDENCIES hpxpi_lib)

  # add a custom target for this example
  add_hpx_pseudo_target(tests.actions.${actions})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.actions
                              tests.actions.${actions})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.actions.${actions}
                              ${actions}_exe)
endforeach()

