/*
Projeto de análise de sentimentos utilizando manipulação de strings

Funcionalidades:
1. Analisa sentimento geral de todo o texto (Questão 1)
2. Analisa sentimento de parágrafos específicos (Questão 2)
3. Analisa sentimento da sentença mais longa (Questão 3)
4. Analisa sentimento da sentença mais curta (Questão 4)

Regras de polaridade:
- Positiva: Diferença positiva > 3 OU apenas palavras positivas
- Negativa: Diferença negativa > 3 OU apenas palavras negativas
- Neutra: Diferença entre -3 e 3 (inclusive)
*/

/*
Para executar
gcc trab4.c -o trab4.exe
./trab4.exe
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#define MAX_PARAGRAFO 10      // Máximo de parágrafos
#define MAX_TEXTO 9040        // Tamanho máximo por parágrafo
#define MAX_PALAVRAS 2000     // Máximo de palavras positivas/negativas
#define MAX_SENTENCAS 1400    // Máximo de sentenças

int num_paragrafo = 0, num_posit = 0, num_neg = 0;
int linha3 = 0, linha4 = 0;  // Índices das sentenças mais longa e mais curta
int total_positiva2[10] = {-50, -50, -50, -50, -50, -50, -50, -50, -50, -50}; // Inicializado com -50 (valor sentinela)
int total_negativa2[10] = {-50, -50, -50, -50, -50, -50, -50, -50, -50, -50};
int total_positiva1 = -50, total_negativa1 = -50; // Resultados para Q1
int total_positiva3 = -50, total_negativa3 = -50; // Resultados para Q3
int total_positiva4 = -50, total_negativa4 = -50; // Resultados para Q4
int cont_sentenca = 0;       // Contador de sentenças

char parag[MAX_PARAGRAFO][MAX_TEXTO];  // Armazena parágrafos originais
char sentencas[MAX_SENTENCAS][MAX_TEXTO]; // Armazena sentenças separadas
char positivas[MAX_PALAVRAS];          // Lista de palavras positivas
char negativas[MAX_PALAVRAS];          // Lista de palavras negativas
char cop_positivas[200][21];           // Palavras positivas tokenizadas
char cop_negativas[200][21];           // Palavras negativas tokenizadas

/*
Lê o número de parágrafos e seu conteúdo
*/ 
void ler_paragrafos() {
    printf("Entre com o numero de paragrafos: ");
    scanf("%d", &num_paragrafo);
    getchar(); // Limpa o \n do buffer
    printf("Entre com o texto: \n");
    for (int i = 0; i < num_paragrafo; i++) {
        fgets(parag[i], MAX_TEXTO, stdin);
    }
}

/*
Lê e processa as listas de palavras positivas e negativas
*/
void ler_palavras() {
    printf("Escreva as palavras positivas: ");
    fgets(positivas, MAX_PALAVRAS, stdin);
    printf("Escreva as palavras negativas: ");
    fgets(negativas, MAX_PALAVRAS, stdin);
    
    // Processa palavras positivas
    char *token = strtok(positivas, " \n");
    while (token != NULL) {
        strcpy(cop_positivas[num_posit], token);
        token = strtok(NULL, " \n");
        num_posit++;
    }
    
    // Processa palavras negativas
    token = strtok(negativas, " \n");
    while (token != NULL) {
        strcpy(cop_negativas[num_neg], token);
        token = strtok(NULL, " \n");
        num_neg++;
    }
}

/*
Divide os parágrafos em sentenças individuais
*/
void ler_sentencas() {
    for (int i = 0; i < num_paragrafo; i++) {
        char cop_parag[MAX_TEXTO];
        strcpy(cop_parag, parag[i]);
        char *sentenca = strtok(cop_parag, ".\n");
        while (sentenca != NULL) {
            strcpy(sentencas[cont_sentenca], sentenca);
            sentenca = strtok(NULL, ".\n");
            cont_sentenca++;
        }
    }
}

