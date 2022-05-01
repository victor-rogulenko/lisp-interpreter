#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>  // std::isdigit
#include <memory>
#include <variant>

struct NullToken {};

struct SymbolToken {
    std::string name;
};

struct QuoteToken {};

struct DotToken {};

// Множество скобок ограничено и известно на этапе компиляции.
// Используем enum class.
enum class BracketToken { OPEN, CLOSE };

// Константа - это просто значение. Заводим структуру.
struct ConstantToken {
    int value;
};

// Чтобы следовать принципу DRY, заводим typedef.
// Когда в будущем добавится новый вариант токена, будет
// достаточно поменять определение в одном месте.
typedef std::variant<NullToken, SymbolToken, QuoteToken, DotToken, BracketToken, ConstantToken> Token;

inline bool operator==(NullToken, NullToken) {
    return true;
}

inline bool operator==(SymbolToken lhs, SymbolToken rhs) {
    return lhs.name == rhs.name;
}

inline bool operator==(QuoteToken, QuoteToken) {
    return true;
}

inline bool operator==(DotToken, DotToken) {
    return true;
}

inline bool operator==(ConstantToken lhs, ConstantToken rhs) {
    return lhs.value == rhs.value;
}

inline bool CheckSymbol(char ch) {
    return (ch != '.' && ch != '\'' && ch != '(' && ch != ')' && ch != ' ' &&
            ch != '\n');
}

class Tokenizer {
public:
    // Создаёт токенизатор читающий символы из потока in.
    Tokenizer(std::istream* in) : in_(in) {
        Next();
    }

    // Достигли мы конца потока или нет.
    bool IsEnd() {
        return is_end_;
    }

    // Попытаться прочитать следующий токен.
    // Либо IsEnd() станет false, либо токен можно будет получить через Token().
    void Next() {
        int next_item = in_->peek();

        //  Skip spaces
        while (next_item != EOF && (static_cast<char>(next_item) == ' ' ||
               static_cast<char>(next_item) == '\n')) {
            char ch;
            in_->get(ch);
            next_item = in_->peek();
        }

        // Deal with end of stream
        if (next_item == EOF) {
            is_end_ = true;
            token_ = NullToken{};
            return;
        }

        //  Deal with positive numbers
        if (std::isdigit(next_item)) {
            int number;
            *in_ >> number;
            token_ = ConstantToken{number};
            possible_unary_sign_ = false;
            return;
        }

        //  Deal with all non-number single symbols
        char ch;
        in_->get(ch);
        next_item = in_->peek();

        if (ch == '\'') {
            token_ = QuoteToken{};
            possible_unary_sign_ = false;
            return;
        }

        if (ch == '(') {
            token_ = BracketToken::OPEN;
            possible_unary_sign_ = true;
            return;
        }

        if (ch == ')') {
            token_ = BracketToken::CLOSE;
            possible_unary_sign_ = false;
            return;
        }

        if (ch == '.') {
            token_ = DotToken{};
            possible_unary_sign_ = false;
            return;
        }

        if (ch == '-' && possible_unary_sign_ && std::isdigit(next_item)) {
            int number;
            *in_ >> number;
            token_ = ConstantToken{(-1) * number};
            possible_unary_sign_ = false;
            return;
        }

        if (ch == '+' && possible_unary_sign_ && std::isdigit(next_item)) {
            int number;
            *in_ >> number;
            token_ = ConstantToken{number};
            possible_unary_sign_ = false;
            return;
        }

        if (ch == '+' || ch == '-' || ch == '*') {
            token_ = SymbolToken{std::string(1, ch)};
            possible_unary_sign_ = false;
            return;
        }

        //  Deal with multichar operators
        std::string buffer;
        buffer += ch;

        while (next_item != EOF && !std::isdigit(next_item) &&
               CheckSymbol(static_cast<char>(next_item))) {
            char ch;
            in_->get(ch);
            buffer += ch;
            next_item = in_->peek();
        }

        token_ = SymbolToken{buffer};
    }

    // Получить текущий токен.
    Token GetToken() {
        return token_;
    }

private:
    std::istream* in_;
    Token token_;
    bool possible_unary_sign_ = true;
    bool is_end_ = false;
};
