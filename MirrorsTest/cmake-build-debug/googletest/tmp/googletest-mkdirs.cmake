# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest-build"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/tmp"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest-stamp"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src"
  "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/ricky/Projects/JaneStreet/MirrorsTest/cmake-build-debug/googletest/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
