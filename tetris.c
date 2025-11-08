#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define QUEUE_CAP 5
#define STACK_CAP 3

typedef struct { char nome; int id; } Peca;
typedef struct { Peca v[QUEUE_CAP]; int front, size; } Fila;
typedef struct { Peca v[STACK_CAP]; int top; } Pilha;

static int NEXT_ID=0;
static Peca gerarPeca(void){ const char t[]={'I','O','T','L'}; return (Peca){ t[rand()%4], NEXT_ID++ }; }

/* fila */
static void fila_init(Fila*q){ q->front=0; q->size=0; }
static int  fila_enq(Fila*q, Peca p){ if(q->size==QUEUE_CAP) return 0; q->v[(q->front+q->size)%QUEUE_CAP]=p; q->size++; return 1; }
static int  fila_deq(Fila*q, Peca*out){ if(!q->size) return 0; *out=q->v[q->front]; q->front=(q->front+1)%QUEUE_CAP; q->size--; return 1; }

/* pilha */
static void pilha_init(Pilha*s){ s->top=-1; }
static int  pilha_push(Pilha*s, Peca p){ if(s->top==STACK_CAP-1) return 0; s->v[++s->top]=p; return 1; }
static int  pilha_pop(Pilha*s, Peca*out){ if(s->top<0) return 0; *out=s->v[s->top--]; return 1; }

/* IO */
static int lerOpt(void){ char buf[64]; if(!fgets(buf,sizeof buf,stdin)) return INT_MIN; char *e; long v=strtol(buf,&e,10); return e==buf?INT_MIN:(int)v; }
static void mostrar(const Fila*q, const Pilha*s){
    printf("\n=== Estado Atual ===\n\nFila de Peças\t");
    if(!q->size) puts("(vazia)");
    else { for(int i=0;i<q->size;i++){ int idx=(q->front+i)%QUEUE_CAP; Peca p=q->v[idx]; printf("[%c %d]%s",p.nome,p.id,i==q->size-1? "\n":" "); } }
    printf("Pilha de Reserva (Topo -> Base): ");
    if(s->top<0) puts("(vazia)");
    else { for(int i=s->top;i>=0;i--) printf("[%c %d]%s",s->v[i].nome,s->v[i].id,i? " ":"\n"); }
    puts("Tabela: Visualização atual da fila de peças e da pilha de reserva.\nCuradoria de TI.\n");
}

/* trocas */
static int trocar1(Fila*q, Pilha*s){ if(!q->size||s->top<0) return 0; int idx=q->front; Peca t=q->v[idx]; q->v[idx]=s->v[s->top]; s->v[s->top]=t; return 1; }
static int trocar3(Fila*q, Pilha*s){
    if(q->size<3||s->top+1<3) return 0;
    Peca f0=q->v[q->front], f1=q->v[(q->front+1)%QUEUE_CAP], f2=q->v[(q->front+2)%QUEUE_CAP];
    Peca s0=s->v[s->top], s1=s->v[s->top-1], s2=s->v[s->top-2];
    q->v[q->front]=s0; q->v[(q->front+1)%QUEUE_CAP]=s1; q->v[(q->front+2)%QUEUE_CAP]=s2;
    s->v[s->top]=f2; s->v[s->top-1]=f1; s->v[s->top-2]=f0;
    return 1;
}

int main(void){
    srand((unsigned)time(NULL));
    Fila fila; Pilha pilha; fila_init(&fila); pilha_init(&pilha);
    for(int i=0;i<QUEUE_CAP;i++) fila_enq(&fila, gerarPeca());

    for(;;){
        mostrar(&fila,&pilha);
        printf("Opções:\n1 Jogar peça\n2 Enviar peça da fila para a pilha\n3 Usar peça reservada\n4 Trocar frente <-> topo\n5 Trocar 3 primeiros da fila com 3 da pilha\n0 Sair\nOpção: ");
        int opt=lerOpt(); if(opt==INT_MIN){ puts("Entrada inválida."); continue; }
        if(opt==0){ puts("Encerrando..."); break; }

        if(opt==1){
            Peca rem; if(fila_deq(&fila,&rem)){ printf("Ação: jogou [%c %d]\n",rem.nome,rem.id); fila_enq(&fila,gerarPeca()); }
            else puts("Fila vazia.");
        } else if(opt==2){
            if(pilha.top==STACK_CAP-1) puts("Pilha cheia. Não é possível reservar.");
            else { Peca rem; if(fila_deq(&fila,&rem)){ if(!pilha_push(&pilha,rem)){ puts("Erro ao reservar, desfazendo."); fila_enq(&fila,rem); } else { printf("Ação: reservada [%c %d]\n",rem.nome,rem.id); fila_enq(&fila,gerarPeca()); } } else puts("Fila vazia."); }
        } else if(opt==3){
            Peca used; if(pilha_pop(&pilha,&used)) printf("Ação: usada reservada [%c %d]\n",used.nome,used.id); else puts("Pilha vazia.");
        } else if(opt==4){
            puts(trocar1(&fila,&pilha)? "Ação: troca realizada entre frente e topo." : "Troca falhou: fila ou pilha vazia.");
        } else if(opt==5){
            puts(trocar3(&fila,&pilha)? "Ação: troca dos 3 primeiros realizada." : "Troca falhou: é necessário ter >=3 em ambas estruturas.");
        } else puts("Opção desconhecida.");
    }
    return 0;
}
