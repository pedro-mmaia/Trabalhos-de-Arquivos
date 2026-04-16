#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
typedef struct _Endereco Endereco;
//Integrantes:
//Geovanne Gomes de Souza
//Pedro Henrique Maia da Silva

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};
struct _Indice{
    char cep[8];
    long posicao;
};typedef struct _Indice Indice;


int compara(const void *i1, const void *i2)
{
	return strncmp(((Indice*)i1)->cep,((Indice*)i2)->cep,8);
}


int main(int argc, char **argv)
{
    FILE *f,*arq1;
    Endereco e;
    long qtd,tamanhoBytes,numeroRegistros;
    f=fopen("cep.dat","rb");
    fseek(f,0,SEEK_END);
    tamanhoBytes=ftell(f);
    numeroRegistros=tamanhoBytes/sizeof(Endereco);
    Indice *i = (Indice*) malloc(numeroRegistros * sizeof(Indice));
    rewind(f);
    qtd=fread(&e,sizeof(Endereco),1,f);
    int j=0;
    long posicao=0;
    while(qtd>0){
        strncpy(i[j].cep,e.cep,8);
        i[j].posicao=posicao;
        j++;
        posicao+=sizeof(Endereco);
        qtd=fread(&e,sizeof(Endereco),1,f);
    }
    qsort(i,numeroRegistros,sizeof(Indice),compara);
    arq1=fopen("arquivo1.dat","w+b");
    fwrite(i,sizeof(Indice),numeroRegistros,arq1);
    rewind(arq1);

    //BuscaBinaria
    
    Indice ind;
    long indice=-1;
    long inicio = 0;
    long fim = numeroRegistros - 1;
    long meio = (inicio + fim) / 2;
    fseek(arq1,meio*sizeof(Indice),SEEK_SET);

    while(inicio<=fim){
        qtd=fread(&ind,sizeof(Indice),1,arq1);
        if(strncmp(argv[1],ind.cep,8)==0){
            indice=ind.posicao;
            break;
        }
        else if(strncmp(argv[1],ind.cep,8)<0){// ir para a esquerda
            fim = meio - 1;
            meio = (inicio + fim) / 2;
            fseek(arq1,meio*sizeof(Indice),SEEK_SET);
        }
        else{
            inicio = meio + 1;
            meio = (inicio + fim) / 2;
            fseek(arq1,meio*sizeof(Indice),SEEK_SET);
        }
    }
    if(indice==-1){
        printf("Cep nao foi encontrado");
        return 0;
    }
    fseek(f,indice,SEEK_SET);
    qtd=fread(&e,sizeof(Endereco),1,f);
    printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
    fclose(f);
    fclose(arq1);
    free(i);
    return 0;
}
