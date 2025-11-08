#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define QUEUE_CAP 5   
#define STACK_CAP 3   

typedef struct { char nome; int id; } Piece;

typedef struct {
    Piece a[QUEUE_CAP];
    int front, size;
} Queue;


typedef struct {
    Piece a[STACK_CAP];
    int top;
} Stack;

static int NEXT_ID = 0;


static Piece gen_piece(void) {
    const char types[] = {'I','O','T','L'};
    Piece p = { types[rand() % 4], NEXT_ID++ };
    return p;
}

static void q_init(Queue *q) { q->front = 0; q->size = 0; }
static int q_empty(const Queue *q) { return q->size == 0; }
static int q_full(const Queue *q)  { return q->size == QUEUE_CAP; }
static int q_enqueue(Queue *q, Piece p) {
    if (q_full(q)) return 0;
    int idx = (q->front + q->size) % QUEUE_CAP;
    q->a[idx] = p; q->size++; return 1;
}
static int q_dequeue(Queue *q, Piece *out) {
    if (q_empty(q)) return 0;
    *out = q->a[q->front];
    q->front = (q->front + 1) % QUEUE_CAP;
    q->size--; return 1;
}

static void s_init(Stack *s) { s->top = -1; }
static int s_empty(const Stack *s) { return s->top == -1; }
static int s_full(const Stack *s)  { return s->top == STACK_CAP-1; }
static int s_push(Stack *s, Piece p) {
    if (s_full(s)) return 0;
    s->a[++s->top] = p; return 1;
}
static int s_pop(Stack *s, Piece *out) {
    if (s_empty(s)) return 0;
    *out = s->a[s->top--]; return 1;
}

static void show(const Queue *q, const Stack *s) {
    printf("\nEstado atual:\n\nFila de peças\t");
    if (q_empty(q)) { printf("(vazia)\n"); }
    else {
        int idx = q->front;
        for (int i=0;i<q->size;i++){
            printf("[%c %d]%s", q->a[idx].nome, q->a[idx].id, i==q->size-1 ? "\n" : " ");
            idx = (idx+1)%QUEUE_CAP;
        }
    }
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (s_empty(s)) printf("(vazia)\n");
    else {
        for (int i=s->top;i>=0;i--) {
            printf("[%c %d]%s", s->a[i].nome, s->a[i].id, i==0 ? "\n" : " ");
        }
    }
    printf("Tabela: Visualização atual da fila de peças e da pilha de reserva.\nCuradoria de TI.\n\n");
}

static int read_opt(void) {
    char buf[64];
    if (!fgets(buf, sizeof buf, stdin)) return INT_MIN;
    char *end; long v = strtol(buf, &end, 10);
    if (end == buf) return INT_MIN;
    return (int)v;
}

int main(void){
    srand((unsigned)time(NULL));
    Queue q; Stack s; Piece p;
    q_init(&q); s_init(&s);

    for (int i=0;i<QUEUE_CAP;i++) q_enqueue(&q, gen_piece());

    for (;;) {
        show(&q, &s);
        printf("Opções:\n1\tJogar peça\n2\tReservar peça\n3\tUsar peça reservada\n0\tSair\nOpção: ");
        int opt = read_opt();
        if (opt==INT_MIN) { printf("Entrada inválida.\n"); continue; }

        if (opt==0) { puts("Encerrando..."); break; }

        if (opt==1) {
            if (q_dequeue(&q, &p)) {
                printf("Peça jogada: [%c %d]\n", p.nome, p.id);
                q_enqueue(&q, gen_piece()); /* repõe para manter fila cheia */
            } else printf("Fila vazia, nada a jogar.\n");
        }
        else if (opt==2) {
            if (s_full(&s)) { printf("Pilha cheia. Não é possível reservar.\n"); }
            else if (q_dequeue(&q, &p)) {
                s_push(&s, p);
                printf("Peça reservada: [%c %d]\n", p.nome, p.id);
                q_enqueue(&q, gen_piece()); /* repõe */
            } else printf("Fila vazia, nada para reservar.\n");
        }
        else if (opt==3) {
            if (s_pop(&s, &p)) {
                printf("Usou peça reservada: [%c %d]\n", p.nome, p.id);
            } else printf("Pilha vazia, nenhuma peça reservada.\n");
        }
        else printf("Opção desconhecida.\n");
    }

    return 0;
}