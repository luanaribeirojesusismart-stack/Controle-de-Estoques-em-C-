#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "inventory.txt"
#define MAX_ITENS 1000
#define TAM_NOME 100

typedef struct {
    char nome[TAM_NOME];
    int quantidade;
} Item;

/* ---------- Funcoes auxiliares de entrada ---------- */

/* Remove \n e \r do final da string */
void remover_quebra_linha(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[--n] = '\0';
    }
}

/* Le uma linha do teclado. Retorna 0 se chegou ao fim da entrada (EOF). */
int ler_linha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        return 0;
    }
    /* Se a linha era maior que o buffer, descarta o resto */
    if (strchr(buffer, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    remover_quebra_linha(buffer);
    return 1;
}

/* Le um inteiro. Retorna 1 se valido, 0 caso contrario. */
int ler_inteiro(int *valor) {
    char buffer[64];
    char *fim;
    long n;

    if (!ler_linha(buffer, sizeof(buffer))) {
        return 0;
    }
    if (buffer[0] == '\0') {
        return 0;
    }
    n = strtol(buffer, &fim, 10);
    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }
    if (*fim != '\0' || n < -2147483647L || n > 2147483647L) {
        return 0;
    }
    *valor = (int)n;
    return 1;
}

/* ---------- Funcoes de arquivo ---------- */

/* Carrega todos os itens do arquivo. Retorna a quantidade de itens lidos. */
int carregar_itens(Item itens[]) {
    FILE *f = fopen(ARQUIVO, "r");
    char linha_nome[TAM_NOME + 2];
    char linha_qtd[64];
    int total = 0;

    if (f == NULL) {
        return 0;
    }

    while (total < MAX_ITENS && fgets(linha_nome, sizeof(linha_nome), f) != NULL) {
        remover_quebra_linha(linha_nome);
        if (linha_nome[0] == '\0') {
            continue; /* ignora linhas em branco */
        }
        if (fgets(linha_qtd, sizeof(linha_qtd), f) == NULL) {
            break;
        }
        strncpy(itens[total].nome, linha_nome, TAM_NOME - 1);
        itens[total].nome[TAM_NOME - 1] = '\0';
        itens[total].quantidade = atoi(linha_qtd);
        total++;
    }

    fclose(f);
    return total;
}

/* Salva todos os itens no arquivo (duas linhas por item). */
void salvar_itens(Item itens[], int total) {
    FILE *f = fopen(ARQUIVO, "w");
    int i;

    if (f == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", ARQUIVO);
        return;
    }
    for (i = 0; i < total; i++) {
        fprintf(f, "%s\n%d\n", itens[i].nome, itens[i].quantidade);
    }
    fclose(f);
}

/* Procura um item pelo nome. Retorna o indice ou -1. */
int buscar_item(Item itens[], int total, const char *nome) {
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(itens[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/* ---------- Operacoes do menu ---------- */

void adicionar_item(void) {
    Item itens[MAX_ITENS];
    char nome[TAM_NOME];
    int quantidade;
    int total, pos;

    printf("Digite o nome do item: ");
    if (!ler_linha(nome, sizeof(nome)) || nome[0] == '\0') {
        printf("Nome inválido!\n");
        return;
    }

    printf("Digite a quantidade: ");
    if (!ler_inteiro(&quantidade) || quantidade <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    total = carregar_itens(itens);
    pos = buscar_item(itens, total, nome);

    if (pos >= 0) {
        /* Item ja existe: soma a quantidade */
        itens[pos].quantidade += quantidade;
    } else {
        if (total >= MAX_ITENS) {
            printf("Estoque cheio!\n");
            return;
        }
        strncpy(itens[total].nome, nome, TAM_NOME - 1);
        itens[total].nome[TAM_NOME - 1] = '\0';
        itens[total].quantidade = quantidade;
        total++;
    }

    salvar_itens(itens, total);
    printf("Item adicionado com sucesso!\n");
}

void remover_item(void) {
    Item itens[MAX_ITENS];
    char nome[TAM_NOME];
    int quantidade;
    int total, pos, i;

    printf("Digite o nome do item: ");
    if (!ler_linha(nome, sizeof(nome)) || nome[0] == '\0') {
        printf("Nome inválido!\n");
        return;
    }

    printf("Digite a quantidade a ser removida: ");
    if (!ler_inteiro(&quantidade) || quantidade <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    total = carregar_itens(itens);
    pos = buscar_item(itens, total, nome);

    if (pos < 0) {
        printf("Item não encontrado.\n");
        return;
    }

    if (quantidade > itens[pos].quantidade) {
        printf("Estoque insuficiente. Quantidade disponível: %d\n", itens[pos].quantidade);
        return;
    }

    itens[pos].quantidade -= quantidade;

    if (itens[pos].quantidade == 0) {
        /* Remove o item deslocando os seguintes */
        for (i = pos; i < total - 1; i++) {
            itens[i] = itens[i + 1];
        }
        total--;
        salvar_itens(itens, total);
        printf("Item removido do estoque!\n");
    } else {
        salvar_itens(itens, total);
        printf("Quantidade atualizada com sucesso!\n");
    }
}

void listar_estoque(void) {
    Item itens[MAX_ITENS];
    int total = carregar_itens(itens);
    int i;

    if (total == 0) {
        printf("O estoque está vazio.\n");
        return;
    }

    printf("=============================\n");
    printf("        Estoque Atual\n");
    printf("=============================\n");
    for (i = 0; i < total; i++) {
        printf("Nome: %s\n", itens[i].nome);
        printf("Quantidade: %d\n\n", itens[i].quantidade);
    }
}

void exibir_menu(void) {
    printf("================================\n");
    printf("   Controle de Estoque\n");
    printf("================================\n");
    printf("Selecione uma opção:\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Estoque\n");
    printf("4. Sair\n");
    printf("Opção: ");
}

void mensagem_despedida(void) {
    printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");
}

/* ---------- Programa principal ---------- */

int main(void) {
    int opcao;
    char resposta[16];

    while (1) {
        /* Mostra o menu ate receber uma opcao valida (1 a 4) */
        while (1) {
            exibir_menu();
            if (ler_inteiro(&opcao) && opcao >= 1 && opcao <= 4) {
                break;
            }
            /* Fim da entrada (EOF): encerra para nao entrar em loop infinito */
            if (feof(stdin)) {
                printf("\n");
                mensagem_despedida();
                return 0;
            }
            printf("Opção inválida! Tente novamente.\n");
        }

        if (opcao == 4) {
            mensagem_despedida();
            return 0;
        }

        switch (opcao) {
            case 1: adicionar_item(); break;
            case 2: remover_item();   break;
            case 3: listar_estoque(); break;
        }

        printf("Deseja realizar outra operação? (s/n): ");
        if (!ler_linha(resposta, sizeof(resposta))) {
            printf("\n");
            mensagem_despedida();
            return 0;
        }
        if (resposta[0] != 's' && resposta[0] != 'S') {
            mensagem_despedida();
            return 0;
        }
        if (resposta[1] != '\0') {
            /* "sim", "sss" etc. nao sao exatamente 's' */
            mensagem_despedida();
            return 0;
        }
    }

    return 0;
}
