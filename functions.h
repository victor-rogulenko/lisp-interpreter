#pragma once

#include <scope.h>
#include <parser.h>
#include <symbols.h>

/*************  Predicates  *************/
class IsNullPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsPairPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsNumberPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsBooleanPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsSymbolPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsListPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsEqualPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class IsIntegerEqualPred : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

/*************  Logical Operators  *************/
class LogicalNegation : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

/*************  Integer Functions  *************/
class AddInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class SubtractInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class MultiplyInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class DivideInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class EqualInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class GreaterInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class LessInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class GreaterEqualInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class LessEqualInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class MinInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class MaxInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class AbsInt : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

/*************  List Functions  *************/
class ConsList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class CarList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class CdrList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class SetCarList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class SetCdrList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListRefList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class ListTailList : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

/*************  Lambda Closure  *************/
class LambdaClosure : public Function {
public:
    LambdaClosure(const std::vector<std::string>& variables,
                  const std::vector<std::shared_ptr<Object>>& body,
                  const std::shared_ptr<Scope>& scope);

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    std::vector<std::string> variables_;
    std::vector<std::shared_ptr<Object>> body_;
    std::shared_ptr<Scope> local_scope_;
};
