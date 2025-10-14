# UMA NOVA ABORDAGEM PARA TESTES AUTOMATIZADOS DE APLICAÇÕES DESKTOP UTILIZANDO DYNAMIC LINK-LIBRARY

Este repositório contém o **código-fonte em C** do **protótipo inicial** do software desenvolvido para o Trabalho de Conclusão de Curso (TCC) intitulado:

> **"Uma Nova Abordagem para Testes Automatizados de Aplicações Desktop Utilizando Dynamic Link-Library"**

---

## Estrutura do Projeto

O projeto está organizado em:

### 1️⃣ `app-with-gui`
Contém a versão do protótipo com **interface gráfica**:
- `main.c` → arquivo fonte principal que cria a janela e os componentes GUI.
- `typeText.c` → código responsável por simular a digitação automática.
- - `typeText.h` → arquivo de exemplo de entrada de texto.
- `test.txt` → arquivo de exemplo de entrada de texto.
- `typingTool.exe` → executável gerado após compilação.

## Como Compilar e Executar

### 1️⃣ Compilação
```bash
gcc main.c typeText.c -o TypingTool.exe -lcomctl32 -lgdi32 -municode
```
O que este código faz:
- `gcc` → invoca o GNU Compiler Collection, que compila programas em C.
- `main.c` → arquivo fonte principal do projeto.
- `-o typingTool.exe` → indica o nome do executável de saída. O compilador gera o arquivo `typingTool.exe` que pode ser executado no Windows.

### 2️⃣ Execução
```bash
.\typingTool.exe
```
O que este código faz:
- `.\` → indica que o executável está no diretório atual.
- `typingTool.exe` → é o arquivo executável gerado na compilação.
- `-o typingTool.exe` → indica o nome do executável de saída. O compilador gera o arquivo 
No caso, uma janela é aberta com um campo de texto e um botão que, quando acionado, simula a digitação do texto digitado.

### Funcionalidade Principal
O protótipo realiza simulação de digitação automática no Windows, utilizando funções da API do Windows (WinAPI), que dependem de DLLs nativas como:

- `user32.dll` → para enviar eventos de teclado e criar a interface gráfica;
- `kernel32.dll` → para funções básicas do sistema, como Sleep().

Dessa forma, o software demonstra a viabilidade da automação de testes de aplicações desktop utilizando DLLs.

### Autor
Thalis Ferreira
Faculdade: Faculdade FIED

Curso: Sistemas de Informação

Ano: 2025
