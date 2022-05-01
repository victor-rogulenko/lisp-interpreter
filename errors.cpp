#include "errors.h"

SyntaxError::SyntaxError(const std::string& what) : std::runtime_error("Syntax error: " + what) {
}

NameError::NameError(const std::string& name) : std::runtime_error("Variable not found: " + name) {
}

RuntimeError::RuntimeError(const std::string& what) : std::runtime_error("Runtime error: " + what) {
}
