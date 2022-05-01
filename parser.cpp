#include <parser.h>

/*************  Object  *************/
bool Object::IsFalse() const {
    return false;
}

Object::Object(ObjectType type) : type_(type) {
}

ObjectType Object::GetType() {
    return type_;
}

/*************  Number  *************/
std::shared_ptr<Object> Number::Eval(const std::shared_ptr<Scope>&) {
    return shared_from_this();
};

void Number::PrintObjectToOstream(std::ostream* out) {
    *out << value_;
}

Number::Number(int64_t number) : Object(ObjectType::NUMBER), value_(number) {
}

int64_t Number::GetValue() const {
    return value_;
}

/*************  Symbol  *************/
std::shared_ptr<Object> Symbol::Eval(const std::shared_ptr<Scope>& scope) {
    return scope->Lookup(name_);
}

void Symbol::PrintObjectToOstream(std::ostream* out) {
    *out << name_;
}

Symbol::Symbol(std::string name) : Object(ObjectType::SYMBOL), name_(std::move(name)) {
}

std::string Symbol::GetName() const {
    return name_;
}

/*************  Function  *************/
std::shared_ptr<Object> Function::Eval(const std::shared_ptr<Scope>& scope) {
    throw RuntimeError("cannot evaluate a function");
}

void Function::PrintObjectToOstream(std::ostream* out) {
    *out << "<function>";
}

Function::Function() : Object(ObjectType::FUNCTION) {
}

/*************  Syntax  *************/
std::shared_ptr<Object> Syntax::Eval(const std::shared_ptr<Scope>& scope) {
    throw RuntimeError("cannot evaluate a syntax");
}

void Syntax::PrintObjectToOstream(std::ostream* out) {
    *out << "<syntax>";
}

Syntax::Syntax() : Object(ObjectType::SYNTAX) {
}

/*************  Cell  *************/
std::shared_ptr<Object> Cell::Eval(const std::shared_ptr<Scope>& scope) {
    auto tfn = first_->Eval(scope);
    auto fn = std::dynamic_pointer_cast<Function>(tfn);
    auto syntax = std::dynamic_pointer_cast<Syntax>(tfn);

    if (!fn && !syntax && second_ == nullptr) {
        return tfn;
    }

    if (!fn && !syntax) {
        //  Extra check for a lambda function;
        tfn = tfn->Eval(scope);
        fn = std::dynamic_pointer_cast<Function>(tfn);
        if (!fn) {
            throw RuntimeError(
                "list: for 1st element, expected a function or "
                "a syntax; got: " +
                Print(tfn));
        }
    }

    std::vector<std::shared_ptr<Object>> args = ToVector(second_);
    if (fn) {
        for (auto& arg : args) {
            arg = arg->Eval(scope);
        }
        return fn->Apply(scope, args);
    } else {
        return syntax->Apply(scope, args);
    }
}

void Cell::PrintObjectToOstream(std::ostream* out) {
    *out << "(";
    PrintTo(first_, out);

    auto current = std::dynamic_pointer_cast<Cell>(shared_from_this());
    auto next = std::dynamic_pointer_cast<Cell>(second_);
    while (next) {
        *out << " ";
        PrintTo(next->GetFirst(), out);
        current = next;
        next = std::dynamic_pointer_cast<Cell>(next->GetSecond());
    }

    if (current->GetSecond()) {
        *out << " . ";
        PrintTo(current->GetSecond(), out);
    }

    *out << ")";
}

Cell::Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
    : Object(ObjectType::CELL), first_(std::move(first)), second_(std::move(second)) {
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}

std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

void Cell::SetFirst(const std::shared_ptr<Object>& new_first) {
    first_ = new_first;
}

void Cell::SetSecond(const std::shared_ptr<Object>& new_second) {
    second_ = new_second;
}

/*************  Helper functions  *************/
std::vector<std::shared_ptr<Object>> ToVector(const std::shared_ptr<Object>& head) {
    std::vector<std::shared_ptr<Object>> elements;
    if (head == nullptr) {
        return elements;
    }

    if (IsCell(head)) {
        auto current = std::dynamic_pointer_cast<Cell>(head);
        auto vec_tail = ToVector(current->GetSecond());
        elements.push_back(current->GetFirst());
        elements.insert(elements.end(), vec_tail.begin(), vec_tail.end());
    } else {
        elements.push_back(head);
    }

    return elements;
}

bool IsNumber(const std::shared_ptr<Object>& obj) {
    return obj->GetType() == ObjectType::NUMBER;
}

bool IsCell(const std::shared_ptr<Object>& obj) {
    return obj->GetType() == ObjectType::CELL;
}

bool IsSymbol(const std::shared_ptr<Object>& obj) {
    return obj->GetType() == ObjectType::SYMBOL;
}

bool IsBracketClose(Token tok) {
    if (BracketToken* brac = std::get_if<BracketToken>(&tok)) {
        if (*brac == BracketToken::CLOSE) {
            return true;
        }
    }
    return false;
}

bool IsDot(Token tok) {
    if (DotToken* dot = std::get_if<DotToken>(&tok)) {
        return true;
    }
    return false;
}

// GRAMMAR:
// <expr>     :: <item> { <list>}
// <list>     :: () | (<expr> ... <expr) | (<expr> ... <expr> . <list>)

//  Read an arbitrary expression
std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    auto tok = tokenizer->GetToken();
    tokenizer->Next();

    if (ConstantToken* num = std::get_if<ConstantToken>(&tok)) {
        return std::make_shared<Number>(num->value);

    } else if (SymbolToken* symb = std::get_if<SymbolToken>(&tok)) {
        return std::make_shared<Symbol>(symb->name);

    } else if (BracketToken* brac = std::get_if<BracketToken>(&tok)) {
        if (*brac == BracketToken::OPEN) {
            return ReadList(tokenizer);
        } else {
            throw SyntaxError{"Incorrect expression: misplaced )"};
        }
    } else if (std::get_if<QuoteToken>(&tok)) {
        auto new_cell = std::make_shared<Cell>(Read(tokenizer), nullptr);
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"), new_cell);
    } else {

        throw SyntaxError{
            "Incorrect expression: number, symbol, ' or ( "
            "expected"};
    }
}

//  Read a list, a pair, or a list with a dot at the end
std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Incorrect list: premature end of stream"};
    }

    auto tok = tokenizer->GetToken();

    //  Empty list
    if (IsBracketClose(tok)) {
        tokenizer->Next();
        return nullptr;
    }

    auto left = Read(tokenizer);
    tok = tokenizer->GetToken();

    auto result = std::make_shared<Cell>(left, nullptr);
    auto previous = result;

    while (!IsDot(tok) && !IsBracketClose(tok) && !tokenizer->IsEnd()) {
        auto new_expression = Read(tokenizer);
        auto new_cell = std::make_shared<Cell>(new_expression, nullptr);
        previous->SetSecond(new_cell);
        previous = new_cell;
        tok = tokenizer->GetToken();
    }

    //  End of list in reduced form
    if (IsBracketClose(tok)) {
        tokenizer->Next();
        return result;
    }

    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Incorrect list: premature end"};
    }

    //  tok is dot
    tokenizer->Next();
    auto right = Read(tokenizer);

    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Incorrect list: premature end after dot"};
    }

    tok = tokenizer->GetToken();
    tokenizer->Next();

    if (!IsBracketClose(tok)) {
        throw SyntaxError{"Incorrect list: ) is missing"};
    }

    if (previous == result) {  //  Only one expression before the dot
        return std::make_shared<Cell>(std::move(left), std::move(right));
    } else {
        previous->SetSecond(right);
        return result;
    }
}
