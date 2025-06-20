#include <string>
#include "automata.h"
#include "lexer.h"
#include <variant>
#include <unordered_map>
#include <stack>
#include <algorithm>

const int NUM_NONTERMINALS = 30;
const int NUM_TERMINALS = 30;

enum NonTerminals
{
    NT_S = 0,
    NT_MAIN = 1,
    NT_FLIST = 2,
    NT_FLIST_ = 3,
    NT_FDEF = 4,
    NT_PARLIST = 5,
    NT_PARLIST_ = 6,
    NT_VARLIST = 7,
    NT_VARLIST_ = 8,
    NT_STMT = 9,
    NT_ATRIBST = 10,
    NT_ATRIBST_ = 11,
    NT_FCALL = 12,
    NT_PARLISTCALL = 13,
    NT_PARLISTCALL_ = 14,
    NT_PRINTST = 15,
    NT_RETURNST = 16,
    NT_RETURNST_ = 17,
    NT_IFSTMT = 18,
    NT_IFSTMT_ = 19,
    NT_STMTLIST = 20,
    NT_EXPR = 21,
    NT_EXPR_ = 22,
    NT_NUMEXPR = 23,
    NT_NUMEXPR_ = 24,
    NT_TERM = 25,
    NT_TERM_ = 26,
    NT_FACTOR = 27
};

std::unordered_map<NonTerminals, std::string> NON_TERMINAL_TO_STRING = {
    {NT_S, "S"},
    {NT_MAIN, "MAIN"},
    {NT_FLIST, "FLIST"},
    {NT_FLIST_, "FLIST_"},
    {NT_FDEF, "FDEF"},
    {NT_PARLIST, "PARLIST"},
    {NT_PARLIST_, "PARLIST_"},
    {NT_VARLIST, "VARLIST"},
    {NT_VARLIST_, "VARLIST_"},
    {NT_STMT, "STMT"},
    {NT_ATRIBST, "ATRIBST"},
    {NT_ATRIBST_, "ATRIBST_"},
    {NT_FCALL, "FCALL"},
    {NT_PARLISTCALL, "PARLISTCALL"},
    {NT_PARLISTCALL_, "PARLISTCALL_"},
    {NT_PRINTST, "PRINTST"},
    {NT_RETURNST, "RETURNST"},
    {NT_RETURNST_, "RETURNST_"},
    {NT_IFSTMT, "IFSTMT"},
    {NT_IFSTMT_, "IFSTMT_"},
    {NT_STMTLIST, "STMTLIST"},
    {NT_EXPR, "EXPR"},
    {NT_EXPR_, "EXPR_"},
    {NT_NUMEXPR, "NUMEXPR"},
    {NT_NUMEXPR_, "NUMEXPR_"},
    {NT_TERM, "TERM"},
    {NT_TERM_, "TERM_"},
    {NT_FACTOR, "FACTOR"}};

enum Productions
{
    EMPTY = 0,
    PROD_S_0, // S ::= MAIN $

    PROD_MAIN_EPSILON, // MAIN ::= ε
    PROD_MAIN_FLIST,   // MAIN ::= FLIST
    PROD_MAIN_STMT,    // MAIN ::= STMT

    PROD_FLIST_FDEF, // FLIST ::= FDEF FLIST_

    PROD_FLIST__EPSILON, // FLIST_ ::= ε
    PROD_FLIST__FDEF,    // FLIST_ ::= FDEF

    PROD_FDEF_DEF, // FDEF ::= def IDFUN lparen PARLIST rparen lbrace STMTLIST rbrace

    PROD_PARLIST_INT,     // PARLIST ::= int id PARLIST_
    PROD_PARLIST_EPSILON, // PARLIST ::= ε

    PROD_PARLIST__COMMA,   // PARLIST_ ::= comma int id PARLIST_
    PROD_PARLIST__EPSILON, // PARLIST_ ::= ε

    PROD_VARLIST_ID, // VARLIST ::= id VARLIST_

    PROD_VARLIST__COMMA,   // VARLIST_ ::= comma id VARLIST_
    PROD_VARLIST__EPSILON, // VARLIST_ ::= ε

