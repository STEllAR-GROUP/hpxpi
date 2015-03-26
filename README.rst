.. Copyright (c) 2007-2014 Louisiana State University

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*******
 HPXPI
*******

HPXPI is an implementation of the 
`XPI specification <https://github.com/STEllAR-GROUP/hpxpi/blob/master/spec.pdf?raw=true>`_
on top of the `HPX runtime system <https://github.com/STEllAR-GROUP/hpx>`_.
It is currently based on the XPI document version r313.

The specification describes XPI as:

    XPI (eXtreme Parallex Interface) is a programming interface for parallel
    applications and systems based on the ParalleX execution model. XPI
    provides a simple abstraction layer to the family of ParalleX implementation
    HPX runtime system software. As HPX evolves, XPI insulates application
    codes from such changes, ensuring stability of experimental application
    codes. XPI serves both as a target for source-to-source compilers of 
    high-level languages and as a readible low-level programming interface
    syntax. XPI is experimental and supports current on-going sponsored research
    projects. Its long term future is entirely dependent on its resulting value;
    an unknown at this time. But it is motivated by a shortterm need to advance
    key project goals.

If you plan to use HPXPI we suggest to start with the latest released version
(currently HPXPI V0.1.0) which can be `downloaded here <http://stellar.cct.lsu.edu/downloads/>`_.

In publications this release of HPX can be cited as: |zenodo_doi|.

.. |zenodo_doi| image:: https://zenodo.org/badge/doi/10.5281/zenodo.16399.svg
     :target: http://dx.doi.org/10.5281/zenodo.16399

If you happen to run into problems we very much encourage and appreciate
any issue reports through the `issue tracker for this Github project
<http://github.com/STEllAR-GROUP/hpxpi/issues>`_.


********************
 Build Instructions
********************

All of HPXPI is distributed under the Boost Software License,
Version 1.0 (See accompanying file LICENSE_1_0.txt or an online copy available
`here <http://www.boost.org/LICENSE_1_0.txt>`_).

Since HPXPI is built on top of HPX, it needs to be installed before starting
to build HPXPI.

Linux
-----

1) Clone the master HPXPI git repository (or a stable tag)::

    $ git clone git://github.com/STEllAR-GROUP/hpxpi.git

2) Create a build directory. HPXPI requires an out-of-tree build. This means you
   will be unable to run CMake in the HPXPI source directory::

    $ cd hpxpi
    $ mkdir my_hpxpi_build
    $ cd my_hpxpi_build

3) Invoke CMake from your build directory, pointing the CMake driver to the root
   of your HPXPI source tree::

    $ cmake -DHPX_ROOT=/your_hpx_directory \
         -DCMAKE_INSTALL_PREFIX=/where_hpxpi_should_be_installed \
         [other CMake variable definitions] \
         /path/to/hpxpi/source/tree

4) Invoke GNU make. If you are on a machine with multiple cores (very likely),
   add the -jN flag to your make invocation, where N is the number of cores
   on your machine plus one::

    $ gmake -j5

5) To complete the build and install HPX::

    $ gmake install




******************
 Acknowledgements
******************

This work is supported by the National Science Foundation through awards 1117470 (APX)
and 1240655 (STAR). Any opinions, findings, and conclusions or recommendations expressed
in this material are those of the author(s) and do not necessarily reflect the views of
the National Science Foundation.

Additionally, this work is supported by the Department of Energy (DoE) 
through the award DE-SC0008714 (XPRESS). Any opinions, findings, and conclusions
or recommendations expressed in this material are those of the author(s) and do not 
necessarily reflect the views of the Department of Energy.

This work is also supported by the Center of Computation and Technology at
Louisiana State University.

