#pragma once

#include <scope.h>
#include <parser.h>
#include <symbols.h>
#include <functions.h>

class IfSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class QuoteSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class LambdaSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class AndSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class OrSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class DefineSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class SetSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};

class EvalSynt : public Syntax {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
};
