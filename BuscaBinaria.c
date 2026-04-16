///Integrantes:
//Geovanne Gomes de Souza
//Pedro Henrique Maia da Silva

#include <stdio.h>
#include <string.h>

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

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;
    long qt;
    int c;

    if (argc != 2)
    {
        fprintf(stderr, "USO: %s [CEP]", argv[0]);
        return 1;
    }

    c = 0;
    printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
    f = fopen("cep_ordenado.dat", "rb");
    fseek(f, 0, SEEK_END);
    /* o fseek move o cursor q esta dentro do arquivo q queremos(nesse caso o f)
       seus parametros:
       f - O arquivo q vc quer mudar o cursor de lugar
       0 - É o deslocamento em bytes a partir da origem.
           Pode ser positivo (andar para frente) ou negativo (andar para trás).
       SEEK_END - É a **origem** do deslocamento. Tem 3 opções:
                   | Constante | Valor | Significa |
                   |---|---|---|
                   | `SEEK_SET` | 0 | A partir do **início** do arquivo |
                   | `SEEK_CUR` | 1 | A partir da **posição atual** |
                   | `SEEK_END` | 2 | A partir do **fim** do arquivo |
       */

    long tamanhoBytes = ftell(f);                            // ele diz quantos bytes tem o arquivo f
    long tamanhoRegistros = tamanhoBytes / sizeof(Endereco); // quantos registros tem o file
    long inicio = 0;
    long fim = tamanhoRegistros - 1;
    long meio = (inicio + fim) / 2;

    fseek(f, meio * sizeof(Endereco), SEEK_SET);
    /*   esta setando o cursor do arquivo para o meio,
         para isso ele usa meio*sizeof(endereco), q diz exatamente quantos bytes ele deve andar ate chegar no
         meio do arquivo
         */
    while (inicio <= fim)
    {
        c++;
        qt = fread(&e, sizeof(Endereco), 1, f);
        /*  o fread vai ler 1 registro a partir do meio do arquivo ja que botamos o cursor do arquivo
            no meio com o fseek
        */

        //  oq o strncmp retorna em cada caso:
        //  argv[1] < e.cep  => strcmp(argv[1],e.cep) < 0
        //  argv[1] > e.cep  => strcmp(argv[1],e.cep) > 0
        //  argv[1] == e.cep  => strcmp(argv[1],e.cep) == 0
        //  pode usar o strstr
        if (strncmp(argv[1], e.cep, 8) == 0) // 8 = numeros de caracters de um cep
        {
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
            break;
        }
        else if (strncmp(argv[1], e.cep, 8) < 0) // ir para a esquerda
        {
            fim = meio - 1;
            meio = (inicio + fim) / 2;
            fseek(f, meio * sizeof(Endereco), SEEK_SET);
        }
        else
        {
            inicio = meio + 1;
            meio = (inicio + fim) / 2;
            fseek(f, meio * sizeof(Endereco), SEEK_SET);
        }
    }
    printf("Total Lido: %d\n", c);
    fclose(f);
}
