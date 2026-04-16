#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define K 2
typedef struct _Endereco Endereco;
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
int compara(const void *e1, const void *e2)
{
    return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}
int main(int argc, char **argv)
{
    FILE *f;
    long qtd;
    f = fopen("cep.dat", "rb");
    fseek(f, 0, SEEK_END);
    long tamanhoBytes = ftell(f);
    long numeroRegistros = tamanhoBytes / sizeof(Endereco);
    rewind(f);
    long base = numeroRegistros / K;
    long maior_bloco = base + (numeroRegistros % K);
    Endereco *bloco = malloc(maior_bloco * sizeof(Endereco));
    for (int i = 0; i < K; i++)
    {
        long qtd_bloco = (i == K - 1) ? (numeroRegistros - base * (K - 1)) : base;
        qtd = fread(bloco, sizeof(Endereco), qtd_bloco, f);
        qsort(bloco, qtd, sizeof(Endereco), compara);

        char nome[20];
        sprintf(nome, "bloco%d.dat", i);
        FILE *arqBloco = fopen(nome, "wb");
        fwrite(bloco, sizeof(Endereco), qtd, arqBloco);
        fclose(arqBloco);
    }
    int blocos_restantes = K;
    int rodada = 0;

    while (blocos_restantes > 1)
    {
        for (int b = 0; b < blocos_restantes; b += 2)
        {
            char nome1[30], nome2[30], nome_saida[30];

            if (rodada == 0)
            {
                // primeira rodada lê os blocos originais
                sprintf(nome1, "bloco%d.dat", b);
                sprintf(nome2, "bloco%d.dat", b + 1);
            }
            else
            {
                // rodadas seguintes lêem os temporários da rodada anterior
                sprintf(nome1, "temp_%d_%d.dat", rodada - 1, b / 2);
                sprintf(nome2, "temp_%d_%d.dat", rodada - 1, b / 2 + 1);
            }
            sprintf(nome_saida, "temp_%d_%d.dat", rodada, b / 2);

            FILE *arq1 = fopen(nome1, "rb");
            FILE *arq2 = fopen(nome2, "rb");
            FILE *saida = fopen(nome_saida, "wb");
            Endereco e1, e2, resultado;
            long qtd_arq1, qtd_arq2;
            qtd_arq1 = fread(&e1, sizeof(Endereco), 1, arq1);
            qtd_arq2 = fread(&e2, sizeof(Endereco), 1, arq2);
            while (qtd_arq1 > 0 || qtd_arq2 > 0)
            {
                if (qtd_arq1 > 0 && qtd_arq2 > 0 && strncmp(e1.cep, e2.cep, 8) == 0)
                {
                    strcpy(resultado.bairro, e1.bairro);
                    strcpy(resultado.cidade, e1.cidade);
                    strcpy(resultado.cep, e1.cep);
                    strcpy(resultado.lixo, e1.lixo);
                    strcpy(resultado.logradouro, e1.logradouro);
                    strcpy(resultado.sigla, e1.sigla);
                    strcpy(resultado.uf, e1.uf);
                    qtd_arq1 = fread(&e1, sizeof(Endereco), 1, arq1);
                    qtd_arq2 = fread(&e2, sizeof(Endereco), 1, arq2);
                }
                else if (qtd_arq1 > 0 && (qtd_arq2 == 0 || strncmp(e1.cep, e2.cep, 8) < 0))
                {
                    strcpy(resultado.bairro, e1.bairro);
                    strcpy(resultado.cidade, e1.cidade);
                    strcpy(resultado.cep, e1.cep);
                    strcpy(resultado.lixo, e1.lixo);
                    strcpy(resultado.logradouro, e1.logradouro);
                    strcpy(resultado.sigla, e1.sigla);
                    strcpy(resultado.uf, e1.uf);
                    qtd_arq1 = fread(&e1, sizeof(Endereco), 1, arq1);
                }
                else
                {
                    strcpy(resultado.bairro, e2.bairro);
                    strcpy(resultado.cidade, e2.cidade);
                    strcpy(resultado.cep, e2.cep);
                    strcpy(resultado.lixo, e2.lixo);
                    strcpy(resultado.logradouro, e2.logradouro);
                    strcpy(resultado.sigla, e2.sigla);
                    strcpy(resultado.uf, e2.uf);
                    qtd_arq2 = fread(&e2, sizeof(Endereco), 1, arq2);
                }
                fwrite(&resultado, sizeof(Endereco), 1, saida);
            }
            fclose(arq1);
            fclose(arq2);
            fclose(saida);
        }
        blocos_restantes /= 2;
        rodada++;
    }
    return 0;
}