    PROD_STMT_INT,       // STMT ::= int VARLIST semicolon
    PROD_STMT_ATRIBST,   // STMT ::= ATRIBST semicolon
    PROD_STMT_BLOCK,     // STMT ::= lbrace STMTLIST rbrace
    PROD_STMT_SEMICOLON, // STMT ::= semicolon
    PROD_STMT_PRINT,     // STMT ::= PRINTST semicolon
    PROD_STMT_RETURN,    // STMT ::= RETURNST semicolon
    PROD_STMT_IF,        // STMT ::= IFSTMT

    PROD_ATRIBST_ID, // ATRIBST ::= id assign ATRIBST_

    PROD_ATRIBST__FCALL, // ATRIBST_ ::= FCALL
    PROD_ATRIBST__EXPR,  // ATRIBST_ ::= EXPR

    PROD_FCALL_IDFUN, // FCALL ::= idfun lparen PARLISTCALL rparen

    PROD_PARLISTCALL_ID, // PARLISTCALL ::= id PARLISTCALL_

    PROD_PARLISTCALL__EPSILON, // PARLISTCALL_ ::= ε
    PROD_PARLISTCALL__COMMA,   // PARLISTCALL_ ::= comma id PARLISTCALL_

    PROD_PRINTST_PRINT, // PRINTST ::= print EXPR

    PROD_RETURNST_RETURN, // RETURNST ::= return RETURNST_

    PROD_RETURNST__ID,      // RETURNST_ ::= id
    PROD_RETURNST__EPSILON, // RETURNST_ ::= ε

    PROD_IFSTMT_IF, // IFSTMT ::= if lparen EXPR rparen lbrace STMT rbrace IFSTMT_

    PROD_IFSTMT__EPSILON, // IFSTMT_ ::= ε
    PROD_IFSTMT__ELSE,    // IFSTMT_ ::= else lbrace STMT rbrace

    PROD_STMTLIST_STMT,    // STMTLIST ::= STMT STMTLIST
    PROD_STMTLIST_EPSILON, // STMTLIST ::= ε

    PROD_EXPR_NUMEXPR, // EXPR ::= NUMEXPR EXPR_

    PROD_EXPR__EPSILON, // EXPR_ ::= ε
    PROD_EXPR__LT,      // EXPR_ ::= lt NUMEXPR
    PROD_EXPR__LE,      // EXPR_ ::= le NUMEXPR
    PROD_EXPR__GT,      // EXPR_ ::= gt NUMEXPR
    PROD_EXPR__GE,      // EXPR_ ::= ge NUMEXPR
    PROD_EXPR__EQ,      // EXPR_ ::= eq NUMEXPR
    PROD_EXPR__NE,      // EXPR_ ::= ne NUMEXPR

    PROD_NUMEXPR_TERM, // NUMEXPR ::= TERM NUMEXPR_

    PROD_NUMEXPR__EPSILON, // NUMEXPR_ ::= ε
    PROD_NUMEXPR__PLUS,    // NUMEXPR_ ::= plus TERM NUMEXPR_
    PROD_NUMEXPR__MINUS,   // NUMEXPR_ ::= minus TERM NUMEXPR_

    PROD_TERM_FACTOR, // TERM ::= FACTOR TERM_

    PROD_TERM__EPSILON, // TERM_ ::= ε
    PROD_TERM__TIMES,   // TERM_ ::= times FACTOR TERM_
    PROD_TERM__DIVIDE,  // TERM_ ::= divide FACTOR TERM_

    PROD_FACTOR_NUMEXPR, // FACTOR ::= lparen NUMEXPR rparen
    PROD_FACTOR_ID,      // FACTOR ::= id
    PROD_FACTOR_NUM,     // FACTOR ::= num
};

Productions ll1_table[NUM_NONTERMINALS][NUM_TERMINALS];
using Symbol = variant<Tag, NonTerminals>;

