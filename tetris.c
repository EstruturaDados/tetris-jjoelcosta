/*
  tetris_stack.c

  Simulação da fila de peças futuras do "Tetris Stack".
  - Fila circular com capacidade fixa.
  - Ações: Jogar peça (dequeue), Inserir nova peça (enqueue), Sair.
  - Peças são geradas automaticamente pela função gerarPeca().
  - Código comentado em português, com nomes de variáveis descritivos.

  Como compilar:
    gcc -o tetris_stack tetris_stack.c

  Como executar:
    ./tetris_stack
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE_FILA 5

/* Representação de uma peça */
typedef struct {
    char nome;   /* 'I', 'O', 'T', 'L' */
    int id;      /* identificador único sequencial */
} Peca;

/* Estrutura da fila circular */
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;   /* índice do elemento da frente */
    int tamanho;  /* número de elementos atualmente na fila */
} Fila;

/* Função que gera uma nova peça automaticamente.
   Retorna uma Peca com nome aleatório entre 'I', 'O', 'T', 'L' e id incremental. */
Peca gerarPeca(void) {
    static int proximoId = 0; /* id único e sequencial */
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % (sizeof(tipos) / sizeof(tipos[0]))];
    nova.id = proximoId++;
    return nova;
}

/* Inicializa a fila vazia */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tamanho = 0;
}

/* Verifica se a fila está cheia */
int filaCheia(const Fila *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

/* Verifica se a fila está vazia */
int filaVazia(const Fila *f) {
    return f->tamanho == 0;
}

/* Enfileira (enqueue) uma peça ao final da fila, se houver espaço.
   Retorna 1 se sucesso, 0 se falha por estar cheia. */
int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int indiceInsercao = (f->frente + f->tamanho) % CAPACIDADE_FILA;
    f->itens[indiceInsercao] = p;
    f->tamanho++;
    return 1;
}

/* Desenfileira (dequeue) a peça da frente da fila.
   Retorna 1 se sucesso (preenche *p), 0 se falha por estar vazia. */
int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;
    *p = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

/* Exibe o estado atual da fila no formato desejado */
void exibirFila(const Fila *f) {
    printf("\nConfira a seguir seu estado:\n\n");
    printf("Fila de peças\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
    } else {
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->frente + i) % CAPACIDADE_FILA;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
        printf("\n");
    }
    printf("Tabela: Visualização a fila de peças.\n");
    printf("Curadoria de TI.\n\n");
}

/* Exibe o menu de ações */
void exibirMenu(void) {
    printf("Opções de ação:\n\n");
    printf("Código\tAção\n");
    printf("1\tJogar peça (dequeue)\n");
    printf("2\tInserir nova peça (enqueue)\n");
    printf("0\tSair\n");
    printf("Tabela: Comandos disponíveis para manipular a fila de peças.\n");
    printf("Curadoria de TI.\n\n");
    printf("Escolha uma opção: ");
}

int main(void) {
    Fila fila;
    inicializarFila(&fila);

    /* inicializa gerador de números aleatórios para gerar tipos de peças */
    srand((unsigned int) time(NULL));

    /* Preencher a fila inicial com CAPACIDADE_FILA peças */
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        Peca p = gerarPeca();
        enfileirar(&fila, p); /* aqui sempre terá espaço até completar */
    }

    int opcao = -1;
    while (1) {
        exibirFila(&fila);
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            /* Se leitura falhar (por exemplo, entrada não numérica), limpar stdin e continuar */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Entrada inválida. Digite um número correspondente às opções.\n\n");
            continue;
        }

        if (opcao == 0) {
            printf("Saindo... Obrigado por testar a fila de peças do Tetris Stack.\n");
            break;
        } else if (opcao == 1) {
            /* Jogar peça: remover da frente */
            Peca jogada;
            if (desenfileirar(&fila, &jogada)) {
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
            } else {
                printf("A fila está vazia. Não há peça para jogar.\n");
            }
        } else if (opcao == 2) {
            /* Inserir nova peça: gerar automaticamente e tentar enfileirar */
            if (filaCheia(&fila)) {
                printf("A fila está cheia. Não é possível inserir nova peça.\n");
            } else {
                Peca nova = gerarPeca();
                if (enfileirar(&fila, nova)) {
                    printf("Nova peça inserida: [%c %d]\n", nova.nome, nova.id);
                } else {
                    /* Caso improvável, mas tratado */
                    printf("Falha ao enfileirar a nova peça.\n");
                }
            }
        } else {
            printf("Opção desconhecida. Tente novamente.\n");
        }

        /* pequena separação visual antes de reexibir */
        printf("\n---------------------------------------------\n");
    }

    return 0;
}