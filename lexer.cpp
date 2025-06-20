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

#include "automata.h"
#include "lexer.h"
#include <sstream>
#include <fstream>

using namespace std;

Token::Token(Tag tag, const string &lexeme) : tag(tag), lexeme(lexeme) {}
Token::~Token() {}

string Token::toString() const
{
    switch (tag)
    {
    case LPAREN:
        return "LPAREN";
    case RPAREN:
        return "RPAREN";
    case LBRACE:
        return "LBRACE";
    case RBRACE:
        return "RBRACE";
    case COMMA:
        return "COMMA";
    case SEMICOLON:
        return "SEMICOLON";
    case ASSIGN:
        return "ASSIGN";
    default:
        return "TOKEN";
    }
}

// Continue similarly for all derived classes...
Word::Word(Tag tag, const string &lexeme) : Token(tag, lexeme) {}
string Word::toString() const
{
    switch (tag)
    {
    case DEF:
        return "DEF";
    case INT:
        return "INT";
    case IF:
        return "IF";
    case ELSE:
        return "ELSE";
    case PRINT:
        return "PRINT";
    case RETURN:
        return "RETURN";
    case IDFUN:
        return "IDFUN(" + lexeme + ")";
    case ID:
        return "ID(" + lexeme + ")";
    case EOF_TOKEN:
        return "EOF(" + lexeme + ") (NAO PERMITIDO NA LINGUAGEM)";
    default:
        return "WORD";
    }
}

Num::Num(int value) : Token(NUM, to_string(value)), value(value) {}
string Num::toString() const { return "NUM(" + lexeme + ")"; }

Relop::Relop(Tag tag, const string &lexeme, Tag type)
    : Token(tag, lexeme), relop(type) {}
string Relop::toString() const
{
    switch (relop)
    {
    case LE:
        return "LE";
    case GE:
        return "GE";
    case EQ:
        return "EQ";
    case NE:
        return "NE";
    case LT:
        return "LT";
    case GT:
        return "GT";
    default:
        return "RELOP";
    }
}

Arithop::Arithop(Tag tag, const string &lexeme, Tag type)
    : Token(tag, lexeme), arithop(type) {}
string Arithop::toString() const
{
    switch (arithop)
    {
    case PLUS:
        return "PLUS";
    case MINUS:
        return "MINUS";
    case TIMES:
        return "TIMES";
    case DIVIDE:
        return "DIVIDE";
    case ASSIGN:
        return "ASSIGN";
    default:
        return "ARITHOP";
    }
}

Unknown::Unknown(Tag tag, const string &lexeme, int line, int column)
    : Token(tag, lexeme), line(line), column(column) {}
string Unknown::toString() const
{
    return "UNKNOWN(" + lexeme + ") at line " + to_string(line) + ", column " + to_string(column);
}

Lexer::Lexer(istream &ssin) : ss(ssin), nlin(0), ncol(0)
{
    reserve(new Word(IF, "if"));
    reserve(new Word(ELSE, "else"));
    reserve(new Word(DEF, "def"));
    reserve(new Word(PRINT, "print"));
    reserve(new Word(RETURN, "return"));
    reserve(new Word(INT, "int"));
    reserve(new Word(EOF_TOKEN, "$"));
    reserve(new Relop(RELOP, "<=", LE));
    reserve(new Relop(RELOP, ">=", GE));
    reserve(new Relop(RELOP, "==", EQ));
    reserve(new Relop(RELOP, "!=", NE));
    reserve(new Relop(RELOP, "<", LT));
    reserve(new Relop(RELOP, ">", GT));
    reserve(new Arithop(ARITHOP, "=", ASSIGN));
    reserve(new Arithop(ARITHOP, "+", PLUS));
    reserve(new Arithop(ARITHOP, "-", MINUS));
    reserve(new Arithop(ARITHOP, "*", TIMES));
    reserve(new Arithop(ARITHOP, "/", DIVIDE));
    reserve(new Token(LPAREN, "("));
    reserve(new Token(RPAREN, ")"));
    reserve(new Token(LBRACE, "{"));
    reserve(new Token(RBRACE, "}"));
    reserve(new Token(COMMA, ","));
    reserve(new Token(SEMICOLON, ";"));

    p = ss.peek();
}

