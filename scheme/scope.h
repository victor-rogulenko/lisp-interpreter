#pragma once

#include <string>
#include <unordered_map>

#include <parser.h>

class Object;

class Scope {
public:
    Scope();
    Scope(const std::shared_ptr<Scope>& previous);
    std::shared_ptr<Object> LookupInCurrentScope(const std::string& name) const;
    std::shared_ptr<Scope> GetPreviousScope() const;
    std::shared_ptr<Object> Lookup(const std::string& name) const;
    void Insert(const std::string& name, const std::shared_ptr<Object>& value);
    void Set(const std::string& name, const std::shared_ptr<Object>& value);
    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Object>> variables_;
    std::shared_ptr<Scope> previous_ = nullptr;
};
