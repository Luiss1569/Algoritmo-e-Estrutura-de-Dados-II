#include <stdio.h>
#include <stdlib.h>
#include "abp.h"

/*
  Diovana Tavares dos Reis - 2021016991
  Luis Ricardo Albano Santos - 2021031844
*/

struct no {
    int chave;
    no* esq;
    no* dir;
    no* pai;
};

struct arvore {
    no* sentinela;
    int numElementos;
};

abp* criaArvore() {
    abp* arv = (abp*)malloc(sizeof(abp));
    arv->sentinela = (no*)malloc(sizeof(no));
    arv->sentinela->chave = -1000;
    arv->sentinela->esq = NULL;
    arv->sentinela->dir = NULL;
    arv->sentinela->pai = NULL;
    arv->numElementos = 0;
    return arv;
}

//Popula uma árvore binária de pesquisa a partir de números contidos no arquivo
//Retorna -1 caso haja erros durante a inserção e/ou leitura do arquivo
//Retorna 0 caso contrário
int carregaArvore(abp* arv, char* nomeArquivo) {
    FILE* arq;
    int num;

    arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo\n");
        return -1;
    }

    while (fscanf(arq, "%d", &num) != EOF) {
        insereNo(arv, num);
    }

    fclose(arq);
    return 0;
}

//Aloca um novo nó e o insere na árvore
//Retorna 0 caso a inserção aconteça e -1 caso contrário
int insereNo(abp* arv, int chave) {
    no* novoNo = (no*)malloc(sizeof(no));
    no* atual = arv->sentinela;
    no* pai = NULL;

    novoNo->chave = chave;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->pai = NULL;

    while (atual != NULL) {
        pai = atual;
        if (chave < atual->chave) {
            atual = atual->esq;
        }
        else {
            atual = atual->dir;
        }
    }

    if (pai->chave < chave) {
        pai->dir = novoNo;
    }
    else {
        pai->esq = novoNo;
    }

    novoNo->pai = pai;
    arv->numElementos++;
    return 0;
}

//Remove um elemento da árvore
//Retorna 0 caso a deleção aconteça;
//Retorna -1 caso a árvore esteja vazia
//Retorna -2 caso o elemento não esteja presente na árvore
int removeNo(abp* arv, int chave) {
    no* atual = arv->sentinela;
    no* filho = NULL;
    no* aux = NULL;

    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave) {
            atual = atual->esq;
        }
        else {
            atual = atual->dir;
        }
    }

    if (atual == NULL) {
        return -2;
    }

    if (atual->esq != NULL && atual->dir != NULL) {
        aux = atual->dir;
        while (aux->esq != NULL) {
            aux = aux->esq;
        }
        atual->chave = aux->chave;
        atual = aux;
    }

    if (atual->esq != NULL) {
        filho = atual->esq;
    }
    else {
        filho = atual->dir;
    }

    if (filho != NULL) {
        filho->pai = atual->pai;
    }

    if (atual->pai == NULL) {
        arv->sentinela = filho;
    }
    else {
        if (atual == atual->pai->esq) {
            atual->pai->esq = filho;
        }
        else {
            atual->pai->dir = filho;
        }
    }

    free(atual);
    arv->numElementos--;

    return 0;
}

//Percorrimento em pré-ordem
//printf("%d\t", raiz->chave)
void percorre(no* raiz) {
    if (raiz != NULL) {
        printf("%d\t", raiz->chave);
        percorre(raiz->esq);
        percorre(raiz->dir);
    }
}

//Retorna o nó que contém a chave indicada (não trata valores repetidos)
//Retorna NULL caso o elemento não exista na árvore
no* recuperaNo(abp* arv, int chave) {
    no* atual = arv->sentinela;

    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave) {
            atual = atual->esq;
        }
        else {
            atual = atual->dir;
        }
    }

    return atual;
}

//Imprime a chave do nó, as chaves de seus filhos e do seu pai
//Se o nó não tiver filho da esquerda e/ou da direita, imprime NULO
//Se o pai for a sentinela, imprime Sentinela
void imprimeNo(no* atual) {
    if (atual != NULL) {
        printf("Chave : %d\n", atual->chave);
        if (atual->esq != NULL) {
            printf("Filho Esq : %d\n", atual->esq->chave);
        }
        else {
            printf("Filho Esq : NULO\n");
        }
        if (atual->dir != NULL) {
            printf("Filho Dir : %d\n", atual->dir->chave);
        }
        else {
            printf("Filho Dir : NULO\n");
        }
        if (atual->pai != NULL) {
            printf("Pai : %d\n", atual->pai->chave);
        }
        else {
            printf("Pai : Sentinela\n");
        }
    }
}

//Retorna a quantidade de elementos contidos na árvore
int getNumElementos(abp* arv) {
    return arv->numElementos;
}

//Retorna a raiz da árvore (filho direita da sentinela)
no* getRaiz(abp* arv) {
    return arv->sentinela->dir;
}
