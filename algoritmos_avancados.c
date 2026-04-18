#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================
   MANSÃO SOMBRIA — EXPLORAÇÃO EM ÁRVORE BINÁRIA
   Cada nó é uma sala com saída esquerda e direita.
   O jogador navega escolhendo o caminho e pode voltar.
   ================================================ */

#define MAX_HISTORICO 20

/* --- Struct Sala: nó da árvore binária --- */
typedef struct Sala {
    char        nome[50];
    char        descricao[120]; /* Descrição atmosférica da sala */
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* ================================================
   FUNÇÃO: criarSala
   Aloca um novo nó com nome e descrição fornecidos.
   ================================================ */
Sala* criarSala(char nome[], char descricao[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { printf("⚠ Erro de memória!\n"); exit(1); }
    strcpy(nova->nome, nome);
    strcpy(nova->descricao, descricao);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

/* ================================================
   FUNÇÃO: liberarArvore
   Percorre a árvore em pós-ordem e libera cada nó.
   CORREÇÃO: o código original não liberava memória.
   ================================================ */
void liberarArvore(Sala *sala) {
    if (sala == NULL) return;
    liberarArvore(sala->esq);
    liberarArvore(sala->dir);
    free(sala);
}

/* ================================================
   FUNÇÃO: exibirHistorico
   Mostra o caminho percorrido até a sala atual.
   ================================================ */
void exibirHistorico(Sala *historico[], int tamanho) {
    printf("  🗺  Caminho: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%s", historico[i]->nome);
        if (i < tamanho - 1) printf(" → ");
    }
    printf("\n");
}

/* ================================================
   FUNÇÃO: explorarSalas
   Navegação interativa pela árvore.
   ADIÇÕES: histórico de caminho, opção de voltar,
            descrição de cada sala.
   ================================================ */
void explorarSalas(Sala *raiz) {
    Sala *historico[MAX_HISTORICO]; /* Pilha de salas visitadas */
    int  profundidade = 0;
    Sala *atual = raiz;

    printf("\n🕯  Bem-vindo à Mansão Sombria...\n");
    printf("    Explore os cômodos e descubra seus segredos.\n");

    while (atual != NULL) {
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║  📍 %-42s║\n", atual->nome);
        printf("╠══════════════════════════════════════════════╣\n");
        printf("║  %s\n", atual->descricao);
        printf("╚══════════════════════════════════════════════╝\n");

        /* Exibe o caminho percorrido até aqui */
        if (profundidade > 0) exibirHistorico(historico, profundidade);

        /* Sala sem saídas: fim de ramo */
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("\n  🚪 Este cômodo não possui mais saídas.\n");
            if (profundidade == 0) {
                printf("  Exploração concluída!\n\n");
                return;
            }
            printf("  Voltando para o cômodo anterior...\n");
            atual = historico[--profundidade];
            continue;
        }

        /* Exibe opções disponíveis */
        printf("\nEscolha um caminho:\n");
        if (atual->esq) printf("  [e] Esquerda → %s\n", atual->esq->nome);
        if (atual->dir) printf("  [d] Direita  → %s\n", atual->dir->nome);
        if (profundidade > 0) printf("  [v] Voltar   → %s\n",
                                     historico[profundidade - 1]->nome);
        printf("  [s] Sair da mansão\n");
        printf("Opção: ");

        char escolha;
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esq) {
            /* Empilha sala atual no histórico antes de avançar */
            if (profundidade < MAX_HISTORICO)
                historico[profundidade++] = atual;
            atual = atual->esq;

        } else if (escolha == 'd' && atual->dir) {
            if (profundidade < MAX_HISTORICO)
                historico[profundidade++] = atual;
            atual = atual->dir;

        } else if (escolha == 'v' && profundidade > 0) {
            /* Volta para a sala anterior desempilhando o histórico */
            atual = historico[--profundidade];

        } else if (escolha == 's') {
            printf("\n  🚪 Você foge da mansão às pressas...\n\n");
            return;

        } else {
            printf("  ⚠ Caminho inválido! Tente novamente.\n");
        }
    }
}

/* ================================================
   FUNÇÃO: exibirMapa
   Percurso pré-ordem para mostrar todas as salas.
   ================================================ */
void exibirMapa(Sala *sala, int nivel) {
    if (!sala) return;
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("%s%s\n", nivel == 0 ? "🏚 " : "└─ ", sala->nome);
    exibirMapa(sala->esq, nivel + 1);
    exibirMapa(sala->dir, nivel + 1);
}

