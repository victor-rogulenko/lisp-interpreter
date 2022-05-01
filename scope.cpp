#include "scope.h"

Scope::Scope() : previous_(nullptr) {
}

Scope::Scope(const std::shared_ptr<Scope>& previous) : previous_(previous) {
}

std::shared_ptr<Object> Scope::LookupInCurrentScope(const std::string& name) const {
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<Scope> Scope::GetPreviousScope() const {
    return previous_;
};

std::shared_ptr<Object> Scope::Lookup(const std::string& name) const {
    auto result = LookupInCurrentScope(name);
    auto previous_scope = previous_;

    while (!result && previous_scope) {
        result = previous_scope->LookupInCurrentScope(name);
        previous_scope = previous_scope->GetPreviousScope();
    }

    if (!result) {
        throw NameError(name);
    } else {
        return result;
    }
}

void Scope::Insert(const std::string& name, const std::shared_ptr<Object>& value) {
    variables_[name] = value;
}

void Scope::Set(const std::string& name, const std::shared_ptr<Object>& value) {
    auto result = LookupInCurrentScope(name);
    if (result) {
        Insert(name, value);
        return;
    }

    auto previous_scope = previous_;
    while (!result && previous_scope) {
        result = previous_scope->LookupInCurrentScope(name);
        if (result) {
            previous_scope->Insert(name, value);
            return;
        }
        previous_scope = previous_scope->GetPreviousScope();
    }

    throw NameError(name);
}

void Scope::Clear() {
    variables_.clear();
}