/*
Encontra a sentença com mais caracteres e a com menos palavras
*/
void palavras_por_sentenca() {
    int maior_senten_palavra[MAX_SENTENCAS] = {0};
    int maior_senten_carac[MAX_SENTENCAS] = {0};
    
    for (int i = 0; i < cont_sentenca; i++) {
        char cop_sentenca[MAX_TEXTO];
        strcpy(cop_sentenca, sentencas[i]);
        
        // Conta palavras na sentença
        char *palavra = strtok(cop_sentenca, " ,;:!?");
        while (palavra != NULL) {
            maior_senten_palavra[i]++;
            palavra = strtok(NULL, " ,;:!?");
        }
        
        // Armazena tamanho em caracteres
        maior_senten_carac[i] = strlen(sentencas[i]);
    }

    // Encontra sentença com mais caracteres (Questão 3)
    int maior_caracteres = 0;
    for (int i = 1; i < cont_sentenca; i++) {
        if (maior_senten_carac[i] > maior_caracteres) {
            maior_caracteres = maior_senten_carac[i];
            linha3 = i;
        }
    }
    
    // Encontra sentença com menos palavras (Questão 4)
    int menor_palavras = maior_senten_palavra[0];
    for (int i = 1; i < cont_sentenca; i++) {
        if (maior_senten_palavra[i] < menor_palavras && maior_senten_palavra[i] > 0) {
            menor_palavras = maior_senten_palavra[i];
            linha4 = i;
        }
    }
}

/*
Conta palavras positivas e negativas em um texto
*/
void conta_palavras(char *texto, int *cont_positiva, int *cont_negativa) {
    *cont_positiva = 0;
    *cont_negativa = 0;
    char texto_cop[MAX_TEXTO];
    strcpy(texto_cop, texto);
    
    char *palavra = strtok(texto_cop, " ,.;!?");
    while (palavra != NULL) {
        // Verifica palavras positivas
        for (int i = 0; i < num_posit; i++) {
            if (strcasecmp(palavra, cop_positivas[i]) == 0) {
                (*cont_positiva)++;
            }
        }
        
        // Verifica palavras negativas
        for (int i = 0; i < num_neg; i++) {
            if (strcasecmp(palavra, cop_negativas[i]) == 0) {
                (*cont_negativa)++;
            }
        }
        palavra = strtok(NULL, " ,.;!?");
    }
}

void questao_1() {
    total_positiva1 = 0, total_negativa1 = 0;
    for (int i = 0; i < cont_sentenca; i++) {
        int cont_positiva, cont_negativa;
        conta_palavras(sentencas[i], &cont_positiva, &cont_negativa);
        total_positiva1 += cont_positiva;
        total_negativa1 += cont_negativa;
    }
}

void questao_2() {
    int p;
    printf("Entre com o paragrafo que deseja analisar: ");
    scanf("%d", &p);
    int cont_positiva, cont_negativa;
    conta_palavras(parag[p], &cont_positiva, &cont_negativa);
    total_positiva2[p] = cont_positiva;
    total_negativa2[p] = cont_negativa;
}

void questao_3() {
    palavras_por_sentenca();
    int cont_positiva, cont_negativa;
    conta_palavras(sentencas[linha3], &cont_positiva, &cont_negativa);
    total_positiva3 = cont_positiva;
    total_negativa3 = cont_negativa;
}

void questao_4() {
    palavras_por_sentenca();
    int cont_positiva, cont_negativa;
    conta_palavras(sentencas[linha4], &cont_positiva, &cont_negativa);
    total_positiva4 = cont_positiva;
    total_negativa4 = cont_negativa;
}

