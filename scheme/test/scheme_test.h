#pragma once

#include <catch.hpp>
#include <scheme.h>

struct SchemeTest {
    Scheme scheme;

    SchemeTest() {
    }

    // Implement following methods.
    void ExpectEq(std::string expression, std::string result) {
        // Silence unused variable warning. Remove this code.
        auto parsed_expression = scheme.ReadCommand(expression);
        auto evaluated_expression = scheme.Eval(parsed_expression);
        auto printed_expression = Print(evaluated_expression);

        REQUIRE(printed_expression == result);
    }

    void ExpectNoError(std::string expression) {
        REQUIRE_NOTHROW(scheme.ReadCommand(expression));
        std::shared_ptr<Object> parsed_expression = scheme.ReadCommand(expression);
        REQUIRE_NOTHROW(scheme.Eval(parsed_expression));
    }

    void ExpectSyntaxError(std::string expression) {
        std::shared_ptr<Object> parsed_expression;
        REQUIRE_THROWS_AS(scheme.Eval(scheme.ReadCommand(expression)), SyntaxError);
    }

    void ExpectRuntimeError(std::string expression) {
        auto parsed_expression = scheme.ReadCommand(expression);
        REQUIRE_THROWS_AS(scheme.Eval(parsed_expression), RuntimeError);
    }

    void ExpectNameError(std::string expression) {
        auto parsed_expression = scheme.ReadCommand(expression);
        REQUIRE_THROWS_AS(scheme.Eval(parsed_expression), NameError);
    }
};
