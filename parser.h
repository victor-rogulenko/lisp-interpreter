#pragma once

#include <vector>
#include <cassert>
#include <memory>

#include <tokenizer.h>
#include <scope.h>
#include <errors.h>
#include <printer.h>

class Scope;

enum class ObjectType { NUMBER, CELL, SYMBOL, FUNCTION, SYNTAX };

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) = 0;
    virtual void PrintObjectToOstream(std::ostream* out) = 0;
    virtual bool IsFalse() const;
    virtual ~Object() = default;
    Object(ObjectType type);
    ObjectType GetType();

private:
    const ObjectType type_;
};

class Number : public Object {
public:
    std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>&) override;
    void PrintObjectToOstream(std::ostream* out) override;
    explicit Number(int64_t number);
    int64_t GetValue() const;

private:
    int64_t value_;
};

class Symbol : public Object {
public:
    std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;
    void PrintObjectToOstream(std::ostream* out) override;
    explicit Symbol(std::string name);
    std::string GetName() const;

private:
    std::string name_;
};

class Function : public Object {
public:
    std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;
    void PrintObjectToOstream(std::ostream* out) override;
    Function();

    virtual std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& arg) = 0;
};

class Syntax : public Object {
public:
    std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;
    void PrintObjectToOstream(std::ostream* out) override;
    Syntax();

    virtual std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& arg) = 0;
};

class Cell : public Object {
public:
    std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;
    void PrintObjectToOstream(std::ostream* out) override;
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second);
    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;
    void SetFirst(const std::shared_ptr<Object>& new_first);
    void SetSecond(const std::shared_ptr<Object>& new_second);

private:
    std::shared_ptr<Object> first_, second_;
};

void PrintTo(const std::shared_ptr<Object>& obj, std::ostream* out);

std::vector<std::shared_ptr<Object>> ToVector(const std::shared_ptr<Object>& head);

bool IsNumber(const std::shared_ptr<Object>& obj);
bool IsCell(const std::shared_ptr<Object>& obj);
bool IsSymbol(const std::shared_ptr<Object>& obj);
bool IsBracketClose(Token tok);
bool IsDot(Token tok);

// GRAMMAR:
// <expr>     :: <item> { <list>}
// <list>     :: () | (<expr> ... <expr) | (<expr> ... <expr> . <list>)

//  Read an arbitrary expression
std::shared_ptr<Object> Read(Tokenizer* tokenizer);

//  Read a list, a pair, or a list with a dot at the end
std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);