void identif_questoes(int a) {
    printf("-----------------------QUESTOES------------------------\n"
           "1. Analisa sentimento geral de todo o texto\n"
            "2. Analisa sentimento de paragrafos especificos\n"
            "3. Analisa sentimento da sentenca mais longa\n"
            "4. Analisa sentimento da sentenca mais curta\n");
    printf("-------------------------------------------------------\n");
    printf("Digite a questao escolhida: ");
    for (int i = 0; i < a; i++) {
        int questao;
        scanf("%d", &questao);
        
        // Executa a função correspondente à questão
        switch(questao) {
            case 1: questao_1(); break;
            case 2: questao_2(); break;
            case 3: questao_3(); break;
            case 4: questao_4(); break;
            default: printf("Opcao invalida\n");
        }
    }

    // Processa e imprime os resultados
    for (int i = 0; i < a; i++) {
        // Resultados para Questão 1
        if (total_positiva1 != -50 || total_negativa1 != -50) {
            if ((total_positiva1 - total_negativa1) > 3 || (total_positiva1 > 0 && total_negativa1 == 0)) {
                printf("1: %d Palavras positivas, %d Palavras negativas: Polaridade positiva\n", 
                      total_positiva1, total_negativa1);
            } else if ((total_negativa1 - total_positiva1) > 3 || (total_negativa1 > 0 && total_positiva1 == 0)) {
                printf("1: %d Palavras positivas, %d Palavras negativas: Polaridade negativa\n", 
                      total_positiva1, total_negativa1);
            } else {
                printf("1: %d Palavras positivas, %d Palavras negativas: Polaridade neutra\n", 
                      total_positiva1, total_negativa1);
            }
            total_positiva1 = -50;
            total_negativa1 = -50;
        }
        
        // Resultados para Questão 2 (parágrafos específicos)
        for (int p = 0; p < 10; p++) {
            if (total_positiva2[p] != -50 || total_negativa2[p] != -50) {
                if ((total_positiva2[p] - total_negativa2[p]) > 3 || (total_positiva2[p] > 0 && total_negativa2[p] == 0)) {
                    printf("2: %d Palavras positivas, %d Palavras negativas: Polaridade positiva\n", 
                          total_positiva2[p], total_negativa2[p]);
                } else if ((total_negativa2[p] - total_positiva2[p]) > 3 || (total_negativa2[p] > 0 && total_positiva2[p] == 0)) {
                    printf("2: %d Palavras positivas, %d Palavras negativas: Polaridade negativa\n", 
                          total_positiva2[p], total_negativa2[p]);
                } else {
                    printf("2: %d Palavras positivas, %d Palavras negativas: Polaridade neutra\n", 
                          total_positiva2[p], total_negativa2[p]);
                }
                total_positiva2[p] = -50;
                total_negativa2[p] = -50;
            }
        }
        
        // Resultados para Questão 3 (sentença mais longa)
        if (total_positiva3 != -50 || total_negativa3 != -50) {
            if ((total_positiva3 - total_negativa3) > 3 || (total_positiva3 > 0 && total_negativa3 == 0)) {
                printf("3: %d Palavras positivas, %d Palavras negativas: Polaridade positiva\n", 
                      total_positiva3, total_negativa3);
            } else if ((total_negativa3 - total_positiva3) > 3 || (total_negativa3 > 0 && total_positiva3 == 0)) {
                printf("3: %d Palavras positivas, %d Palavras negativas: Polaridade negativa\n", 
                      total_positiva3, total_negativa3);
            } else {
                printf("3: %d Palavras positivas, %d Palavras negativas: Polaridade neutra\n", 
                      total_positiva3, total_negativa3);
            }
            total_positiva3 = -50;
            total_negativa3 = -50;
        }
        
        // Resultados para Questão 4 (sentença mais curta)
        if (total_positiva4 != -50 || total_negativa4 != -50) {
            if ((total_positiva4 - total_negativa4) > 3 || (total_positiva4 > 0 && total_negativa4 == 0)) {
                printf("4: %d Palavras positivas, %d Palavras negativas: Polaridade positiva\n", 
                      total_positiva4, total_negativa4);
            } else if ((total_negativa4 - total_positiva4) > 3 || (total_negativa4 > 0 && total_positiva4 == 0)) {
                printf("4: %d Palavras positivas, %d Palavras negativas: Polaridade negativa\n", 
                      total_positiva4, total_negativa4);
            } else {
                printf("4: %d Palavras positivas, %d Palavras negativas: Polaridade neutra\n", 
                      total_positiva4, total_negativa4);
            }
            total_positiva4 = -50;
            total_negativa4 = -50;
        }
    }
}

int main() {
    int quant_questoes;
    
    ler_paragrafos();       // 1. Lê os parágrafos de entrada
    ler_palavras();         // 2. Lê as palavras positivas/negativas
    ler_sentencas();        // 3. Separa em sentenças
    
    // Processa as questões
    printf("Quantas questoes serao respondidas?: ");
    scanf("%d", &quant_questoes);
    printf("\n");
    identif_questoes(quant_questoes);
    
    return 0;
}