std::unordered_map<Productions, std::vector<Symbol>> productionsMap = {
    {PROD_S_0, {NonTerminals::NT_MAIN, Tag::EOF_TOKEN}}, // S ::= MAIN $

    {PROD_MAIN_EPSILON, {}},                     // MAIN ::= ε
    {PROD_MAIN_FLIST, {NonTerminals::NT_FLIST}}, // MAIN ::= FLIST
    {PROD_MAIN_STMT, {NonTerminals::NT_STMT}},   // MAIN ::= STMT

    {PROD_FLIST_FDEF, {NonTerminals::NT_FDEF, NonTerminals::NT_FLIST_}}, // FLIST ::= FDEF FLIST_

    {PROD_FLIST__EPSILON, {}},                   // FLIST_ ::= ε
    {PROD_FLIST__FDEF, {NonTerminals::NT_FDEF}}, // FLIST_ ::= FDEF

    {PROD_FDEF_DEF, {Tag::DEF, Tag::IDFUN, Tag::LPAREN, NonTerminals::NT_PARLIST, Tag::RPAREN, Tag::LBRACE, NonTerminals::NT_STMTLIST, Tag::RBRACE}}, // FDEF ::= def IDFUN lparen PARLIST rparen lbrace STMTLIST rbrace

    {PROD_PARLIST_INT, {Tag::INT, Tag::ID, NonTerminals::NT_PARLIST_}}, // PARLIST ::= int id PARLIST_
    {PROD_PARLIST_EPSILON, {}},                                         // PARLIST ::= ε

    {PROD_PARLIST__COMMA, {Tag::COMMA, Tag::INT, Tag::ID, NonTerminals::NT_PARLIST_}}, // PARLIST_ ::= comma int id PARLIST_
    {PROD_PARLIST__EPSILON, {}},                                                       // PARLIST_ ::= ε

    {PROD_VARLIST_ID, {Tag::ID, NonTerminals::NT_VARLIST_}}, // VARLIST ::= id VARLIST_

    {PROD_VARLIST__COMMA, {Tag::COMMA, Tag::ID, NonTerminals::NT_VARLIST_}}, // VARLIST_ ::= comma id VARLIST_
    {PROD_VARLIST__EPSILON, {}},                                             // VARLIST_ ::= ε

    {PROD_STMT_INT, {Tag::INT, NonTerminals::NT_VARLIST, Tag::SEMICOLON}},    // STMT ::= int VARLIST semicolon
    {PROD_STMT_ATRIBST, {NonTerminals::NT_ATRIBST, Tag::SEMICOLON}},          // STMT ::= ATRIBST semicolon
    {PROD_STMT_BLOCK, {Tag::LBRACE, NonTerminals::NT_STMTLIST, Tag::RBRACE}}, // STMT ::= lbrace STMTLIST rbrace
    {PROD_STMT_SEMICOLON, {Tag::SEMICOLON}},                                  // STMT ::= semicolon
    {PROD_STMT_PRINT, {NonTerminals::NT_PRINTST, Tag::SEMICOLON}},            // STMT ::= PRINTST semicolon
    {PROD_STMT_RETURN, {NonTerminals::NT_RETURNST, Tag::SEMICOLON}},          // STMT ::= RETURNST semicolon
    {PROD_STMT_IF, {NonTerminals::NT_IFSTMT}},                                // STMT ::= IFSTMT

    {PROD_ATRIBST_ID, {Tag::ID, Tag::ASSIGN, NonTerminals::NT_ATRIBST_}}, // ATRIBST ::= id assign ATRIBST_
    {PROD_ATRIBST__FCALL, {NonTerminals::NT_FCALL}},                      // ATRIBST_ ::= FCALL
    {PROD_ATRIBST__EXPR, {NonTerminals::NT_EXPR}},                        // ATRIBST_ ::= EXPR

    {PROD_FCALL_IDFUN, {Tag::IDFUN, Tag::LPAREN, NonTerminals::NT_PARLISTCALL, Tag::RPAREN}}, // FCALL ::= idfun lparen PARLISTCALL rparen

    {PROD_PARLISTCALL_ID, {Tag::ID, NonTerminals::NT_PARLISTCALL_}},                 // PARLISTCALL ::= id PARLISTCALL_
    {PROD_PARLISTCALL__EPSILON, {}},                                                 // PARLISTCALL_ ::= ε
    {PROD_PARLISTCALL__COMMA, {Tag::COMMA, Tag::ID, NonTerminals::NT_PARLISTCALL_}}, // PARLISTCALL_ ::= comma id PARLISTCALL_

    {PROD_PRINTST_PRINT, {Tag::PRINT, NonTerminals::NT_EXPR}}, // PRINTST ::= print EXPR

    {PROD_RETURNST_RETURN, {Tag::RETURN, NonTerminals::NT_RETURNST_}}, // RETURNST ::= return RETURNST_
    {PROD_RETURNST__ID, {Tag::ID}},                                    // RETURNST_ ::= id
    {PROD_RETURNST__EPSILON, {}},                                      // RETURNST_ ::= ε

    {PROD_IFSTMT_IF, {Tag::IF, Tag::LPAREN, NonTerminals::NT_EXPR, Tag::RPAREN, Tag::LBRACE, NonTerminals::NT_STMT, Tag::RBRACE, NonTerminals::NT_IFSTMT_}}, // IFSTMT ::= if lparen EXPR rparen lbrace STMT rbrace IFSTMT_

    {PROD_IFSTMT__EPSILON, {}},                                                        // IFSTMT_ ::= ε
    {PROD_IFSTMT__ELSE, {Tag::ELSE, Tag::LBRACE, NonTerminals::NT_STMT, Tag::RBRACE}}, // IFSTMT_ ::= else lbrace STMT rbrace

    {PROD_STMTLIST_STMT, {NonTerminals::NT_STMT, NonTerminals::NT_STMTLIST}}, // STMTLIST ::= STMT STMTLIST
    {PROD_STMTLIST_EPSILON, {}},                                              // STMTLIST ::= ε

    {PROD_EXPR_NUMEXPR, {NonTerminals::NT_NUMEXPR, NonTerminals::NT_EXPR_}}, // EXPR ::= NUMEXPR EXPR_
    {PROD_EXPR__EPSILON, {}},                                                // EXPR_ ::= ε
    {PROD_EXPR__LT, {Tag::LT, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= lt NUMEXPR
    {PROD_EXPR__LE, {Tag::LE, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= le NUMEXPR
    {PROD_EXPR__GT, {Tag::GT, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= gt NUMEXPR
    {PROD_EXPR__GE, {Tag::GE, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= ge NUMEXPR
    {PROD_EXPR__EQ, {Tag::EQ, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= eq NUMEXPR
    {PROD_EXPR__NE, {Tag::NE, NonTerminals::NT_NUMEXPR}},                    // EXPR_ ::= ne NUMEXPR

    {PROD_NUMEXPR_TERM, {NonTerminals::NT_TERM, NonTerminals::NT_NUMEXPR_}},               // NUMEXPR ::= TERM NUMEXPR_
    {PROD_NUMEXPR__EPSILON, {}},                                                           // NUMEXPR_ ::= ε
    {PROD_NUMEXPR__PLUS, {Tag::PLUS, NonTerminals::NT_TERM, NonTerminals::NT_NUMEXPR_}},   // NUMEXPR_ ::= plus TERM NUMEXPR_
    {PROD_NUMEXPR__MINUS, {Tag::MINUS, NonTerminals::NT_TERM, NonTerminals::NT_NUMEXPR_}}, // NUMEXPR_ ::= minus TERM NUMEXPR_

    {PROD_TERM_FACTOR, {NonTerminals::NT_FACTOR, NonTerminals::NT_TERM_}},               // TERM ::= FACTOR TERM_
    {PROD_TERM__EPSILON, {}},                                                            // TERM_ ::= ε
    {PROD_TERM__TIMES, {Tag::TIMES, NonTerminals::NT_FACTOR, NonTerminals::NT_TERM_}},   // TERM_ ::= times FACTOR TERM_
    {PROD_TERM__DIVIDE, {Tag::DIVIDE, NonTerminals::NT_FACTOR, NonTerminals::NT_TERM_}}, // TERM_ ::= divide FACTOR TERM_

    {PROD_FACTOR_NUMEXPR, {Tag::LPAREN, NonTerminals::NT_NUMEXPR, Tag::RPAREN}}, // FACTOR ::= lparen NUMEXPR rparen
    {PROD_FACTOR_ID, {Tag::ID}},                                                 // FACTOR ::= id
    {PROD_FACTOR_NUM, {Tag::NUM}},                                               // FACTOR ::= num
};

std::unordered_map<Productions, std::string> PRODUCTIONS_TO_STRING = {
    {PROD_S_0, "S ::= MAIN $"},
    {PROD_MAIN_EPSILON, "MAIN ::= ε"},
    {PROD_MAIN_FLIST, "MAIN ::= FLIST"},
    {PROD_MAIN_STMT, "MAIN ::= STMT"},
    {PROD_FLIST_FDEF, "FLIST ::= FDEF FLIST_"},
    {PROD_FLIST__EPSILON, "FLIST_ ::= ε"},
    {PROD_FLIST__FDEF, "FLIST_ ::= FDEF"},
    {PROD_FDEF_DEF, "FDEF ::= def IDFUN lparen PARLIST rparen lbrace STMTLIST rbrace"},
    {PROD_PARLIST_INT, "PARLIST ::= int id comma PARLIST_"},
    {PROD_PARLIST_EPSILON, "PARLIST ::= ε"},
    {PROD_PARLIST__COMMA, "PARLIST_ ::= comma int id PARLIST_"},
    {PROD_PARLIST__EPSILON, "PARLIST_ ::= ε"},
    {PROD_VARLIST_ID, "VARLIST ::= id VARLIST_"},
    {PROD_VARLIST__COMMA, "VARLIST_ ::= comma id VARLIST_"},
    {PROD_VARLIST__EPSILON, "VARLIST_ ::= ε"},
    {PROD_STMT_INT, "STMT ::= int VARLIST semicolon"},
    {PROD_STMT_ATRIBST, "STMT ::= ATRIBST semicolon"},
    {PROD_STMT_BLOCK, "STMT ::= lbrace STMTLIST rbrace"},
    {PROD_STMT_SEMICOLON, "STMT ::= semicolon"},
    {PROD_STMT_PRINT, "STMT ::= PRINTST semicolon"},
    {PROD_STMT_RETURN, "STMT ::= RETURNST semicolon"},
    {PROD_STMT_IF, "STMT ::= IFSTMT"},
    {PROD_ATRIBST_ID, "ATRIBST ::= id assign ATRIBST_"},
    {PROD_ATRIBST__FCALL, "ATRIBST_ ::= FCALL"},
    {PROD_ATRIBST__EXPR, "ATRIBST_ ::= EXPR"},
    {PROD_FCALL_IDFUN, "FCALL ::= idfun lparen PARLISTCALL rparen"},
    {PROD_PARLISTCALL_ID, "PARLISTCALL ::= id PARLISTCALL_"},
    {PROD_PARLISTCALL__EPSILON, "PARLISTCALL_ ::= ε"},
    {PROD_PARLISTCALL__COMMA, "PARLISTCALL_ ::= comma id PARLISTCALL_"},
    {PROD_PRINTST_PRINT, "PRINTST ::= print EXPR"},
    {PROD_RETURNST_RETURN, "RETURNST ::= return RETURNST_"},
    {PROD_RETURNST__ID, "RETURNST_ ::= id"},
    {PROD_RETURNST__EPSILON, "RETURNST_ ::= ε"},
    {PROD_IFSTMT_IF, "IFSTMT ::= if lparen EXPR rparen lbrace STMT rbrace IFSTMT_"},
    {PROD_IFSTMT__EPSILON, "IFSTMT_ ::= ε"},
    {PROD_IFSTMT__ELSE, "IFSTMT_ ::= else lbrace STMT rbrace"},
    {PROD_STMTLIST_STMT, "STMTLIST ::= STMT STMTLIST"},
    {PROD_STMTLIST_EPSILON, "STMTLIST ::= ε"},
    {PROD_EXPR_NUMEXPR, "EXPR ::= NUMEXPR EXPR_"},
    {PROD_EXPR__EPSILON, "EXPR_ ::= ε"},
    {PROD_EXPR__LT, "EXPR_ ::= lt NUMEXPR"},
    {PROD_EXPR__LE, "EXPR_ ::= le NUMEXPR"},
    {PROD_EXPR__GT, "EXPR_ ::= gt NUMEXPR"},
    {PROD_EXPR__GE, "EXPR_ ::= ge NUMEXPR"},
    {PROD_EXPR__EQ, "EXPR_ ::= eq NUMEXPR"},
    {PROD_EXPR__NE, "EXPR_ ::= ne NUMEXPR"},
    {PROD_NUMEXPR_TERM, "NUMEXPR ::= TERM NUMEXPR_"},
    {PROD_NUMEXPR__EPSILON, "NUMEXPR_ ::= ε"},
    {PROD_NUMEXPR__PLUS, "NUMEXPR_ ::= plus NUMEXPR_"},
    {PROD_NUMEXPR__MINUS, "NUMEXPR_ ::= minus NUMEXPR_"},
    {PROD_TERM_FACTOR, "TERM ::= FACTOR TERM_"},
    {PROD_TERM__EPSILON, "TERM_ ::= ε"},
    {PROD_TERM__TIMES, "TERM_ ::= times TERM"},
    {PROD_TERM__DIVIDE, "TERM_ ::= divide TERM"},
    {PROD_FACTOR_NUMEXPR, "FACTOR ::= lparen NUMEXPR rparen"},
    {PROD_FACTOR_ID, "FACTOR ::= id"},
    {PROD_FACTOR_NUM, "FACTOR ::= num"}};

void initialize_ll1_table()
{
    // S
    ll1_table[NT_S][EOF_TOKEN] = PROD_S_0;
    ll1_table[NT_S][DEF] = PROD_S_0;
    ll1_table[NT_S][LBRACE] = PROD_S_0;
    ll1_table[NT_S][SEMICOLON] = PROD_S_0;
    ll1_table[NT_S][PRINT] = PROD_S_0;
    ll1_table[NT_S][IF] = PROD_S_0;

    // MAIN
    ll1_table[NT_MAIN][EOF_TOKEN] = PROD_MAIN_EPSILON;
    ll1_table[NT_MAIN][DEF] = PROD_MAIN_FLIST;
    ll1_table[NT_MAIN][LBRACE] = PROD_MAIN_STMT;
    ll1_table[NT_MAIN][SEMICOLON] = PROD_MAIN_STMT;
    ll1_table[NT_MAIN][PRINT] = PROD_MAIN_STMT;
    ll1_table[NT_MAIN][RETURN] = PROD_MAIN_STMT;
    ll1_table[NT_MAIN][IF] = PROD_MAIN_STMT;

    // FLIST
    ll1_table[NT_FLIST][DEF] = PROD_FLIST_FDEF;

    // FLIST_
    ll1_table[NT_FLIST_][EOF_TOKEN] = PROD_FLIST__EPSILON;
    ll1_table[NT_FLIST_][DEF] = PROD_FLIST__FDEF;

    // FDEF
    ll1_table[NT_FDEF][DEF] = PROD_FDEF_DEF;

    // PARLIST
    ll1_table[NT_PARLIST][INT] = PROD_PARLIST_INT;
    ll1_table[NT_PARLIST][RPAREN] = PROD_PARLIST_EPSILON;

    // PARLIST_
    ll1_table[NT_PARLIST_][RPAREN] = PROD_PARLIST__EPSILON;
    ll1_table[NT_PARLIST_][COMMA] = PROD_PARLIST__COMMA;

    // VARLIST
    ll1_table[NT_VARLIST][ID] = PROD_VARLIST_ID;

    // VARLIST_
    ll1_table[NT_VARLIST_][COMMA] = PROD_VARLIST__COMMA;
    ll1_table[NT_VARLIST_][SEMICOLON] = PROD_VARLIST__EPSILON;

    // STMT
    ll1_table[NT_STMT][INT] = PROD_STMT_INT;
    ll1_table[NT_STMT][ID] = PROD_STMT_ATRIBST;
    ll1_table[NT_STMT][LBRACE] = PROD_STMT_BLOCK;
    ll1_table[NT_STMT][SEMICOLON] = PROD_STMT_SEMICOLON;
    ll1_table[NT_STMT][PRINT] = PROD_STMT_PRINT;
    ll1_table[NT_STMT][RETURN] = PROD_STMT_RETURN;
    ll1_table[NT_STMT][IF] = PROD_STMT_IF;

    // ATRIBST
    ll1_table[NT_ATRIBST][ID] = PROD_ATRIBST_ID;

    // ATRIBST_
    ll1_table[NT_ATRIBST_][IDFUN] = PROD_ATRIBST__FCALL;
    ll1_table[NT_ATRIBST_][LPAREN] = PROD_ATRIBST__EXPR;
    ll1_table[NT_ATRIBST_][ID] = PROD_ATRIBST__EXPR;
    ll1_table[NT_ATRIBST_][NUM] = PROD_ATRIBST__EXPR;

    // FCALL
    ll1_table[NT_FCALL][IDFUN] = PROD_FCALL_IDFUN;

    // PARLISTCALL
    ll1_table[NT_PARLISTCALL][ID] = PROD_PARLISTCALL_ID;

    // PARLISTCALL_
    ll1_table[NT_PARLISTCALL_][RPAREN] = PROD_PARLISTCALL__EPSILON;
    ll1_table[NT_PARLISTCALL_][COMMA] = PROD_PARLISTCALL__COMMA;

    // PRINTST
    ll1_table[NT_PRINTST][PRINT] = PROD_PRINTST_PRINT;

    // RETURNST
    ll1_table[NT_RETURNST][RETURN] = PROD_RETURNST_RETURN;

    // RETURNST_
    ll1_table[NT_RETURNST_][ID] = PROD_RETURNST__ID;
    ll1_table[NT_RETURNST_][SEMICOLON] = PROD_RETURNST__EPSILON;

    // IFSTMT
    ll1_table[NT_IFSTMT][IF] = PROD_IFSTMT_IF;

    // IFSTMT_
    ll1_table[NT_IFSTMT_][EOF_TOKEN] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][LBRACE] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][RBRACE] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][SEMICOLON] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][PRINT] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][RETURN] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][IF] = PROD_IFSTMT__EPSILON;
    ll1_table[NT_IFSTMT_][ELSE] = PROD_IFSTMT__ELSE;

    // STMTLIST
    ll1_table[NT_STMTLIST][INT] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][ID] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][LBRACE] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][RBRACE] = PROD_STMTLIST_EPSILON;
    ll1_table[NT_STMTLIST][SEMICOLON] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][PRINT] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][RETURN] = PROD_STMTLIST_STMT;
    ll1_table[NT_STMTLIST][IF] = PROD_STMTLIST_STMT;

    // EXPR
    ll1_table[NT_EXPR][LPAREN] = PROD_EXPR_NUMEXPR;
    ll1_table[NT_EXPR][ID] = PROD_EXPR_NUMEXPR;
    ll1_table[NT_EXPR][NUM] = PROD_EXPR_NUMEXPR;

    // EXPR_
    ll1_table[NT_EXPR_][RPAREN] = PROD_EXPR__EPSILON;
    ll1_table[NT_EXPR_][SEMICOLON] = PROD_EXPR__EPSILON;
    ll1_table[NT_EXPR_][LT] = PROD_EXPR__LT;
    ll1_table[NT_EXPR_][LE] = PROD_EXPR__LE;
    ll1_table[NT_EXPR_][GT] = PROD_EXPR__GT;
    ll1_table[NT_EXPR_][GE] = PROD_EXPR__GE;
    ll1_table[NT_EXPR_][EQ] = PROD_EXPR__EQ;
    ll1_table[NT_EXPR_][NE] = PROD_EXPR__NE;

    // NUMEXPR
    ll1_table[NT_NUMEXPR][LPAREN] = PROD_NUMEXPR_TERM;
    ll1_table[NT_NUMEXPR][ID] = PROD_NUMEXPR_TERM;
    ll1_table[NT_NUMEXPR][NUM] = PROD_NUMEXPR_TERM;

    // NUMEXPR_
    ll1_table[NT_NUMEXPR_][RPAREN] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][SEMICOLON] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][LT] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][LE] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][GT] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][GE] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][EQ] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][NE] = PROD_NUMEXPR__EPSILON;
    ll1_table[NT_NUMEXPR_][PLUS] = PROD_NUMEXPR__PLUS;
    ll1_table[NT_NUMEXPR_][MINUS] = PROD_NUMEXPR__MINUS;

    // TERM
    ll1_table[NT_TERM][LPAREN] = PROD_TERM_FACTOR;
    ll1_table[NT_TERM][ID] = PROD_TERM_FACTOR;
    ll1_table[NT_TERM][NUM] = PROD_TERM_FACTOR;

    // TERM_
    ll1_table[NT_TERM_][RPAREN] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][SEMICOLON] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][LT] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][LE] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][GT] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][GE] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][EQ] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][NE] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][PLUS] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][MINUS] = PROD_TERM__EPSILON;
    ll1_table[NT_TERM_][TIMES] = PROD_TERM__TIMES;
    ll1_table[NT_TERM_][DIVIDE] = PROD_TERM__DIVIDE;

    // FACTOR
    ll1_table[NT_FACTOR][LPAREN] = PROD_FACTOR_NUMEXPR;
    ll1_table[NT_FACTOR][ID] = PROD_FACTOR_ID;
    ll1_table[NT_FACTOR][NUM] = PROD_FACTOR_NUM;
}

