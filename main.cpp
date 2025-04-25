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
#include <sstream>
#include <fstream>

using namespace std;

class Token
{
public:
    int tag;
    string lexeme;
    Token(int tag, string lexeme) : tag(tag), lexeme(lexeme) {}
    virtual ~Token() {}
    virtual string toString() const
    {
        switch (tag)
        {
        case LPAREN:
            return "LPAREN: " + lexeme;
        case RPAREN:
            return "RPAREN: " + lexeme;
        case LBRACE:
            return "LBRACE: " + lexeme;
        case RBRACE:
            return "RBRACE: " + lexeme;
        case COMMA:
            return "COMMA: " + lexeme;
        case SEMICOLON:
            return "SEMICOLON: " + lexeme;
        case EQUALS:
            return "EQUALS: " + lexeme;
        default:
            return "TOKEN: " + lexeme;
        }
    }
};

class Word : public Token
{
public:
    Word(int tag, const string lexeme) : Token(tag, lexeme) {}
    string toString() const override
    {
        switch (tag)
        {
        case DEF:
            return "DEF: " + lexeme;
        case INT:
            return "INT: " + lexeme;
        case IF:
            return "IF: " + lexeme;
        case ELSE:
            return "ELSE: " + lexeme;
        case PRINT:
            return "PRINT: " + lexeme;
        case RETURN:
            return "RETURN: " + lexeme;
        case ID:
            return "ID: " + lexeme;
        default:
            return "WORD: " + lexeme;
        }
    }
};

class Num : public Token
{
public:
    int value;
    Num(int value) : Token(NUM, to_string(value)), value(value) {}
    string toString() const override
    {
        return "NUM: " + lexeme;
    }
};

class Relop : public Token
{
public:
    int relop;
    Relop(int tag, string lexeme, int type) : Token(tag, lexeme), relop(type) {}
    string toString() const override
    {
        return "RELOP: " + lexeme;
    }
};

class Arithop : public Token
{
public:
    int arithop;
    Arithop(int tag, string lexeme, int type) : Token(tag, lexeme), arithop(type) {}
    string toString() const override
    {
        return "ARITHOP: " + lexeme;
    }
};

class Unknown : public Token
{
public:
    int line, column;
    Unknown(int tag, const string lexeme, int line, int column) : Token(tag, lexeme), line(line), column(column) {}
    string toString() const override
    {
        return "UNKNOWN: " + lexeme + " at line " + to_string(line) + ", column " + to_string(column);
    }
};

class Lexer
{
public:
    Lexer(istream &ssin) : ss(ssin), nlin(0), ncol(0)
    {
        // Reservar palavras-chave
        reserve(new Word(IF, "if"));
        reserve(new Word(ELSE, "else"));
        reserve(new Word(DEF, "def"));
        reserve(new Word(PRINT, "print"));
        reserve(new Word(RETURN, "return"));
        reserve(new Word(INT, "int"));
        reserve(new Relop(RELOP, "<=", LE));
        reserve(new Relop(RELOP, ">=", GE));
        reserve(new Relop(RELOP, "==", EQ));
        reserve(new Relop(RELOP, "!=", NE));
        reserve(new Relop(RELOP, "<", LT));
        reserve(new Relop(RELOP, ">", GT));
        reserve(new Relop(RELOP, "=", EQUALS));
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

    Token *scan()
    {
        if (p == EOF)
            return nullptr;

        // Ignorar espaços e linhas em branco
        while (isspace(p))
        {
            ss.get();
            if (p == '\n')
            {
                nlin++;
                ncol = 0;
            }
            else
                ncol++;
            p = ss.peek();
        }

        if (p == EOF)
            return nullptr;

        string lexeme;
        string best_lexeme;
        Tag best_tag;
        int best_size = 0;
        streampos start = ss.tellg();

        // Consumir caractere por caractere
        while (true)
        {
            lexeme += p;
            ss.get();
            ncol++;
            p = ss.peek();

            // Tenta todos automatas no lexema atual
            for (const auto &automataMap : automatas)
            {
                if (run_automata(lexeme, automataMap.second))
                {
                    // Se o automato reconheceu o lexema, verifica se é o melhor
                    // Se o lexema é maior ou se é reservado e o melhor não, troca
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

            // Se EOF ou espaço, ou não existe possibilidade de continuar crescendo o lexema, parar
            if (p == EOF || isspace(p))
                break;
        }

        // Se encontrou um token válido
        if (!best_lexeme.empty())
        {
            // Voltar o que foi lido a mais
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

        // Se não reconheceu nada
        if (!lexeme.empty())
        {
            return new Unknown(UNK, lexeme, nlin, ncol);
        }

        return nullptr;
    }

private:
    int nlin, ncol;
    istream &ss;
    unordered_map<string, Token *> words;
    char p; // caracter atual (peek)

    void reserve(Token *w) { words[w->lexeme] = w; }

    Token *create_token(Tag tag, const string &lexeme)
    {
        switch (tag)
        {
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
        case EQUALS:
            return new Token(tag, lexeme);
        case DEF:
        case IF:
        case ELSE:
        case PRINT:
        case RETURN:
        case INT:
            return new Word(tag, lexeme);
        default:
            return new Word(UNK, lexeme);
        }
    }
};

int analise_automatas(stringstream &ss)
{
    Lexer lexer(ss);

    Token *tok;
    while ((tok = lexer.scan()) != nullptr)
    {
        cout << tok->toString() << "\n";
        delete tok;
    }

    return 0;
}

stringstream testString()
{
    string codigo = "def main() { int defa = 5; if (a < 10) { print(a + 1); } else { print(0); } return 0; & }";
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

int main(int argc, char *argv[])
{
    stringstream ss;

    if (argc < 2)
    {
        cout << "Nenhum arquivo informado, usando código de teste padrão.\n";
        ss = testString();
    }
    else
    {
        ss = readFile(argv[1]);
    }

    analise_automatas(ss);
    return 0;
}
