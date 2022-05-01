#include "functions.h"

/*************  Predicates  *************/
std::shared_ptr<Object> IsNullPred::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsNullPred: wrong number of arguments");
    }
    if (args[0] == nullptr) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    };
}

std::shared_ptr<Object> IsPairPred::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsPairPred: wrong number of arguments");
    }
    if (std::dynamic_pointer_cast<Cell>(args[0])) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    };
}

std::shared_ptr<Object> IsNumberPred::Apply(const std::shared_ptr<Scope>& scope,
                                            const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsNumberPred: wrong number of arguments");
    }
    if (std::dynamic_pointer_cast<Number>(args[0])) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    };
}

std::shared_ptr<Object> IsBooleanPred::Apply(const std::shared_ptr<Scope>& scope,
                                             const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsBooleanPred: wrong number of arguments");
    }
    auto symbol = std::dynamic_pointer_cast<Symbol>(args[0]);
    if (symbol && (symbol->GetName() == "#t" || symbol->GetName() == "#f")) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    }
}

std::shared_ptr<Object> IsSymbolPred::Apply(const std::shared_ptr<Scope>& scope,
                                            const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsSymbolPred: wrong number of arguments");
    }
    if (std::dynamic_pointer_cast<Symbol>(args[0])) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    }
}

std::shared_ptr<Object> IsListPred::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("IsListPred: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);

    if (!args[0]) {
        return std::make_shared<True>();
    } else if (!cell) {
        return std::make_shared<False>();
    } else {
        if (!std::dynamic_pointer_cast<Number>(cell->GetFirst())) {
            return std::make_shared<False>();
        }
        IsListPred is_list;
        std::vector<std::shared_ptr<Object>> new_args;
        new_args.push_back(cell->GetSecond());
        return is_list.Apply(scope, new_args);
    }
}

std::shared_ptr<Object> IsEqualPred::Apply(const std::shared_ptr<Scope>& scope,
                                           const std::vector<std::shared_ptr<Object>>& args) {

    throw std::runtime_error("IsEqualPred not implemented\n");
}

std::shared_ptr<Object> IsIntegerEqualPred::Apply(
    const std::shared_ptr<Scope>& scope, const std::vector<std::shared_ptr<Object>>& args) {

    throw std::runtime_error("IsIntegerEqualPred not implemented\n");
}

/*************  Logical Operators  *************/
std::shared_ptr<Object> LogicalNegation::Apply(const std::shared_ptr<Scope>& scope,
                                               const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("LogicalNegation: wrong number of arguments");
    }

    auto symbol = std::dynamic_pointer_cast<Symbol>(args[0]);
    if (symbol && (symbol->GetName() == "#f")) {
        return std::make_shared<True>();
    } else {
        return std::make_shared<False>();
    }
}

/*************  Integer Functions  *************/
std::shared_ptr<Object> AddInt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {

    int64_t value = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("+ arguments must be numbers");
        }

        value += number->GetValue();
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> SubtractInt::Apply(const std::shared_ptr<Scope>& scope,
                                           const std::vector<std::shared_ptr<Object>>& args) {

    if (args.empty()) {
        throw RuntimeError("- no arguments");
    }

    bool is_first = true;
    int64_t value = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("- arguments must be numbers");
        }

        if (is_first) {
            value += number->GetValue();
            is_first = false;
        } else {
            value -= number->GetValue();
        }
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> MultiplyInt::Apply(const std::shared_ptr<Scope>& scope,
                                           const std::vector<std::shared_ptr<Object>>& args) {

    int64_t value = 1;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        value *= number->GetValue();
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> DivideInt::Apply(const std::shared_ptr<Scope>& scope,
                                         const std::vector<std::shared_ptr<Object>>& args) {

    if (args.empty()) {
        throw RuntimeError("/ no arguments");
    }

    bool is_first = true;
    int64_t value = 1;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("/ arguments must be numbers");
        }

        if (is_first) {
            value *= number->GetValue();
            is_first = false;
        } else {
            value /= number->GetValue();
        }
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> EqualInt::Apply(const std::shared_ptr<Scope>& scope,
                                        const std::vector<std::shared_ptr<Object>>& args) {

    bool first = true;
    int64_t prev_number = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        if (!first && prev_number != number->GetValue()) {
            return std::make_shared<False>();
        }

        prev_number = number->GetValue();
        first = false;
    }

    return std::make_shared<True>();
}

std::shared_ptr<Object> GreaterInt::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    bool first = true;
    int64_t prev_number = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        if (!first && prev_number <= number->GetValue()) {
            return std::make_shared<False>();
        }

        prev_number = number->GetValue();
        first = false;
    }

    return std::make_shared<True>();
}

std::shared_ptr<Object> LessInt::Apply(const std::shared_ptr<Scope>& scope,
                                       const std::vector<std::shared_ptr<Object>>& args) {

    bool first = true;
    int64_t prev_number = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        if (!first && prev_number >= number->GetValue()) {
            return std::make_shared<False>();
        }

        prev_number = number->GetValue();
        first = false;
    }

    return std::make_shared<True>();
}

std::shared_ptr<Object> GreaterEqualInt::Apply(const std::shared_ptr<Scope>& scope,
                                               const std::vector<std::shared_ptr<Object>>& args) {

    bool first = true;
    int64_t prev_number = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        if (!first && prev_number < number->GetValue()) {
            return std::make_shared<False>();
        }

        prev_number = number->GetValue();
        first = false;
    }

    return std::make_shared<True>();
}

