#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dados.h"
#include "resultado.h"
#include "ordenacao.h"

#define true 1
#define false 0

#define RESULTADO_TXT "./resutados/r"
#define SELECTION_SORT "selectionSort"
#define QUICK_SORT "quickSort"
#define MOBILE_SORT "mobileSort"

#define AUDITORIA true

bool computarOrdenacaoMobileSort(Registro* r, int* vet, int tamanho) {

    clock_t inicio, fim;

    inicio = clock();
    mobileSort(r, vet, tamanho);
    fim = clock();

    r->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;


    return true;
}

bool computarOrdenacaoSelectionSort(Registro* r, int* vetor, int tamanho) {

    clock_t inicio, fim;
    inicio = clock();

    selecao(r, vetor, tamanho);

    fim = clock();
    r->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    return true;
}

bool computarOrdenacaoQuickSort(Registro* r, int* vetor, int tamanho) {

    clock_t inicio, fim;
    inicio = clock();

    quickSort(r, vetor, 0, tamanho - 1);

    fim = clock();
    r->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    return true;
}

bool realizarOrdenacao(char tipo, char metodo, int tam, int casos_teste) {

    bool salvarRegistro(Registro);

    Registro r;
    r.tipo = tipo;
    r.qtd = tam;

    for (int j = 0; j < casos_teste; j++) {
        int* dados = lerDados(tipo, tam, j);

        r.comparacoes = 0;
        r.copias = 0;
        r.tempo = 0;
        r.caso = j;

        if (dados == NULL) {
            free(dados);
            return false;
        }

        switch (metodo) {
        case 's':
            r.metodo = SELECTION_SORT;
            computarOrdenacaoSelectionSort(&r, dados, tam);
            break;
        case 'q':
            r.metodo = QUICK_SORT;
            computarOrdenacaoQuickSort(&r, dados, tam);
            break;
        case 'm':
            r.metodo = MOBILE_SORT;
            computarOrdenacaoMobileSort(&r, dados, tam);
            break;
        default:
            free(dados);
            printf("Metodo %c não existe!\n", metodo);
            return false;
        }

        if (AUDITORIA) {
            if (!salvarOrdenacao(r, dados, tam)) {
                free(dados);
                return false;
            }
        }

        free(dados);

        if (!salvarRegistro(r)) {
            printf("Erro ao escrever resultado\n");
            return false;
        }
    }

    return true;
}

bool criarArquivoResultado(char* metodo) {
    char path[40] = "";

    sprintf(path, "%s_%s.csv", RESULTADO_TXT, metodo);

    FILE* arquivo = fopen(path, "w+");
    if (arquivo == NULL)
        return false;

    if (fprintf(arquivo, "tipo,qtd,caso,tempo,comparacoes,copias\n") < 0) {
        printf("Erro ao criar arquivo de resultado\n");
        return false;
    }

    fclose(arquivo);

    if (AUDITORIA) {
        criarArquivoOrdenacao(metodo);
    }

    return true;
}

bool salvarRegistro(Registro r) {
    char path[40] = "";

    sprintf(path, "%s_%s.csv", RESULTADO_TXT, r.metodo);

    FILE* arquivo = fopen(path, "a+");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de resultado %s.\n", path);
        return false;
    }

    if (fprintf(arquivo, "%c,%d,%d,%lf,%ld,%ld\n", r.tipo, r.qtd, r.caso, r.tempo, r.comparacoes, r.copias) < 0) {
        printf("Erro ao salvar registro de resultado %s.\n", path);
        return false;
    }

    fclose(arquivo);

    return true;
}

bool zerarArquivosResultados() {

    if (!criarArquivoResultado(SELECTION_SORT))
        return false;

    if (!criarArquivoResultado(QUICK_SORT))
        return false;

    if (!criarArquivoResultado(MOBILE_SORT))
        return false;

    return true;
}