Token *Lexer::scan()
{
    if (p == EOF)
        return nullptr;

    while (isspace(p))
    {
        ss.get();
        if (p == '\n')
        {
            nlin++;
            ncol = 0;
        }
        else
        {
            ncol++;
        }
        p = ss.peek();
    }

    if (p == EOF)
        return nullptr;

    string lexeme, best_lexeme;
    Tag best_tag;
    int best_size = 0;
    streampos start = ss.tellg();

    while (true)
    {
        lexeme += p;
        ss.get();
        ncol++;
        p = ss.peek();

        for (const auto &automataMap : automatas)
        {
            if (run_automata(lexeme, automataMap.second))
            {
                bool is_reserved = words.find(lexeme) != words.end();
                bool best_is_reserved = words.find(best_lexeme) != words.end();
                bool best = lexeme.size() > best_size || (lexeme.size() == best_size && is_reserved && !best_is_reserved);
                if (best)
                {
                    best_lexeme = lexeme;
                    best_tag = automataMap.first;
                    best_size = lexeme.size();
                }
            }
        }

        if (p == EOF || isspace(p))
            break;
    }

    if (!best_lexeme.empty())
    {
        int to_return = lexeme.size() - best_size;
        if (to_return > 0)
        {
            ss.clear();
            ss.seekg(start + streamoff(best_size));
            p = ss.peek();
            ncol -= to_return;
        }
        return create_token(best_tag, best_lexeme);
    }

    if (!lexeme.empty())
    {
        return new Unknown(UNK, lexeme, nlin, ncol);
    }

    return nullptr;
}

void Lexer::reserve(Token *w)
{
    words[w->lexeme] = w;
}

Token *Lexer::create_token(Tag tag, const string &lexeme)
{
    switch (tag)
    {
    case IDFUN:
        return new Word(IDFUN, lexeme);
    case ID:
        return new Word(ID, lexeme);
    case NUM:
        return new Num(stoi(lexeme));
    case LE:
    case GE:
    case EQ:
    case NE:
    case LT:
    case GT:
        return new Relop(RELOP, lexeme, tag);
    case PLUS:
    case MINUS:
    case TIMES:
    case DIVIDE:
        return new Arithop(ARITHOP, lexeme, tag);
    case LPAREN:
    case RPAREN:
    case LBRACE:
    case RBRACE:
    case COMMA:
    case SEMICOLON:
    case ASSIGN:
        return new Token(tag, lexeme);
    case DEF:
    case IF:
    case ELSE:
    case PRINT:
    case RETURN:
    case INT:
        return new Word(tag, lexeme);
    case EOF_TOKEN:
        return new Word(EOF_TOKEN, lexeme);
    default:
        return new Word(UNK, lexeme);
    }
}

vector<Token *> analise_automatas(stringstream &ss)
{
    Lexer lexer(ss);

    vector<Token *> tokens;
    Token *tok;
    while ((tok = lexer.scan()) != nullptr)
    {
        tokens.push_back(tok);
    }
    return tokens;
}

stringstream testString()
{
    string codigo = "def Main() { int x, y; x = 10; y = 20; if (x < y) { print(x + y); } else { print(y); } id = CallFunc(x); return; }";
    stringstream ss(codigo);
    return ss;
}

stringstream readFile(const string &caminho)
{
    ifstream arquivo(caminho);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir arquivo: " << caminho << endl;
        exit(1);
    }
    stringstream ss;
    ss << arquivo.rdbuf();
    return ss;
}

// int main(int argc, char *argv[])
// {
//     stringstream ss;

//     if (argc < 2)
//     {
//         cout << "Nenhum arquivo informado, usando código de teste padrão.\n";
//         ss = testString();
//     }
//     else
//     {
//         ss = readFile(argv[1]);
//     }

//     vector<Token *> tokens = analise_automatas(ss);
//     return 0;
// }
