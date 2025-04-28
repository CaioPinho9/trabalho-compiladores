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
 * Este arquivo define os autômatos de transição utilizados no analisador léxico.
 *
 * Data: Abril de 2025
 */

// automata.h
#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

struct Automata
{
    vector<vector<int>> transition_table;
    set<int> final_states;
    unordered_map<char, int> input_symbol_index;

    Automata(int num_estados, int num_simbolos) : transition_table(num_estados, vector<int>(num_simbolos, -1)) {}
};

enum Tag
{
    RELOP,
    ARITHOP,
    IF,
    ELSE,
    DEF,
    PRINT,
    RETURN,
    INT,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMICOLON,
    LE,
    GE,
    NE,
    EQ,
    LT,
    GT,
    ASSIGN,
    ID,
    NUM,
    UNK
};
enum TokenType
{
    KEYWORD,
    SYMBOL
};

// Mapeamento de TAGs para nomes
// Utilizado para imprimir os tokens
const unordered_map<Tag, string> TagNames = {
    {RELOP, "RELOP"},
    {ARITHOP, "ARITHOP"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {DEF, "DEF"},
    {PRINT, "PRINT"},
    {RETURN, "RETURN"},
    {INT, "INT"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {TIMES, "TIMES"},
    {DIVIDE, "DIVIDE"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {LBRACE, "LBRACE"},
    {RBRACE, "RBRACE"},
    {COMMA, "COMMA"},
    {SEMICOLON, "SEMICOLON"},
    {LE, "LE"},
    {GE, "GE"},
    {NE, "NE"},
    {EQ, "EQ"},
    {LT, "LT"},
    {GT, "GT"},
    {ASSIGN, "ASSIGN"},
    {ID, "ID"},
    {NUM, "NUM"},
    {UNK, "UNK"}
};

// ==========================
// Definição dos autômatos
// ==========================

// Preferi seguir pela lógica de utilizar menos memória criando tabelas menores, do que criar matrizes com todo o alfabeto, mesmo que seja menos performático. Porque também achei mais legível. Mas entendo que uma matriz sem precisar de indice para cada símbolo seria mais rápido.

// def
const Automata automata_def = []
{
    Automata a(4, 3);
    a.input_symbol_index = {{'d', 0}, {'e', 1}, {'f', 2}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.transition_table[2][2] = 3;
    a.final_states = {3};
    return a;
}();

// int
const Automata automata_int = []
{
    Automata a(4, 3);
    a.input_symbol_index = {{'i', 0}, {'n', 1}, {'t', 2}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.transition_table[2][2] = 3;
    a.final_states = {3};
    return a;
}();

// if
const Automata automata_if = []
{
    Automata a(3, 2);
    a.input_symbol_index = {{'i', 0}, {'f', 1}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.final_states = {2};
    return a;
}();

// else
const Automata automata_else = []
{
    Automata a(5, 3);
    a.input_symbol_index = {{'e', 0}, {'l', 1}, {'s', 2}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.transition_table[2][2] = 3;
    a.transition_table[3][0] = 4;
    a.final_states = {4};
    return a;
}();

// print
const Automata automata_print = []
{
    Automata a(6, 5);
    a.input_symbol_index = {{'p', 0}, {'r', 1}, {'i', 2}, {'n', 3}, {'t', 4}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.transition_table[2][2] = 3;
    a.transition_table[3][3] = 4;
    a.transition_table[4][4] = 5;
    a.final_states = {5};
    return a;
}();

// return
const Automata automata_return = []
{
    Automata a(7, 5);
    a.input_symbol_index = {{'r', 0}, {'e', 1}, {'t', 2}, {'u', 3}, {'n', 4}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.transition_table[2][2] = 3;
    a.transition_table[3][3] = 4;
    a.transition_table[4][0] = 5;
    a.transition_table[5][4] = 6;
    a.final_states = {6};
    return a;
}();

// id
const Automata automata_id = []
{
    Automata a(2, 62); // 26 letras minúsculas + 26 maiúsculas + 10 dígitos
    int idx = 0;

    for (char c = 'a'; c <= 'z'; ++c)
    {
        a.input_symbol_index[c] = idx;
        a.transition_table[0][idx] = 1;
        a.transition_table[1][idx++] = 1;
    }
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        a.input_symbol_index[c] = idx;
        a.transition_table[0][idx] = 1;
        a.transition_table[1][idx++] = 1;
    }
    for (char c = '0'; c <= '9'; ++c)
    {
        a.input_symbol_index[c] = idx;
        a.transition_table[1][idx++] = 1;
    }

    a.final_states = {1};
    return a;
}();

// num
const Automata automata_num = []
{
    Automata a(2, 10); // dígitos '0'..'9'
    int idx = 0;
    for (char c = '0'; c <= '9'; ++c)
    {
        a.input_symbol_index[c] = idx++;
    }

    for (auto &p : a.input_symbol_index)
    {
        a.transition_table[0][p.second] = 1;
        a.transition_table[1][p.second] = 1;
    }

    a.final_states = {1};
    return a;
}();

// plus
const Automata automata_plus = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'+', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// minus
const Automata automata_minus = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'-', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// times
const Automata automata_times = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'*', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// divide
const Automata automata_divide = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'/', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// assign
const Automata automata_assign = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'=', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// less
const Automata automata_less = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'<', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// greater
const Automata automata_greater = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'>', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// less_equal
const Automata automata_less_equal = []
{
    Automata a(3, 2);
    a.input_symbol_index = {{'<', 0}, {'=', 1}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.final_states = {2};
    return a;
}();

// greater_equal
const Automata automata_greater_equal = []
{
    Automata a(3, 2);
    a.input_symbol_index = {{'>', 0}, {'=', 1}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.final_states = {2};
    return a;
}();

// different
const Automata automata_different = []
{
    Automata a(3, 2);
    a.input_symbol_index = {{'!', 0}, {'=', 1}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][1] = 2;
    a.final_states = {2};
    return a;
}();

// equal_equal
const Automata automata_equal_equal = []
{
    Automata a(3, 1);
    a.input_symbol_index = {{'=', 0}};
    a.transition_table[0][0] = 1;
    a.transition_table[1][0] = 2;
    a.final_states = {2};
    return a;
}();

// lparen
const Automata automata_lparen = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'(', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// rparen
const Automata automata_rparen = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{')', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// lbrace
const Automata automata_lbrace = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'{', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// rbrace
const Automata automata_rbrace = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{'}', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// comma
const Automata automata_comma = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{',', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// semicolon
const Automata automata_semicolon = []
{
    Automata a(2, 1);
    a.input_symbol_index = {{';', 0}};
    a.transition_table[0][0] = 1;
    a.final_states = {1};
    return a;
}();

// ==========================
// Vetor de mapeamento do automato com a TAG
// ==========================

const unordered_map<Tag, Automata> automatas = {
    {DEF, automata_def},
    {INT, automata_int},
    {IF, automata_if},
    {ELSE, automata_else},
    {PRINT, automata_print},
    {RETURN, automata_return},
    {ID, automata_id},
    {NUM, automata_num},
    {PLUS, automata_plus},
    {MINUS, automata_minus},
    {TIMES, automata_times},
    {DIVIDE, automata_divide},
    {ASSIGN, automata_assign},
    {LT, automata_less},
    {GT, automata_greater},
    {LE, automata_less_equal},
    {GE, automata_greater_equal},
    {NE, automata_different},
    {EQ, automata_equal_equal},
    {LPAREN, automata_lparen},
    {RPAREN, automata_rparen},
    {LBRACE, automata_lbrace},
    {RBRACE, automata_rbrace},
    {COMMA, automata_comma},
    {SEMICOLON, automata_semicolon}};

bool run_automata(const string &input, const Automata &automata);

#endif // AUTOMATA_H
