/*
 * Trabalho de Compiladores - Analisador Léxico
 * Parte A: Lexer baseado em Autômatos de Transição
 *
 * Autor: Caio Broering Pinho
 * Sistema: GNU/Linux
 * Linguagem: C++17
 * Compilador: g++ versão 13.3.0
 *
 * Descrição:
 * Este arquivo faz parte da implementação de um analisador léxico que reconhece
 * identificadores, constantes numéricas inteiras e operadores relacionais,
 * utilizando diagramas de transição (autômatos).
 *
 * Data: Abril de 2025
 */

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

// Base Token class
class Token
{
public:
    Tag tag;
    string lexeme;
    Token(Tag tag, const string &lexeme);
    virtual ~Token();
    virtual string toString() const;
};

// Word token class
class Word : public Token
{
public:
    Word(Tag tag, const string &lexeme);
    string toString() const override;
};

// Numeric token class
class Num : public Token
{
public:
    int value;
    Num(int value);
    string toString() const override;
};

// Relational operator class
class Relop : public Token
{
public:
    Tag relop;
    Relop(Tag tag, const string &lexeme, Tag type);
};

// Arithmetic operator class
class Arithop : public Token
{
public:
    Tag arithop;
    Arithop(Tag tag, const string &lexeme, Tag type);
};

// Unknown token class
class Unknown : public Token
{
public:
    int line, column;
    Unknown(Tag tag, const string &lexeme, int line, int column);
    string toString() const override;
};

// Lexer class definition
class Lexer
{
public:
    Lexer(istream &ssin);
    Token *scan();

private:
    int nlin, ncol;
    istream &ss;
    unordered_map<string, Token *> words;
    char p;

    void reserve(Token *w);
    Token *create_token(Tag tag, const string &lexeme);
};

// External helper functions
vector<Token *> analise_automatas(stringstream &ss);
stringstream testString();
stringstream readFile(const string &caminho);

#endif // LEXER_H
