#pragma once

#include <scope.h>
#include <parser.h>

class True : public Symbol {
public:
    True();
};

class False : public Symbol {
public:
    False();
    bool IsFalse() const override;
};
