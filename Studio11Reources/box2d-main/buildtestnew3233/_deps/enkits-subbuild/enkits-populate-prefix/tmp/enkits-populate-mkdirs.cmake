# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-src")
  file(MAKE_DIRECTORY "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-build"
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix"
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/tmp"
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/src/enkits-populate-stamp"
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/src"
  "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/src/enkits-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/src/enkits-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/747jf/Downloads/Studio11Reources/box2d-main/buildtestnew3233/_deps/enkits-subbuild/enkits-populate-prefix/src/enkits-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
