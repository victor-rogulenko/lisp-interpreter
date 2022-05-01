#pragma once

#include <tokenizer.h>
#include <parser.h>
#include <functions.h>
#include <syntax.h>
#include <symbols.h>
#include "scope.h"
#include "errors.h"
#include "printer.h"

class Scheme {
public:
    Scheme();
    std::shared_ptr<Object> ReadCommand(const std::string& str);
    std::shared_ptr<Object> Eval(const std::shared_ptr<Object>& in);
    ~Scheme();

private:
    std::shared_ptr<Scope> global_scope_;
};
