# Analisador Léxico - Parte A

## Descrição

Este projeto implementa a Parte A do trabalho de Analisadores Léxicos: um lexer manual baseado em **autômatos de transição** para reconhecer:

- **Identificadores**: letras seguidas de letras ou dígitos.
- **Constantes Numéricas Inteiras**: sequência de dígitos decimais.
- **Operadores Relacionais**: `<`, `>`, `=`, `<=`, `>=`, `!=`.

Também é feita a distinção de **palavras-chave** (`def`, `int`, `if`, `else`, `print`, `return`) através de uma **tabela de símbolos**.

---

## Estrutura dos arquivos

- `automata.h` → Definição dos autômatos.
- `automata.cpp` → Implementação dos autômatos de transição.
- `main.cpp` → Implementação do analisador léxico (lexer manual).
- `entrada_valida.txt` → Exemplo de entrada correta (sem erros).
- `entrada_invalida.txt` → Exemplo de entrada com erro léxico.
- `README.md` → Este arquivo de instruções.

---

## Como compilar

No terminal Linux, compile usando:

```bash
g++ main.cpp automata.cpp
./main <path_do_arquivo>
```
