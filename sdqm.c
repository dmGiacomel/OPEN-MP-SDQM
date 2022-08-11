#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>
#include <time.h>

int n_threads;

double randomReal(){
    
    double d, low = -10000.0, high = 1000.0;
    
    d = (double) rand() / ((double) RAND_MAX + 1);
    return (low + d * (high - low));
}

double** alocarMatriz(double **M, int linhas, int colunas){

    double **matriz;

    matriz = (double**)malloc(sizeof(double*) * linhas);

    for(int i = 0; i < colunas; i++){
        matriz[i] = (double*)malloc(sizeof(double) * colunas);
    }

    return matriz; 
}


void lerMatriz(double **M, int linhas, int colunas){

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            scanf("%lf", &M[i][j]);
        }
    }

}

void randomizarMatriz(double **M, int linhas, int colunas){

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            M[i][j] = randomReal();
        }
    }

}

void elevarElementosAoQuadrado(double **M, int linhas, int colunas){

    int i, j;

    #pragma omp parallel for shared(M, i) private (j)
    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            M[i][j] = pow(M[i][j], 2);
        }
    }
}


void printMatriz(double **M, int linhas, int colunas){

    printf("\n");
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            printf("%lf ", M[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

double **subtrairMatrizes(double **M1, double **M2, int linhas, int colunas){

    int i, j;
    double **MR = alocarMatriz(MR, linhas, colunas);
    
    #pragma omp parallel for shared(M1, M2, MR, i) private (j)
    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            MR[i][j] = M1[i][j] - M2[i][j];
        }
    }

    return MR;
}


double reduzirMatriz(double **M, int linhas, int colunas){

    double resultado = 0;

    int i, j;
    
    #pragma omp parallel for shared(M, i) private (j) reduction(+:resultado)
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            resultado += M[i][j];
        }
    }

    return resultado;
}

int main (int argc, char **argv){

    srand(time(NULL));
    double inicio, fim;
    double tempo_total = 0.0;
    double **M1, **M2, **MR, resultado_final;
    int linha = atoi(argv[1]), coluna = atoi(argv[2]); 
    n_threads = atoi(argv[3]);

    omp_set_num_threads(n_threads);

    M1 = alocarMatriz(M1, linha, coluna);
    M2 = alocarMatriz(M2, linha, coluna);

    randomizarMatriz(M1, linha, coluna);
    randomizarMatriz(M2, linha, coluna);

    //calculo

    inicio = omp_get_wtime();

    elevarElementosAoQuadrado(M1, linha, coluna);
    elevarElementosAoQuadrado(M2, linha, coluna);
    
    MR = subtrairMatrizes(M1, M2, linha, coluna);

    resultado_final = reduzirMatriz(MR, linha, coluna);

    fim = omp_get_wtime();

    tempo_total = fim - inicio;
    printf("Resultado final: %lf\n", resultado_final);
    printf("Tempo: %f\n", tempo_total);

    return 0;
}