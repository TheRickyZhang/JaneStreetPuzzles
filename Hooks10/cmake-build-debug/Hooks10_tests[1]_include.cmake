if(EXISTS "C:/Users/ricky/Projects/JaneStreet/Hooks10/cmake-build-debug/Hooks10_tests[1]_tests.cmake")
  include("C:/Users/ricky/Projects/JaneStreet/Hooks10/cmake-build-debug/Hooks10_tests[1]_tests.cmake")
else()
  add_test(Hooks10_tests_NOT_BUILT Hooks10_tests_NOT_BUILT)
endif()
