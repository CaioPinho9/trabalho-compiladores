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
 * Este arquivo implementa a função run_automata, que simula um autômato de transição
 *
 * Data: Abril de 2025
 */

#include "automata.h"

bool run_automata(const string &input, const Automata &automata)
{
    int estado = 0;

    for (char simbolo : input)
    {
        auto it = automata.input_symbol_index.find(simbolo);
        if (it == automata.input_symbol_index.end())
        {
            return false;
        }
        int idx = it->second;
        estado = automata.transition_table[estado][idx];
        if (estado == -1)
        {
            return false;
        }
    }

    return automata.final_states.count(estado) > 0;
}

struct TestAutomatas
{
    string input;
    Tag automata_tag;
    bool expected; // true = deveria aceitar, false = deveria rejeitar
};

int testAutomata()
{
    // Lista de testes (input, qual automato, resultado esperado)
    vector<TestAutomatas> tests = {
        // ID
        {"abc123", ID, true},
        {"123abc", ID, false},

        // NUM
        {"123456", NUM, true},
        {"12a34", NUM, false},

        // Palavras-chave
        {"def", DEF, true},
        {"deff", DEF, false},

        {"int", INT, true},
        {"innt", INT, false},

        {"if", IF, true},
        {"iff", IF, false},

        {"else", ELSE, true},
        {"els", ELSE, false},

        {"print", PRINT, true},
        {"prinnt", PRINT, false},

        {"return", RETURN, true},
        {"reeturn", RETURN, false},

        // Operadores simples
        {"+", PLUS, true},
        {"++", PLUS, false},

        {"-", MINUS, true},
        {"--", MINUS, false},

        {"*", TIMES, true},
        {"**", TIMES, false},

        {"/", DIVIDE, true},
        {"//", DIVIDE, false},

        {"=", ASSIGN, true},
        {"==", ASSIGN, false},

        {"(", LPAREN, true},
        {"((", LPAREN, false},

        {")", RPAREN, true},
        {"))", RPAREN, false},

        {"{", LBRACE, true},
        {"{{", LBRACE, false},

        {"}", RBRACE, true},
        {"}}", RBRACE, false},

        {",", COMMA, true},
        {",,", COMMA, false},

        {";", SEMICOLON, true},
        {";;", SEMICOLON, false},

        // Operadores compostos
        {"<=", GE, true},
        {"<", GE, false},

        {">=", LE, true},
        {">", LE, false},

        {"!=", NE, true},
        {"!", NE, false},

        {"==", EQ, true},
        {"=", EQ, false},

        {"<", LT, true},
        {"<<", LT, false},

        {">", GT, true},
        {">>", GT, false},
    };

    // Rodar os testes
    int ok = 0, fail = 0;
    for (auto &test : tests)
    {
        bool encontrado = false;

        auto it = automatas.find(test.automata_tag);

        if (it == automatas.end())
        {
            cout << "Automato \"" << test.automata_tag << "\" não encontrado!\n";
            continue;
        }

        bool result = run_automata(test.input, it->second);

        if (result == test.expected)
        {
            cout << "[OK] Entrada \"" << test.input << "\" no automato \"" << test.automata_tag << "\"\n";
            ok++;
        }
        else
        {
            cout << "[FAIL] Entrada \"" << test.input << "\" no automato \"" << test.automata_tag
                 << "\" (esperado " << (test.expected ? "ACEITAR" : "REJEITAR") << ")\n";
            fail++;
        }
    }

    cout << "\nResumo: " << ok << " OK, " << fail << " FAIL\n";

    return 0;
}

// int main()
// {
//     testAutomata();
//     return 0;
// }