#include "scheme.h"

Scheme::Scheme() : global_scope_(std::make_shared<Scope>()) {
    /*************  Symbols  *************/
    global_scope_->Insert("#t", std::make_shared<True>());
    global_scope_->Insert("#f", std::make_shared<False>());

    /*************  Syntax  *************/
    global_scope_->Insert("if", std::make_shared<IfSynt>());
    global_scope_->Insert("quote", std::make_shared<QuoteSynt>());
    global_scope_->Insert("lambda", std::make_shared<LambdaSynt>());
    global_scope_->Insert("and", std::make_shared<AndSynt>());
    global_scope_->Insert("or", std::make_shared<OrSynt>());
    global_scope_->Insert("define", std::make_shared<DefineSynt>());
    global_scope_->Insert("set!", std::make_shared<SetSynt>());
    global_scope_->Insert("eval", std::make_shared<EvalSynt>());

    /*************  Predicates  *************/
    global_scope_->Insert("null?", std::make_shared<IsNullPred>());
    global_scope_->Insert("pair?", std::make_shared<IsPairPred>());
    global_scope_->Insert("number?", std::make_shared<IsNumberPred>());
    global_scope_->Insert("boolean?", std::make_shared<IsBooleanPred>());
    global_scope_->Insert("symbol?", std::make_shared<IsSymbolPred>());
    global_scope_->Insert("list?", std::make_shared<IsListPred>());
    global_scope_->Insert("eq?", std::make_shared<IsEqualPred>());
    global_scope_->Insert("equal?", std::make_shared<IsEqualPred>());
    global_scope_->Insert("integer-equal?", std::make_shared<IsIntegerEqualPred>());

    /*************  Logical Operators  *************/
    global_scope_->Insert("not", std::make_shared<LogicalNegation>());

    /*************  Integer Functions  *************/
    global_scope_->Insert("+", std::make_shared<AddInt>());
    global_scope_->Insert("-", std::make_shared<SubtractInt>());
    global_scope_->Insert("*", std::make_shared<MultiplyInt>());
    global_scope_->Insert("/", std::make_shared<DivideInt>());

    global_scope_->Insert("=", std::make_shared<EqualInt>());
    global_scope_->Insert(">", std::make_shared<GreaterInt>());
    global_scope_->Insert("<", std::make_shared<LessInt>());
    global_scope_->Insert(">=", std::make_shared<GreaterEqualInt>());
    global_scope_->Insert("<=", std::make_shared<LessEqualInt>());

    global_scope_->Insert("min", std::make_shared<MinInt>());
    global_scope_->Insert("max", std::make_shared<MaxInt>());
    global_scope_->Insert("abs", std::make_shared<AbsInt>());

    /*************  List Functions  *************/
    global_scope_->Insert("cons", std::make_shared<ConsList>());
    global_scope_->Insert("car", std::make_shared<CarList>());
    global_scope_->Insert("cdr", std::make_shared<CdrList>());
    global_scope_->Insert("set-car!", std::make_shared<SetCarList>());
    global_scope_->Insert("set-cdr!", std::make_shared<SetCdrList>());
    global_scope_->Insert("list", std::make_shared<ListList>());
    global_scope_->Insert("list-ref", std::make_shared<ListRefList>());
    global_scope_->Insert("list-tail", std::make_shared<ListTailList>());
}

std::shared_ptr<Object> Scheme::ReadCommand(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    auto result = Read(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("tokenizer: end of tokenizer expected");
    } else {
        return result;
    }
}

std::shared_ptr<Object> Scheme::Eval(const std::shared_ptr<Object>& in) {
    if (!in) {
        throw RuntimeError("scheme: no objects to evaluate");
    } else {
        std::vector<std::shared_ptr<Object>> in_as_vector{in};
        IsListPred is_list;
        auto check_list = is_list.Apply(global_scope_, in_as_vector);
        if (std::dynamic_pointer_cast<True>(check_list)) {
            throw RuntimeError("scheme: lists are not self-evaluating");
        } else {
            return in->Eval(global_scope_);
        }
    }
}

Scheme::~Scheme() {
    global_scope_->Clear();  // TODO create proper garbage collector
}