/* ================================================
   MAIN: monta a árvore e inicia a exploração
   ================================================ */
int main() {
    /* Criando todas as salas com nome e descrição */
    Sala *hall       = criarSala("Hall de Entrada",
                        "Lustres quebrados piscam. O cheiro de mofo é forte.");
    Sala *estar      = criarSala("Sala de Estar",
                        "Móveis cobertos por lençóis. Um relógio parado nas 3h.");
    Sala *cozinha    = criarSala("Cozinha",
                        "Panelas enferrujadas. Algo se move atrás do fogão.");
    Sala *biblioteca = criarSala("Biblioteca",
                        "Livros caídos por toda parte. Um diário está aberto.");
    Sala *jardim     = criarSala("Jardim",
                        "Vegetação selvagem. Uma fonte seca com água escura.");
    Sala *quarto     = criarSala("Quarto",
                        "Cama desfeita. Marcas de arranhão nas paredes.");
    Sala *banheiro   = criarSala("Banheiro",
                        "Espelho rachado. A torneira pinga sem parar.");
    Sala *sotao      = criarSala("Sótão",
                        "Caixas velhas por toda parte. Há um baú trancado.");
    Sala *adega      = criarSala("Adega",
                        "Garrafas empoeiradas. Uma porta secreta entreaberta.");

    /* Montando a árvore — mapa da mansão:
                    Hall
                  /      \
              Estar      Cozinha
             /    \      /    \
          Biblio  Jardim Quarto Banheiro
                         /    \
                       Sótão  Adega               */
    hall->esq      = estar;
    hall->dir      = cozinha;
    estar->esq     = biblioteca;
    estar->dir     = jardim;
    cozinha->esq   = quarto;
    cozinha->dir   = banheiro;
    quarto->esq    = sotao;
    quarto->dir    = adega;

    /* Exibe o mapa completo antes de começar */
    printf("\n📜 MAPA DA MANSÃO:\n");
    exibirMapa(hall, 0);

    /* Inicia a exploração a partir do Hall */
    explorarSalas(hall);

    /* Libera toda a memória alocada — CORREÇÃO do código original */
    liberarArvore(hall);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================
   MANSÃO DO MISTÉRIO — ÁRVORE DE SALAS + BST DE PISTAS
   Navegação pela mansão coleta pistas automaticamente.
   BST organiza as pistas em ordem alfabética.
   ================================================ */

#define MAX_HISTORICO 20

/* --- Struct Sala: nó da árvore binária de navegação --- */
typedef struct Sala {
    char        nome[50];
    char        pista[100];  /* Pista encontrada nesta sala (pode ser vazia) */
    int         visitada;    /* Flag: evita coletar a mesma pista duas vezes */
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* --- Struct PistaNode: nó da BST de pistas coletadas --- */
typedef struct PistaNode {
    char             pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* ================================================
   ÁRVORE DE SALAS
   ================================================ */

Sala* criarSala(char nome[], char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { printf("⚠ Erro de memória!\n"); exit(1); }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->visitada = 0;
    nova->esq = nova->dir = NULL;
    return nova;
}

/* Libera a árvore de salas em pós-ordem */
void liberarSalas(Sala *sala) {
    if (!sala) return;
    liberarSalas(sala->esq);
    liberarSalas(sala->dir);
    free(sala);
}

/* Mapa visual em pré-ordem com indentação por nível */
void exibirMapa(Sala *sala, int nivel) {
    if (!sala) return;
    for (int i = 0; i < nivel; i++) printf("   ");
    printf("%s%s%s\n",
           nivel == 0 ? "🏚 " : "└─ ",
           sala->nome,
           sala->visitada ? " ✔" : "");
    exibirMapa(sala->esq, nivel + 1);
    exibirMapa(sala->dir, nivel + 1);
}

/* ================================================
   BST DE PISTAS
   ================================================ */

/* Insere pista na BST em ordem alfabética */
PistaNode* inserirPista(PistaNode *raiz, char pista[]) {
    if (!raiz) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novo) { printf("⚠ Erro de memória!\n"); exit(1); }
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0)      /* CORREÇÃO: ignora duplicatas (cmp == 0) */
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

/* Exibe todas as pistas em ordem alfabética (in-ordem) */
void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf("  🔍 %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

/* Conta total de pistas na BST */
int contarPistas(PistaNode *raiz) {
    if (!raiz) return 0;
    return 1 + contarPistas(raiz->esq) + contarPistas(raiz->dir);
}

/* Busca uma pista pelo texto exato na BST */
PistaNode* buscarPista(PistaNode *raiz, char pista[]) {
    if (!raiz) return NULL;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0)  return raiz;
    if (cmp < 0)   return buscarPista(raiz->esq, pista);
    return             buscarPista(raiz->dir, pista);
}

/* Libera a BST de pistas em pós-ordem */
void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

/* ================================================
   EXPLORAÇÃO
   ================================================ */

void explorarSalas(Sala *raiz, PistaNode **arvorePistas) {
    Sala *historico[MAX_HISTORICO]; /* Pilha para opção "voltar" */
    int   prof = 0;
    Sala *atual = raiz;

    printf("\n🕯  A investigação começa no Hall de Entrada...\n");

    while (atual != NULL) {

        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║  📍 %-42s║\n", atual->nome);
        printf("╠══════════════════════════════════════════════╣\n");

        /* Coleta a pista da sala na primeira visita */
        if (strlen(atual->pista) > 0 && !atual->visitada) {
            printf("║  🔍 Pista encontrada: %-24s║\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            atual->visitada = 1;
        } else if (strlen(atual->pista) == 0) {
            printf("║  💨 Nenhuma pista nesta sala.                ║\n");
        } else {
            printf("║  ✔  Pista já coletada anteriormente.         ║\n");
        }

        printf("╚══════════════════════════════════════════════╝\n");

        /* Sala sem saídas: fim de ramo */
        if (!atual->esq && !atual->dir) {
            printf("  🚪 Sem mais saídas neste corredor.\n");
            if (prof == 0) { printf("  Exploração encerrada!\n"); return; }
            printf("  Voltando...\n");
            atual = historico[--prof];
            continue;
        }

        /* Menu de navegação */
        printf("\nCaminhos disponíveis:\n");
        if (atual->esq) printf("  [e] Esquerda → %s\n", atual->esq->nome);
        if (atual->dir) printf("  [d] Direita  → %s\n", atual->dir->nome);
        if (prof > 0)   printf("  [v] Voltar   → %s\n", historico[prof-1]->nome);
        printf("  [m] Ver mapa da mansão\n");
        printf("  [p] Ver pistas coletadas\n");
        printf("  [s] Sair da investigação\n");
        printf("Opção: ");

        char escolha;
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esq) {
            if (prof < MAX_HISTORICO) historico[prof++] = atual;
            atual = atual->esq;
        } else if (escolha == 'd' && atual->dir) {
            if (prof < MAX_HISTORICO) historico[prof++] = atual;
            atual = atual->dir;
        } else if (escolha == 'v' && prof > 0) {
            atual = historico[--prof];
        } else if (escolha == 'm') {
            printf("\n📜 MAPA DA MANSÃO:\n");
            exibirMapa(raiz, 0);
        } else if (escolha == 'p') {
            int total = contarPistas(*arvorePistas);
            printf("\n🗂  Pistas coletadas até agora (%d):\n", total);
            if (total == 0) printf("  Nenhuma pista ainda.\n");
            else exibirPistas(*arvorePistas);
        } else if (escolha == 's') {
            printf("\n  🚪 Saindo da investigação...\n");
            return;
        } else {
            printf("  ⚠ Opção inválida!\n");
        }
    }
}

/* ================================================
   MAIN
   ================================================ */

int main() {
    /* Criando salas com nome e pista */
    Sala *hall       = criarSala("Hall de Entrada",  "");
    Sala *sala       = criarSala("Sala de Estar",    "Pegadas no tapete");
    Sala *cozinha    = criarSala("Cozinha",           "Faca ensanguentada");
    Sala *biblioteca = criarSala("Biblioteca",        "Livro fora do lugar");
    Sala *jardim     = criarSala("Jardim",            "Terra remexida");
    Sala *quarto     = criarSala("Quarto",            "Janela aberta");
    Sala *banheiro   = criarSala("Banheiro",          "");
    Sala *sotao      = criarSala("Sótão",             "Baú arrombado");
    Sala *adega      = criarSala("Adega",             "Garrafa quebrada");

    /* Montando a árvore — mapa da mansão:
                    Hall
                  /      \
              Sala       Cozinha
             /    \      /     \
          Biblio  Jardim Quarto Banheiro
                         /    \
                       Sótão  Adega           */
    hall->esq      = sala;
    hall->dir      = cozinha;
    sala->esq      = biblioteca;
    sala->dir      = jardim;
    cozinha->esq   = quarto;
    cozinha->dir   = banheiro;
    quarto->esq    = sotao;
    quarto->dir    = adega;

    /* Exibe mapa inicial */
    printf("\n📜 MAPA DA MANSÃO:\n");
    exibirMapa(hall, 0);

    /* Inicia exploração */
    PistaNode *pistas = NULL;
    explorarSalas(hall, &pistas);

    /* Relatório final */
    int total = contarPistas(pistas);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║        🗂  RELATÓRIO FINAL DA INVESTIGAÇÃO   ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  Pistas coletadas: %-26d║\n", total);
    printf("╚══════════════════════════════════════════════╝\n");

    if (total == 0) {
        printf("  Nenhuma pista foi coletada.\n\n");
    } else {
        printf("  Pistas em ordem alfabética:\n");
        exibirPistas(pistas);

        /* Demonstração da busca na BST */
        char alvo[] = "Faca ensanguentada";
        printf("\n🔎 Buscando: \"%s\"\n", alvo);
        if (buscarPista(pistas, alvo))
            printf("  ✔ Pista confirmada no arquivo!\n\n");
        else
            printf("  ✘ Pista não encontrada.\n\n");
    }

    /* Libera toda a memória alocada */
    liberarSalas(hall);
    liberarPistas(pistas);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================
   MANSÃO DO MISTÉRIO — SISTEMA COMPLETO
   Árvore de Salas + BST de Pistas + Tabela Hash
   (Pista → Suspeito)
   ================================================ */

#define TAM_HASH     10
#define MAX_HIST     20
#define MIN_PROVAS    2  /* Mínimo de pistas para confirmar culpado */

/* ================================================
   STRUCTS
   ================================================ */

/* Nó da árvore binária de navegação */
typedef struct Sala {
    char        nome[50];
    char        pista[100];
    int         visitada;    /* Flag: evita coletar pista duas vezes */
    struct Sala *esq, *dir;
} Sala;

/* Nó da BST de pistas coletadas */
typedef struct PistaNode {
    char             pista[100];
    struct PistaNode *esq, *dir;
} PistaNode;

/* Nó da tabela hash: pista → suspeito */
typedef struct HashNode {
    char             pista[100];
    char             suspeito[50];
    struct HashNode *prox;  /* Encadeamento para colisões */
} HashNode;

/* Tabela hash global inicializada no main */
HashNode *tabela[TAM_HASH];

/* ================================================
   TABELA HASH
   ================================================ */

/* Função hash: soma dos ASCII módulo TAM_HASH */
int hashFunc(char *str) {
    unsigned int soma = 0;
    for (int i = 0; str[i]; i++) soma += (unsigned char)str[i];
    return soma % TAM_HASH;
}

/* Insere par pista→suspeito.
   CORREÇÃO: verifica duplicata antes de inserir */
void inserirNaHash(char *pista, char *suspeito) {
    int idx = hashFunc(pista);
    HashNode *aux = tabela[idx];

    /* Verifica se a pista já existe na cadeia */
    while (aux) {
        if (strcmp(aux->pista, pista) == 0) return; /* Duplicata: ignora */
        aux = aux->prox;
    }

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) { printf("⚠ Erro de memória!\n"); exit(1); }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox   = tabela[idx]; /* Insere no início da cadeia */
    tabela[idx]  = novo;
}

/* Busca suspeito pela pista — retorna "Desconhecido" se não achar */
char* encontrarSuspeito(char *pista) {
    int idx = hashFunc(pista);
    HashNode *aux = tabela[idx];
    while (aux) {
        if (strcmp(aux->pista, pista) == 0) return aux->suspeito;
        aux = aux->prox;
    }
    return "Desconhecido";
}

/* Libera toda a tabela hash */
void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *aux = tabela[i];
        while (aux) {
            HashNode *temp = aux;
            aux = aux->prox;
            free(temp);
        }
        tabela[i] = NULL;
    }
}

