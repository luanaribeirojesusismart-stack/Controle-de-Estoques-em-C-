# 📦 Controle de Estoque em C - Feito por Luana Ribeiro de Jesus

Programa de terminal em C para gerenciamento de um pequeno estoque (com capacidade para até 1.000 itens), garantindo a persistência dos dados no arquivo `inventory.txt` entre as execuções. Projeto final desenvolvido para a sprint da **Beringh Academy**.

## ⚙️ Funcionalidades

- **1. Adicionar Item:** Cadastra um novo produto ou soma a quantidade caso o item já exista no estoque.
- **2. Remover Item:** Subtrai a quantidade informada de um produto existente e exclui o registro automaticamente caso o estoque chegue a zero.
- **3. Listar Estoque:** Lê os dados do arquivo `inventory.txt` e exibe todos os produtos cadastrados e suas quantidades atuais.
- **4. Sair:** Encerra a execução do sistema de forma segura.
- **Validação de Entradas:** Tratamento contra opções inválidas, textos vazios, valores negativos e limpeza de buffer para evitar falhas na leitura do teclado.

## 🗂️ Estrutura do Projeto

- `main.c` — Código-fonte principal contendo a lógica do menu, validações e manipulação de arquivos.
- `inventory.txt` — Arquivo de texto utilizado para armazenar os itens (cada produto ocupa duas linhas consecutivas: nome na primeira linha e quantidade inteira na segunda).

### Exemplo do formato de `inventory.txt`

```text
Notebook
10
Caneta
50
```

## 🚀 Como Executar

### Pré-requisitos
É necessário ter um compilador C instalado na máquina (como o **GCC**).

### Passo a passo

1. **Clone o repositório:**
   ```bash
   git clone https://github.com
   ```

2. **Acesse a pasta do projeto:**
   ```bash
   cd nome-do-repositorio
   ```

3. **Compile o código-fonte:**
   ```bash
   gcc main.c -o controle_estoque
   ```

4. **Execute o programa:**
   - **Linux/macOS:**
     ```bash
     ./controle_estoque
     ```
   - **Windows:**
     ```bash
     controle_estoque.exe
     ```
