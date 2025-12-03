#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"./TokenType.cpp"

class Scanner{
    std::string source;
    std::vector<Token> tokens;
    int start=0;
    int current=0;
    int line=1;
    std::map<std::string, TokenType> keywords {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}
    }; 

    
    
    Scanner(std::string src):source(src){};

    bool isAtEnd(){
        return current>=source.length();
    }

    char advance(){
        current++;
        return source.at(current-1);
    }

    void addToken(TokenType type){
        tokens.emplace_back(type,NULL);
    }

    void addToken(TokenType type,Literal lit){
        std::string text=source.substr(start,current);
        tokens.emplace_back(Token(type,text,lit,line));
    }

    bool peek(){
        if(isAtEnd())return '\0';
        return source.at(current);
    }

    bool peekNext(){
        if(current+1>=source.length()) return '\0';
        return source.at(current+1);
    }

    bool match(char expected){
        if(isAtEnd()) return false;
        if(source.at(current)!=expected)return false;

        current++;
        return true;
    }

    void string(){
        while(peek()!='"' && !isAtEnd()){
            if(peek() =='\n')line++;
            advance();
        }

        if(isAtEnd()){
            Lox::error(line,"Unterminated string.");
            return;
        }

        advance();

        std::string value=source.substr(start+1,current-1);
        addToken(STRING,value);
    }

    bool isDigit(char c){
        if(!isAtEnd())return false;
        return c>='0' && c<='9';
    }

    bool isAlpha(char c){
        return (c>='A'&&c<='Z')||(c>='a'&&c<='z');
    }

    bool isAlphaNum(char c){
        return c=='_'||isDigit(c)||isAlpha(c);
    }

    void identifier(){
        while(isAlphaNum(peek()))advance();
        std::string text=source.substr(start,current);
        TokenType type=keywords[text];
        if(type==NULL)type=IDENTIFIER;
        addToken(type);
    }

    void number(){
        while(isDigit(peek()))advance();

        if(peek()=='.' && isDigit(peekNext())){
            advance();
            while(isDigit(peek()))advance;
        }

        addToken(NUMBER,std::stod(source.substr(start,current)));
    }

    void scanToken(){
        char c=advance();
        switch(c){
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;
            case '!':
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                addToken(match('=') ? GREATER_EQUAL : GREATER);
                break;
            case '/':
                if(match('/')){
                    while(peek()!='\n'&& !isAtEnd())advance;
                }else {
                    addToken(SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            case '"':string();break;

            default:
                if(isDigit(c)){
                    number();
                }else if(isAlpha(c)){
                    identifier();
                }else{
                    Lox::error(line, "Unexpected character.");
                }
                break;
        }
    }

    std::vector<Token> scanTokens(){
        while(!isAtEnd()){
            start=current;
            scanToken();
        }

        tokens.emplace_back(Token(TokenType::eof,"",std::monostate{},line));
        return tokens;
    }

};