/* ================================================
   ÁRVORE DE SALAS
   ================================================ */

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { printf("⚠ Erro de memória!\n"); exit(1); }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->visitada = 0;
    nova->esq = nova->dir = NULL;
    return nova;
}

/* Libera árvore de salas em pós-ordem */
void liberarSalas(Sala *s) {
    if (!s) return;
    liberarSalas(s->esq);
    liberarSalas(s->dir);
    free(s);
}

/* Mapa visual em pré-ordem com indentação por nível */
void exibirMapa(Sala *s, int nivel) {
    if (!s) return;
    for (int i = 0; i < nivel; i++) printf("   ");
    printf("%s%s%s\n",
           nivel == 0 ? "🏚 " : "└─ ",
           s->nome,
           s->visitada ? " ✔" : "");
    exibirMapa(s->esq, nivel + 1);
    exibirMapa(s->dir, nivel + 1);
}

/* ================================================
   BST DE PISTAS
   ================================================ */

/* Insere pista na BST em ordem alfabética.
   CORREÇÃO: ignora duplicatas (cmp == 0) */
PistaNode* inserirPista(PistaNode *raiz, char *pista) {
    if (!raiz) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novo) { printf("⚠ Erro de memória!\n"); exit(1); }
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if      (cmp < 0) raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, pista);
    /* cmp == 0: duplicata, não insere */
    return raiz;
}