std::shared_ptr<Object> LessEqualInt::Apply(const std::shared_ptr<Scope>& scope,
                                            const std::vector<std::shared_ptr<Object>>& args) {

    bool first = true;
    int64_t prev_number = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("* arguments must be numbers");
        }

        if (!first && prev_number > number->GetValue()) {
            return std::make_shared<False>();
        }

        prev_number = number->GetValue();
        first = false;
    }

    return std::make_shared<True>();
}

std::shared_ptr<Object> MinInt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError("min no arguments");
    }

    bool first = true;
    int64_t value = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("min arguments must be numbers");
        }

        if (first || value > number->GetValue()) {
            value = number->GetValue();
            first = false;
        }
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> MaxInt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError("max no arguments");
    }

    bool first = true;
    int64_t value = 0;
    for (const auto& arg : args) {
        auto new_element = arg->Eval(scope);
        auto number = std::dynamic_pointer_cast<Number>(new_element);
        if (!number) {
            throw RuntimeError("max arguments must be numbers");
        }

        if (first || value < number->GetValue()) {
            value = number->GetValue();
            first = false;
        }
    }

    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> AbsInt::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("AbsInt: wrong number of arguments");
    }

    auto new_element = args[0]->Eval(scope);
    auto number = std::dynamic_pointer_cast<Number>(new_element);
    if (!number) {
        throw RuntimeError("max arguments must be numbers");
    }

    auto value = number->GetValue();
    if (value < 0) {
        value *= -1;
    }

    return std::make_shared<Number>(value);
}

/*************  List Functions  *************/
std::shared_ptr<Object> ConsList::Apply(const std::shared_ptr<Scope>& scope,
                                        const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw RuntimeError("ConsList: wrong number of arguments");
    }

    return std::make_shared<Cell>(args[0], args[1]);
}

std::shared_ptr<Object> CarList::Apply(const std::shared_ptr<Scope>& scope,
                                       const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("CarList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("CarList: argument must be a cell");
    }

    return cell->GetFirst();
}

std::shared_ptr<Object> CdrList::Apply(const std::shared_ptr<Scope>& scope,
                                       const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 1) {
        throw RuntimeError("CdrList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("CdrList: argument must be a cell");
    }

    return cell->GetSecond();
}

std::shared_ptr<Object> SetCarList::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw RuntimeError("SetCarList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("SetCarList: argument must be a cell");
    }

    cell->SetFirst(args[1]);
    return cell->GetFirst();
}

std::shared_ptr<Object> SetCdrList::Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw RuntimeError("SetCdrList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("SetCdrList: argument must be a cell");
    }

    cell->SetSecond(args[1]);
    return cell->GetSecond();
}

std::shared_ptr<Object> ListList::Apply(const std::shared_ptr<Scope>& scope,
                                        const std::vector<std::shared_ptr<Object>>& args) {

    if (args.empty()) {
        return nullptr;
    }

    ListList new_list;
    auto new_args = std::vector(args.begin() + 1, args.end());
    return std::make_shared<Cell>(args[0], new_list.Apply(scope, new_args));
}

std::shared_ptr<Object> ListRefList::Apply(const std::shared_ptr<Scope>& scope,
                                           const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw RuntimeError("ListRefList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("ListRefList: first argument must be a cell");
    }

    auto number = std::dynamic_pointer_cast<Number>(args[1]);
    int64_t counter = 0;
    if (!number) {
        throw RuntimeError("ListRefList: second argument must be a number");
    }
    counter = number->GetValue();

    while (counter > 0 && cell) {
        auto new_cell = cell->GetSecond();
        cell = std::dynamic_pointer_cast<Cell>(new_cell);
        --counter;
    }

    if (!cell) {
        throw RuntimeError("ListRefList: wrong reference");
    }

    return cell->GetFirst();
}

std::shared_ptr<Object> ListTailList::Apply(const std::shared_ptr<Scope>& scope,
                                            const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != 2) {
        throw RuntimeError("ListTailList: wrong number of arguments");
    }

    auto cell = std::dynamic_pointer_cast<Cell>(args[0]);
    if (!cell) {
        throw RuntimeError("ListTailList: first argument must be a cell");
    }

    auto number = std::dynamic_pointer_cast<Number>(args[1]);
    int64_t counter = 0;
    if (!number) {
        throw RuntimeError("ListTailList: second argument must be a number");
    }
    counter = number->GetValue();

    auto next_cell = cell->GetSecond();
    std::vector<std::shared_ptr<Object>> results{args[0], next_cell};

    while (next_cell) {
        cell = std::dynamic_pointer_cast<Cell>(next_cell);
        next_cell = cell->GetSecond();
        results.push_back(next_cell);
    }

    if (counter < results.size()) {
        return results[counter];
    } else {
        throw RuntimeError("ListTailList: wrong reference");
    }
}

/*************  Lambda Closure  *************/
LambdaClosure::LambdaClosure(const std::vector<std::string>& variables,
                             const std::vector<std::shared_ptr<Object>>& body,
                             const std::shared_ptr<Scope>& previous_scope)
    : variables_(variables),
      body_(body),
      local_scope_(std::make_shared<Scope>(Scope(previous_scope))) {
}

std::shared_ptr<Object> LambdaClosure::Apply(const std::shared_ptr<Scope>& scope,
                                             const std::vector<std::shared_ptr<Object>>& args) {

    if (args.size() != variables_.size()) {
        throw RuntimeError("LambdaClosure: wrong number of arguments");
    }

    for (size_t it = 0; it < args.size(); ++it) {
        local_scope_->Insert(variables_[it], args[it]);
    }

    std::shared_ptr<Object> result;
    for (auto& cell : body_) {
        result = cell->Eval(local_scope_);
    }

    return result;
}
