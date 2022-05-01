#pragma once

#include <string>
#include <stdexcept>

struct SyntaxError : public std::runtime_error {
    explicit SyntaxError(const std::string& what);
};

struct NameError : public std::runtime_error {
    explicit NameError(const std::string& name);
};

struct RuntimeError : public std::runtime_error {
    explicit RuntimeError(const std::string& what);
};
