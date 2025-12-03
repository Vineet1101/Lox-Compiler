#include<iostream>
#include<variant>
#include<vector>
#include<map>
#include "./Lox.cpp"

using Literal=std::variant<std::monostate,double,std::string,bool>;

enum TokenType{
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    
    // Literals.
    IDENTIFIER, STRING, NUMBER,
    
    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    
    eof
};

class Token{
    TokenType type;
    std::string lexeme;
    Literal literal;
    int line;

public:

    Token(TokenType _type, std::string _lexeme, Literal _literal, int _line):type(_type),lexeme(std::move(_lexeme)),line(_line),literal(std::move(_literal)){}
    
    struct LiteralToString {
        std::string operator()(std::monostate) const { return "null"; }
        std::string operator()(double d) const { return std::to_string(d); }
        std::string operator()(const std::string& s) const { return s; }
        std::string operator()(bool b) const { return b ? "true" : "false"; }
    };

    static std::string tokenTypeToString(TokenType type) {
        switch (type) {
            // Single-character tokens
            case LEFT_PAREN:      return "LEFT_PAREN";
            case RIGHT_PAREN:     return "RIGHT_PAREN";
            case LEFT_BRACE:      return "LEFT_BRACE";
            case RIGHT_BRACE:     return "RIGHT_BRACE";
            case COMMA:           return "COMMA";
            case DOT:             return "DOT";
            case MINUS:           return "MINUS";
            case PLUS:            return "PLUS";
            case SEMICOLON:       return "SEMICOLON";
            case SLASH:           return "SLASH";
            case STAR:            return "STAR";

            // One or two character tokens
            case BANG:            return "BANG";
            case BANG_EQUAL:      return "BANG_EQUAL";
            case EQUAL:           return "EQUAL";
            case EQUAL_EQUAL:     return "EQUAL_EQUAL";
            case GREATER:         return "GREATER";
            case GREATER_EQUAL:   return "GREATER_EQUAL";
            case LESS:            return "LESS";
            case LESS_EQUAL:      return "LESS_EQUAL";

            // Literals
            case IDENTIFIER:      return "IDENTIFIER";
            case STRING:          return "STRING";
            case NUMBER:          return "NUMBER";

            // Keywords
            case AND:             return "AND";
            case CLASS:           return "CLASS";
            case ELSE:            return "ELSE";
            case FALSE:           return "FALSE";
            case FUN:             return "FUN";
            case FOR:             return "FOR";
            case IF:              return "IF";
            case NIL:             return "NIL";
            case OR:              return "OR";
            case PRINT:           return "PRINT";
            case RETURN:          return "RETURN";
            case SUPER:           return "SUPER";
            case THIS:            return "THIS";
            case TRUE:            return "TRUE";
            case VAR:             return "VAR";
            case WHILE:           return "WHILE";

            // End-of-file
            case eof:             return "EOF";
        }

        return "UNKNOWN_TOKEN";
    }


    std::string toString() const{
        return tokenTypeToString(type)+" "+lexeme+ " "+std::visit(LiteralToString{},literal);
    }

};
