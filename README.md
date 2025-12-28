
### Simulador de Processador IMAS (Institute of More Advanced Studies)

Este projeto consiste na implementação de um simulador funcional do processador IMAS, uma arquitetura de 16 bits baseada no modelo de von Neumann. O simulador reproduz o ciclo de instrução de três estágios: Busca, Decodificação e Execução.

---

### 1. Estrutura de Arquivos e Pastas

* imas.h: Define a estrutura imas_t com os registradores de 16 bits (PC, MAR, IBR, IR, MBR, AC, MQ) e a memória de 4KB.
* imas.c: Contém a implementação da lógica da CPU, os subciclos de instrução e as funções obrigatórias de Memória e E/S.
* Makefile: Arquivo para automatizar a compilação do código.
* testes/: Pasta contendo os arquivos de memória em hexadecimal para validação (ex1.txt, ex2.txt, ex3.txt).

---

### 2. Como Compilar

Certifique-se de estar no diretório raiz do projeto e utilize o terminal Ubuntu:

# Comando para compilar manualmente via GCC
gcc imas.c -o imas

# Ou, se preferir usar o Makefile
make

---

### 3. Como Testar os Casos de Uso

O simulador recebe o arquivo de texto e endereços de breakpoints (em hexadecimal) para imprimir o estado dos registradores.

### Passo a Passo: Exemplo 1 (Aritmética Básica)
Valida as instruções LOAD, ADD, SUB, STOR e HALT.
1. Comando: ./imas testes/ex1.txt 001 002
2. Verificação: No PC 0x0001, o AC deve ser 0x0035. No PC 0x0002, o AC deve ser 0x0010.

### Passo a Passo: Exemplo 2 (Entrada e Saída)
Valida as instruções IN (leitura para AC) e OUT (escrita do AC).
1. Comando: ./imas testes/ex2.txt 003
2. Interação: Quando aparecer IN =>, digite 1 e Enter. No segundo IN =>, digite 2 e Enter.
3. Verificação: O AC no breakpoint deve ser 0x0003 e a saída final OUT => 3.

### Passo a Passo: Exemplo 3 (Loops e Instrução STA)
Valida saltos (JPOS) e a modificação de campos de endereço na memória (STA).
1. Comando: ./imas testes/ex3.txt 010
2. Interação: Insira os valores 1, 2 e 3.
3. Verificação: O AC final no breakpoint 010 deve ser 0x0006 e a saída final OUT => 6.

---

### 4. Funcionamento do Ciclo de Instrução

O simulador executa três estágios obrigatórios por instrução:

1. Busca (Fetch): O endereço no PC vai para o MAR, a instrução é buscada na memória para o MBR, o PC é incrementado e a instrução é movida para o IBR.
2. Decodificação (Decode): O opcode (bits 0-3 do IBR) vai para o IR e o endereço do operando (bits 4-15 do IBR) vai para o MAR.
3. Execução (Execute): A instrução é processada na ULA ou Unidade de Controle via bloco switch.



[Image of von Neumann architecture diagram]


---

### 5. Regras de Ouro (Requisitos do Projeto)

* PC e PC+: O simulador deve imprimir o PC (endereço atual) e o PC+ (endereço após a execução ou desvio).
* Instrução STA (Opcode 5): Deve alterar apenas os 12 bits de endereço da instrução alvo na memória, preservando o opcode original.
* Acesso à Memória: Todas as interações com a memória devem usar obrigatoriamente MAR e MBR através das funções memory_read e memory_write.
* Variáveis: Não é permitido o uso de variáveis globais ou auxiliares extras; deve-se usar apenas a estrutura imas_t.
* Sinais: AC, MQ e MBR são tratados como inteiros com sinal (int16_t) para garantir a aritmética correta.
