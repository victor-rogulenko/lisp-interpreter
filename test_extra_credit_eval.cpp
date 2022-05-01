#include <test/scheme_test.h>

TEST_CASE_METHOD(SchemeTest, "Eval1") {
    ExpectNoError("(define x '(+ 2 3))");
    ExpectEq("(eval x)", "5");
}

TEST_CASE_METHOD(SchemeTest, "Eval2") {
    ExpectEq("(eval(+ 2 4))", "6");
    ExpectRuntimeError("(eval ('(1 2)) )");
    ExpectRuntimeError("(eval( eval( '(1 2) ) ))");
}
