# Trabalho Compiladores Parte 1

## Analisador Léxico - Parte A

### Estrutura dos arquivos

- `automata.h` → Definição dos autômatos.
- `automata.cpp` → Implementação dos autômatos de transição.
- `main.cpp` → Implementação do analisador léxico (lexer manual).
- `entrada_valida.txt` → Exemplo de entrada correta (sem erros).
- `entrada_invalida.txt` → Exemplo de entrada com erro léxico.
- `README.md` → Este arquivo de instruções.

---

### Como compilar

No terminal Linux, compile usando:

```bash
g++ main.cpp automata.cpp
./main entrada_valida.txt
```

## Analisador Léxico Flex - Parte B

### Como compilar

No terminal Linux, compile usando:

```bash
flex lexer.l
gcc lex.yy.c -o lexer -lfl
./lexer entrada_valida.txt
```
