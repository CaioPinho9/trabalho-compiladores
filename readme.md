# Trabalho Compiladores Parte 3

## Analisador Léxico - Parte A

### Estrutura dos arquivos

- `automata.h` → Definição dos autômatos.
- `automata.cpp` → Implementação dos autômatos de transição.
- `lexer.cpp` → Implementação do analisador léxico (lexer manual).
- `lexer.h` → Definição das funções e estruturas do analisador léxico.

## Analisador Sintáico - Parte C

### Estrutura dos arquivos

- `parser.cpp` → Implementação do parser sintático.
- `entrada_valida.txt` → Exemplo de entrada correta (sem erros).
- `entrada_invalida1.txt` → Exemplo de entrada com erro sintático, não pode funções dentro de funções.
- `entrada_invalida2.txt` → Exemplo de entrada com erro sintático, não pode else if.
- `README.md` → Este arquivo de instruções.

---

### Como compilar

No terminal Linux, compile usando:

```bash
g++ parser.cpp lexer.cpp automata.cpp
./a.out entrada_valida.txt
```

## Analisador Léxico Flex - Parte B

### Como compilar

No terminal Linux, compile usando:

```bash
flex lexer.l
gcc lex.yy.c -o lexer -lfl
./lexer entrada_valida.txt
```
