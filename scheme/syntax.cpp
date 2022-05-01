#include "syntax.h"

std::shared_ptr<Object> IfSynt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() < 2 || args.size() > 3) {
        throw SyntaxError("if: wrong number of arguments: " + std::to_string(args.size()));
    }

    auto condition = args[0];
    auto true_branch = args[1];
    std::shared_ptr<Object> false_branch = nullptr;

    if (args.size() == 3) {
        false_branch = args[2];
    }

    auto result = condition->Eval(scope);
    if (result && !result->IsFalse()) {
        return true_branch->Eval(scope);
    } else if (false_branch) {
        return false_branch->Eval(scope);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> QuoteSynt::Apply(const std::shared_ptr<Scope>& scope,
                                         const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("quote: wrong number of arguments: " + std::to_string(args.size()));
    }

    return args[0];
}

std::shared_ptr<Object> LambdaSynt::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.empty()) {
        throw SyntaxError("lambda is empty");
    }

    //  Parse names of variables
    std::vector<std::string> variables;
    if (args[0]) {
        auto variables_as_objects = ToVector(args[0]);
        for (auto& var : variables_as_objects) {
            auto name = std::dynamic_pointer_cast<Symbol>(var);
            if (!var) {
                throw SyntaxError("lambda variables must be symbols");
            } else {
                variables.push_back(name->GetName());
            }
        }
    }

    //  Save function body
    std::vector<std::shared_ptr<Object>> body;
    if (args.size() < 2) {
        throw SyntaxError("lambda is missing function body");
    } else {
        body = std::vector(args.begin() + 1, args.end());
    }

    //  Create LambdaClosure using names of variables, function body,
    //  and its scope
    auto result = LambdaClosure(variables, body, scope);
    return std::make_shared<LambdaClosure>(result);
}

std::shared_ptr<Object> AndSynt::Apply(const std::shared_ptr<Scope>& scope,
                                       const std::vector<std::shared_ptr<Object>>& args) {

    for (auto& arg : args) {
        auto current = arg->Eval(scope);
        if (current->IsFalse()) {
            return std::make_shared<False>();
        }
    }

    if (args.empty()) {
        return std::make_shared<True>();
    } else {
        return args.back()->Eval(scope);
    }
}

std::shared_ptr<Object> OrSynt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {

    for (auto& arg : args) {
        auto current = arg->Eval(scope);
        if (!current->IsFalse()) {
            return current;
        }
    }

    if (args.empty()) {
        return std::make_shared<False>();
    } else {
        return args.back()->Eval(scope);
    }
}

std::shared_ptr<Object> DefineSynt::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw SyntaxError("define: wrong number of arguments: " + std::to_string(args.size()));
    }

    auto name = std::dynamic_pointer_cast<Symbol>(args[0]);
    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (cell) {
        name = std::dynamic_pointer_cast<Symbol>(cell->GetFirst());
    }

    std::shared_ptr<Object> result;
    if (!name && !cell) {
        throw SyntaxError("define: first argument must be a symbol or a cell");
    } else if (name && !cell) {
        result = args[1]->Eval(scope);
    } else {
        //  Deal with short syntax for a lambda function
        std::vector<std::shared_ptr<Object>> new_args;
        new_args.push_back(cell->GetSecond());
        new_args.push_back(args[1]);
        LambdaSynt lambda;
        result = lambda.Apply(scope, new_args);
    }

    scope->Insert(name->GetName(), result);
    return result;
}

std::shared_ptr<Object> SetSynt::Apply(const std::shared_ptr<Scope>& scope,
                                       const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw SyntaxError("set: wrong number of arguments: " + std::to_string(args.size()));
    }

    auto name = std::dynamic_pointer_cast<Symbol>(args[0]);
    if (!name) {
        throw SyntaxError("set: first argument must be a symbol");
    }

    auto result = args[1]->Eval(scope);
    scope->Set(name->GetName(), result);
    return result;
}

std::shared_ptr<Object> EvalSynt::Apply(const std::shared_ptr<Scope>& scope,
                                        const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw SyntaxError("EvalSynt: wrong number of arguments: " + std::to_string(args.size()));
    }

    auto evaluated = args[0]->Eval(scope);
    return evaluated->Eval(scope);
}
