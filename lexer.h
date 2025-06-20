#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

// Base Token class
class Token {
public:
    Tag tag;
    string lexeme;
    Token(Tag tag, const string& lexeme);
    virtual ~Token();
    virtual string toString() const;
};

// Word token class
class Word : public Token {
public:
    Word(Tag tag, const string& lexeme);
    string toString() const override;
};

// Numeric token class
class Num : public Token {
public:
    int value;
    Num(int value);
    string toString() const override;
};

// Relational operator class
class Relop : public Token {
public:
    Tag relop;
    Relop(Tag tag, const string& lexeme, Tag type);
    string toString() const override;
};

// Arithmetic operator class
class Arithop : public Token {
public:
    Tag arithop;
    Arithop(Tag tag, const string& lexeme, Tag type);
    string toString() const override;
};

// Unknown token class
class Unknown : public Token {
public:
    int line, column;
    Unknown(Tag tag, const string& lexeme, int line, int column);
    string toString() const override;
};

// Lexer class definition
class Lexer {
public:
    Lexer(istream &ssin);
    Token *scan();

private:
    int nlin, ncol;
    istream &ss;
    unordered_map<string, Token*> words;
    char p;

    void reserve(Token *w);
    Token* create_token(Tag tag, const string& lexeme);
};

// External helper functions
vector<Token*> analise_automatas(stringstream &ss);
stringstream testString();
stringstream readFile(const string &caminho);

#endif // LEXER_H
