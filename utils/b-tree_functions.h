#include "bt.h"

/* STRUCTS */
typedef struct REGISTRO
{
    char codCliente[12];
    char codVeiculo[8];
    char nomeCliente[50];
    char nomeVeiculos[50];
    char dias[4];
} REGISTRO;

typedef struct
{
    char codCliente[12];
    char codVeiculo[8];
} BUSCA;

/* PROTOTIPAÇÂO*/
void imprime(short rrn, FILE *pontResult);
void imprimeDadosPagina(BTPAGE *p_page, int * pos, FILE *pontResult);

/*Função: insere*/
int insere(short rrn, INDEX chave, short * promoRrnFilho, INDEX * promoChave, int * repetida){
    BTPAGE page, newPage; // page: página atual; newpage: página criada caso ocorra um split;
    int encontrado, promovido;
    short pos, rrnPromovido;
    INDEX chavePromovido;

    if (rrn == NIL){
        *promoChave = chave;
        *promoRrnFilho = NIL;
        return YES;
    }

    btread(rrn, &page);
    encontrado = search_node(chave, &page, &pos);
    if (encontrado){
        printf("A chave já foi cadastrada: %s \n", chave.key);
        *repetida = 1;
        return NO;
    }

    promovido = insere(page.child[pos], chave, &rrnPromovido, &chavePromovido, repetida);
    if (!promovido)
        return NO;

    if (page.keyCount < MAXKEYS){
        ins_in_page(chavePromovido, rrnPromovido, &page);
        btwrite(rrn, &page);
        return NO;
    }
    else{
        split(chavePromovido, rrnPromovido, &page, promoChave, promoRrnFilho, &newPage);
        btwrite(rrn, &page);
        btwrite(*promoRrnFilho, &newPage);
        return YES;
    }
}

void imprimeDadosPagina(BTPAGE * page, int * pos, FILE * pontResult){
    int i = 0;
    REGISTRO tempRegistro;

    (*pos) = i;
    imprime(page->child[(*pos)], pontResult);
    for (i = 0; i < page->keyCount; i++){
        if (strcmp(page->keys[i].key, "@@@@@@@@@@@@@@@@@@") != 0){
            printf("Chave: %s\n", page->keys[i].key);
            fseek(pontResult, page->keys[i].offset, SEEK_SET);
            fread(&tempRegistro, sizeof(REGISTRO), 1, pontResult);
            printf("Cod Cliente:%s\nCod Veiculo:%s\nNome Cliente:%s\nNome Veiculo:%s\nDias:%s\n", tempRegistro.codCliente, tempRegistro.codVeiculo, tempRegistro.nomeCliente, tempRegistro.nomeVeiculos, tempRegistro.dias);
        }
    }
    (*pos) = i;
}

void imprime(short rrn, FILE * pontResult){
    BTPAGE page;
    int pos;

    if (rrn = NIL)
        return ;

    btread(rrn, &page);
    imprimeDadosPagina(&page, &pos, pontResult);
    imprime(page.child[pos], pontResult);
}