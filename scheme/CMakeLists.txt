if (TEST_SOLUTION)
  include_directories(../private/scheme-tokenizer)
  include_directories(../private/scheme-parser)
  include_directories(../private/scheme)

  add_library(libscheme
    ../private/scheme-parser/parser.cpp
    ../private/scheme/scheme.cpp)
else()
  include_directories(../scheme-tokenizer)
  include_directories(../scheme-parser)
  include_directories(../scheme)

  # Add your code to this library.
  add_library(libscheme
    ../scheme-parser/parser.cpp
    functions.cpp
    syntax.cpp
    symbols.cpp
    errors.cpp
    scope.cpp
    printer.cpp
    scheme.cpp)
endif()

add_executable(scheme-repl
  main.cpp)

target_link_libraries(scheme-repl
  libscheme)

add_catch(test_scheme
  test/test_boolean.cpp
  test/test_control_flow.cpp
  test/test_eval.cpp
  test/test_integer.cpp
  test/test_lambda.cpp
  test/test_list.cpp
  test/test_symbol.cpp
  test_extra_credit_eval.cpp
  SOLUTION_SRCS test/scheme_test.cpp)

target_link_libraries(test_scheme
  libscheme)
