#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mergeQuick.h"

/*
  Luis Ricardo Albano Santos - 2021031844
  gcc -pedantic-errors -Wall main.c -o main.exe && ./main.exe
*/

int geraAleatorios(char* nomeArquivo, int qtd) {

    srand(time(NULL));

    FILE* arquivo = fopen(nomeArquivo, "w+");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    int numero;
    for (int i = 0; i < qtd; i++) {
        numero = rand() % 1000;
        if (!fprintf(arquivo, "%d\n", numero)) {
            printf("Erro ao escrever no arquivo.\n");
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}

//Esta função lê 'qtd' dados do arquivo 'nomeArquivo' e os insere em um vetor de inteiros
//A função retorna NULL caso haja algum erro no processo.
int* leArquivo(char* nomeArquivo, int qtd) {
    int* vetor = (int*)malloc(qtd * sizeof(int));

    if (vetor == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    FILE* arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(vetor);
        return NULL;
    }

    for (int i = 0; i < qtd; i++) {
        if (fscanf(arquivo, "%d", &vetor[i]) != 1) {
            printf("Erro ao ler o arquivo.\n");
            free(vetor);
            fclose(arquivo);
            return NULL;
        }
    }

    fclose(arquivo);
    return (int*)vetor;
}

void imprimeVet(int* vet, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");

    return;
}

void merge(int* vet, int inicio, int meio, int fim) {
    int marcadorV1 = inicio;
    int marcadorV2 = meio + 1;
    int* aux = (int*)malloc((fim - inicio + 1) * sizeof(int));
    int i = 0;

    if (aux == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    while (marcadorV1 <= meio && marcadorV2 <= fim) {
        if (vet[marcadorV1] < vet[marcadorV2]) {
            aux[i] = vet[marcadorV1];
            marcadorV1++;
        }
        else {
            aux[i] = vet[marcadorV2];
            marcadorV2++;
        }
        i++;
    }

    while (marcadorV1 <= meio) {
        aux[i] = vet[marcadorV1];
        marcadorV1++;
        i++;
    }

    while (marcadorV2 <= fim) {
        aux[i] = vet[marcadorV2];
        marcadorV2++;
        i++;
    }

    for (i = 0; i < fim - inicio + 1; i++) {
        vet[inicio + i] = aux[i];
    }

    free(aux);

    return;
}

void mergeSort(int* vet, int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSort(vet, inicio, meio);
        mergeSort(vet, meio + 1, fim);
        merge(vet, inicio, meio, fim);
    }
}

int particiona(int* vet, int inicio, int fim) {
    int pivo = vet[inicio];
    int pos = inicio;
    int aux;

    for (int i = inicio + 1; i <= fim; i++) {
        if (vet[i] < pivo) {
            pos++;
            if (pos != i) {
                aux = vet[i];
                vet[i] = vet[pos];
                vet[pos] = aux;
            }
        }
    }

    aux = vet[inicio];
    vet[inicio] = vet[pos];
    vet[pos] = aux;

    return pos;
}

void quickSort(int* vet, int inicio, int fim) {
    if (inicio < fim) {
        int pivo = particiona(vet, inicio, fim);
        quickSort(vet, inicio, pivo - 1);
        quickSort(vet, pivo + 1, fim);
    }
}