/* Exibe pistas em ordem alfabética com suspeito vinculado */
void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf("  🔍 %-35s → Suspeito: %s\n",
           raiz->pista, encontrarSuspeito(raiz->pista));
    exibirPistas(raiz->dir);
}

/* Conta pistas que apontam para um suspeito específico */
int contarPistasPorSuspeito(PistaNode *raiz, char *suspeito) {
    if (!raiz) return 0;
    int conta = strcmp(encontrarSuspeito(raiz->pista), suspeito) == 0 ? 1 : 0;
    return conta
         + contarPistasPorSuspeito(raiz->esq, suspeito)
         + contarPistasPorSuspeito(raiz->dir, suspeito);
}

/* Lista suspeitos únicos presentes nas pistas coletadas */
void listarSuspeitos(PistaNode *raiz,
                     char suspeitos[][50], int *total) {
    if (!raiz) return;
    listarSuspeitos(raiz->esq, suspeitos, total);

    char *s = encontrarSuspeito(raiz->pista);
    if (strcmp(s, "Desconhecido") != 0) {
        /* Verifica se já está na lista */
        int achou = 0;
        for (int i = 0; i < *total; i++)
            if (strcmp(suspeitos[i], s) == 0) { achou = 1; break; }
        if (!achou) strcpy(suspeitos[(*total)++], s);
    }

    listarSuspeitos(raiz->dir, suspeitos, total);
}