inline bool is_tag(const Symbol &sym, Tag tag)
{
    return std::holds_alternative<Tag>(sym) && std::get<Tag>(sym) == tag;
}

inline bool is_terminal(const Symbol &sym)
{
    for (const auto &terminal : TERMINAIS)
    {
        if (is_tag(sym, terminal))
        {
            return true;
        }
    }
    return false;
}

inline Productions get_matrix(const NonTerminals &nt, const Tag &t)
{
    if (nt < 0 || nt >= NUM_NONTERMINALS || t < 0 || t >= NUM_TERMINALS)
    {
        throw std::out_of_range("Índice fora do intervalo da tabela LL(1)");
    }

    return ll1_table[nt][t];
}

int main(int argc, char *argv[])
{
    initialize_ll1_table();

    std::stringstream ss;
    if (argc < 2)
    {
        cout << "Nenhum arquivo informado, usando código de teste padrão.\n";
        ss = testString();
    }
    else
    {
        ss = readFile(argv[1]);
    }

    vector<Token *> tokens = analise_automatas(ss);

    cout << "Tokens encontrados:\n";
    for (const auto &token : tokens)
    {
        cout << token->toString() << ' ';
    }
    cout << endl;

    std::stack<Symbol> parseStack;

    parseStack.push(NonTerminals::NT_S); // símbolo inicial da gramática

    Token *token = tokens[0];
    tokens.push_back(new Token(Tag::EOF_TOKEN, "$")); // adiciona um token EOF para facilitar o processamento
    Symbol currentSymbol = parseStack.top();

    while (!is_tag(currentSymbol, EOF_TOKEN))
    {
        Tag tag = token->tag;
        if (Relop *relopToken = dynamic_cast<Relop *>(token))
        {
            tag = relopToken->relop;
        }
        else if (Arithop *arithopToken = dynamic_cast<Arithop *>(token))
        {
            tag = arithopToken->arithop;
        }

        if (is_tag(currentSymbol, tag))
        {
            parseStack.pop();
            cout << token->toString() << "( " << token->lexeme << " )" << endl;
            tokens.erase(tokens.begin());
            token = tokens[0];
        }
        else if (is_terminal(currentSymbol))
        {
            cout << "Erro de sintaxe: símbolo terminal inesperado '" << token->toString() << "' ao invés de '" << TAG_TO_STRING.at(std::get<Tag>(currentSymbol)) << "'." << endl;
            return 1;
        }
        else if (get_matrix(std::get<NonTerminals>(currentSymbol), tag) == EMPTY)
        {
            cout << "Erro de sintaxe: símbolo não terminal '" << NON_TERMINAL_TO_STRING[std::get<NonTerminals>(currentSymbol)] << "' não é seguido de '" << token->toString() << "'." << endl;
            return 1;
        }
        else
        {
            Productions prod = get_matrix(std::get<NonTerminals>(currentSymbol), tag);
            vector<Symbol> production = productionsMap[prod];

            // Print detailed debug info
            cout << "\n=== Debug Info ===" << endl;
            cout << "Top of stack (Non-terminal): " << NON_TERMINAL_TO_STRING[std::get<NonTerminals>(currentSymbol)] << endl;
            cout << "Current input token: " << token->toString() << " ( " << token->lexeme << " )" << endl;
            cout << "Applying production: " << PRODUCTIONS_TO_STRING[prod] << endl;

            parseStack.pop();

            // Show production symbols being pushed (if any)
            if (!production.empty())
            {
                cout << "Pushing to stack (rightmost first): ";
                for (auto it = production.rbegin(); it != production.rend(); ++it)
                {
                    if (std::holds_alternative<Tag>(*it))
                        cout << TAG_TO_STRING.at(std::get<Tag>(*it)) << " ";
                    else
                        cout << NON_TERMINAL_TO_STRING.at(std::get<NonTerminals>(*it)) << " ";
                }
                cout << endl;
            }
            else
            {
                cout << "Production is epsilon (no symbols pushed)." << endl;
            }

            // Push production symbols in reverse order
            for (auto it = production.rbegin(); it != production.rend(); ++it)
            {
                parseStack.push(*it);
            }

            // Print current stack state (top to bottom)
            cout << "Stack now: ";
            stack<Symbol> tmp = parseStack;
            vector<string> items;
            while (!tmp.empty())
            {
                const Symbol &s = tmp.top();
                if (std::holds_alternative<Tag>(s))
                    items.push_back(TAG_TO_STRING.at(std::get<Tag>(s)));
                else
                    items.push_back(NON_TERMINAL_TO_STRING.at(std::get<NonTerminals>(s)));
                tmp.pop();
            }
            reverse(items.begin(), items.end());
            for (const auto &s : items)
                cout << s << " ";
            cout << "\n==================\n"
                 << endl;
        }
        currentSymbol = parseStack.top();
    }

    return 0;
}