/* Libera BST de pistas em pós-ordem */
void liberarBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarBST(raiz->esq);
    liberarBST(raiz->dir);
    free(raiz);
}

/* ================================================
   EXPLORAÇÃO
   ================================================ */

void explorar(Sala *raiz, PistaNode **bst) {
    Sala *hist[MAX_HIST];   /* Pilha de histórico para opção "voltar" */
    int   prof = 0;
    Sala *atual = raiz;

    printf("\n🕯  A investigação começa no Hall de Entrada...\n");

    while (atual) {
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║  📍 %-42s║\n", atual->nome);
        printf("╠══════════════════════════════════════════════╣\n");

        /* Coleta pista apenas na primeira visita */
        if (strlen(atual->pista) > 0 && !atual->visitada) {
            printf("║  🔍 Pista: %-35s║\n", atual->pista);
            printf("║     Suspeito vinculado: %-21s║\n",
                   encontrarSuspeito(atual->pista));
            *bst = inserirPista(*bst, atual->pista);
            atual->visitada = 1;
        } else if (strlen(atual->pista) == 0) {
            printf("║  💨 Nenhuma pista nesta sala.                ║\n");
        } else {
            printf("║  ✔  Pista já coletada.                       ║\n");
        }
        printf("╚══════════════════════════════════════════════╝\n");

        /* Sala folha: sem saídas */
        if (!atual->esq && !atual->dir) {
            printf("  🚪 Sem mais saídas.\n");
            if (prof == 0) { printf("  Exploração encerrada!\n"); return; }
            printf("  Voltando...\n");
            atual = hist[--prof];
            continue;
        }

        /* Menu de navegação */
        printf("\nCaminhos:\n");
        if (atual->esq) printf("  [e] Esquerda → %s\n", atual->esq->nome);
        if (atual->dir) printf("  [d] Direita  → %s\n", atual->dir->nome);
        if (prof > 0)   printf("  [v] Voltar   → %s\n", hist[prof-1]->nome);
        printf("  [m] Mapa  [p] Pistas coletadas  [s] Sair\n");
        printf("Opção: ");

        char op;
        scanf(" %c", &op);

        if      (op == 'e' && atual->esq) {
            if (prof < MAX_HIST) hist[prof++] = atual;
            atual = atual->esq;
        }
        else if (op == 'd' && atual->dir) {
            if (prof < MAX_HIST) hist[prof++] = atual;
            atual = atual->dir;
        }
        else if (op == 'v' && prof > 0) {
            atual = hist[--prof];
        }
        else if (op == 'm') {
            printf("\n📜 MAPA:\n"); exibirMapa(raiz, 0);
        }
        else if (op == 'p') {
            printf("\n🗂  Pistas coletadas:\n");
            if (!*bst) printf("  Nenhuma ainda.\n");
            else exibirPistas(*bst);
        }
        else if (op == 's') {
            printf("  🚪 Saindo...\n"); return;
        }
        else printf("  ⚠ Opção inválida!\n");
    }
}

/* ================================================
   ACUSAÇÃO FINAL
   ================================================ */

void acusacaoFinal(PistaNode *bst) {
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║         🗂  RELATÓRIO DE INVESTIGAÇÃO        ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  Pistas coletadas (ordem alfabética):        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");

    if (!bst) { printf("  Nenhuma pista coletada!\n\n"); return; }
    exibirPistas(bst);

    /* Lista suspeitos com base nas pistas coletadas */
    char suspeitos[10][50];
    int  total = 0;
    listarSuspeitos(bst, suspeitos, &total);

    printf("\n🕵  Suspeitos identificados pelas pistas:\n");
    for (int i = 0; i < total; i++) {
        int qtd = contarPistasPorSuspeito(bst, suspeitos[i]);
        printf("  [%d] %-20s — %d pista(s)\n", i+1, suspeitos[i], qtd);
    }

    printf("\nQuem você acusa? ");
    char acusado[50];
    scanf(" %49[^\n]", acusado);

    int provas = contarPistasPorSuspeito(bst, acusado);

    printf("\n╔══════════════════════════════════════════════╗\n");
    if (provas >= MIN_PROVAS)
        printf("║  ✅ CULPADO CONFIRMADO! %d prova(s) contra %-6s║\n",
               provas, acusado);
    else
        printf("║  ❌ Provas insuficientes. %d pista(s) para %-6s║\n",
               provas, acusado);
    printf("╚══════════════════════════════════════════════╝\n\n");
}

/* ================================================
   MAIN
   ================================================ */

int main() {
    /* Inicializa tabela hash */
    for (int i = 0; i < TAM_HASH; i++) tabela[i] = NULL;

    /* Popula hash: pista → suspeito */
    inserirNaHash("Faca ensanguentada",  "Cozinheiro");
    inserirNaHash("Pegadas no tapete",   "Mordomo");
    inserirNaHash("Janela aberta",       "Ladrão");
    inserirNaHash("Livro fora do lugar", "Professor");
    inserirNaHash("Terra remexida",      "Jardineiro");
    inserirNaHash("Baú arrombado",       "Ladrão");
    inserirNaHash("Garrafa quebrada",    "Cozinheiro");

    /* Monta árvore de salas */
    Sala *hall    = criarSala("Hall de Entrada",  "");
    Sala *sala    = criarSala("Sala de Estar",    "Pegadas no tapete");
    Sala *cozinha = criarSala("Cozinha",          "Faca ensanguentada");
    Sala *bib     = criarSala("Biblioteca",       "Livro fora do lugar");
    Sala *jardim  = criarSala("Jardim",           "Terra remexida");
    Sala *quarto  = criarSala("Quarto",           "Janela aberta");
    Sala *sotao   = criarSala("Sótão",            "Baú arrombado");
    Sala *adega   = criarSala("Adega",            "Garrafa quebrada");

    hall->esq    = sala;    hall->dir    = cozinha;
    sala->esq    = bib;     sala->dir    = jardim;
    cozinha->esq = quarto;  cozinha->dir = adega;
    quarto->esq  = sotao;

    printf("\n📜 MAPA DA MANSÃO:\n");
    exibirMapa(hall, 0);

    PistaNode *bst = NULL;
    explorar(hall, &bst);
    acusacaoFinal(bst);

    /* Libera toda a memória */
    liberarSalas(hall);
    liberarBST(bst);
    liberarHash();

    return 0;
}
