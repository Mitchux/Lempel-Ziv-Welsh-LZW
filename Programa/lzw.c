#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "biblioteca.h"

//define um tipo INT como um inteiro de 16 bits, ou seja, a quantidade de palavras que o dicionário poderá ter é de 2^16
typedef uint16_t INT;

int procura(HASHMAP dicionario[], int tamanhoDicionario, char *palavra);

int procuraNumero(HASHMAP dicionario[], int tamanhoDicionario, INT numero);

int codifica(char *arqEntrada, char* arqSaida);

int decodifica(char *arqEntrada, char *arqSaida);

/*
    A constante MAX define o tamanho máximo do tamanho do dicionário, como estamos trabalhando com um número de no máximo 16 bits
    o valor do tamanho máximo de palavras do dicionário é de 2^16 = 65.536
*/
const int MAX = 65536;


int main (int argc, char *argv[])
{

    // garante que não sejam passados mais que 6 argumentos pela linha de comando
    if (argc != 6)
    {
        printf("Erro! Passe corretamente os argumentos pela linha de comando\n");
        return 1;
    }
    //A string operação armazena o tipo de operação que irá ser realizado, se ira ser o processo de codificação ou decodificação
    char *operacao = argv[1];


    // define os ponteiros para os arquivos de entrada e saída
    char *arqEntrada = argv[3];
    char *arqSaida = argv[5];


    //lê o 3 e 5 parametro da linha de comando
    char *parametro2 = argv[2];
    char *parametro4 = argv[4];

    //verificação se os parametros foram passados dentro da conformidade exigida 
    if(strcmp(parametro2, "-i")!=0||strcmp(parametro4, "-o")!=0)
    {
        printf("Erro de sintaxe nos parametros passados pela linha de comando\n");
        return 1;
    }


    //verifica se a operação é de encode, se verdadeiro executa o processo de codificação para o arquivo binario
    if(strcmp(operacao, "encode")==0)
    {
        return codifica(arqEntrada, arqSaida);
	}
	else
	{
		//se a operação é igual a decode o método de decodificação de um arquivo irá ser invocado
		if (strcmp(operacao, "decode") == 0)
		{
			return decodifica(arqEntrada, arqSaida);
		}
		else
		{
			//mensagem de erro nos parametros passados pela linha de comando
			printf("Erro de sintaxe nos parametro passados pela linha de comando\n");
			return 1;
		}
	}
}


/*
    Define uma função que realiza a busca de uma palavra dentro de um dicionário.
    Se a palavra estiver contida no dicionário a função irá retornar o indice da palavra no dicionario, e caso
    a palavra não esteja contida no vetor a função irá retornar -1.
*/
int procura(HASHMAP dicionario[], int tamanhoDicionario, char *palavra){
    //laço que percorre todo o dicionário
    for(int i =0; i<tamanhoDicionario; i++){
        //verificação se a chave do dicionário no indice i é igual a palavra procurada e se igual retorna o indíce i
        if(strcmp(dicionario[i].chave, palavra)==0){
            return i;
        }
    }
    //caso a palavra não esteja presente no dicionário a função retorna -1
    return -1;
}

/*
    A função procuraNumero se um numero está contido em um determinado dicionario, caso esse número seja encontrado a função irá
    retornar o indice do dicionario que o número se encontra, e -1 caso o numero procurada não esteja no dicionário
*/
int procuraNumero(HASHMAP dicionario[], int tamanhoDicionario, INT numero){
    int aux = (int) numero;
    //laço que percorre todo o dicionário
    for(int i =0; i<tamanhoDicionario; i++)
    {
        //se o número que está sendo procurado é igual ao indice do dicionário é retornado o indice do dicionario
        if(dicionario[i].indice== aux)
        {
            return i;
        }
    }
    //caso o número não esteja presente no dicionário a função retorna -1
    return -1;
}
/*
    Esta função recebe duas Strings com nomes de arquivos e realiza a codificação de um arquivo de entrada
    para um arquivo de saída
*/

int codifica(char *arqEntrada, char* arqSaida)
{

    //Lendo o arquivo de entrada
    FILE *arquivoEntrada = fopen(arqEntrada, "rb");

    // verificação se a leitura do arquivo de entrada ocorreu com sucesso e se o arquivo não está vazio
    if (arquivoEntrada == NULL)
    {
        printf("Erro! Programa não pôde abrir o arquivo %s.\n", arqEntrada);
        return 2;
    }

    //Abrindo ou criando o arquivo de saída
    FILE *arquivoSaida = fopen(arqSaida, "wb");

    // verificação se a abertura ou criação do arquivo de saída foi efetuado com sucesso
    if (arquivoSaida == NULL)
    {
        fclose(arquivoEntrada);
        printf("Erro! Programa não pôde criar/abrir o arquivo %s.\n", arqSaida);
        return 3;
    }


    //inicializa um dicionário com todos os caracteres contidos na tabela ASCII, e define o tamanho máximo do dicionário para a constante MAX
    HASHMAP dicionario[MAX];

    //define um controle para o tamanho do dicionario
    int tamanhoDicionario = 0;

    //define um controle para o tamanho do dicionario de resultados
    int tamanhoResultado = 0;
    //define um vetor para armazenar os números que representaram o arquivo compactado
    INT resultado[MAX];


    //inicializa o dicionario com todos os 256 caracteres e caracteres especiais da tabela ASCII
    for(int i=0; i<256; i++)
    {
        dicionario[i].indice = i;
        //aloca 2 bytes para armazenar o caractere da tabela ascci e o caractere de fim de linha, afinal o atributo chave da estrutura HASHMAP é uma "string"
        dicionario[i].chave = malloc(2);
        dicionario[i].chave[0] = (char)i;
        dicionario[i].chave[1] = '\0';
        //incrementa o tamanho do dicionario
        tamanhoDicionario++;
    }

    //criacao de uma String temporaria para armazenas os valores lidos
    char *stringTemporaria = "";

    //define uma variavel para ler os caracteres do arquivo original
    char c;
    //define uma String como auxiliar para o processo de codificação
    char *stringTemporaria2;
	printf("Comprimindo arquivo...\n");
    //laço que percorre todo o arquivo de entrada
    while(fread(&c, sizeof(char), 1, arquivoEntrada))
    {
        //define um inteiro auxiliar que armazena o tamanho da stringTemporaria
        int auxiliar = strlen(stringTemporaria);
        //aloca dinamicamente uma String do tamanho da stringTemporaria+2 para armazenar a stringTemporaria mais o caractere adicional e o fim de linha
        stringTemporaria2 = malloc(auxiliar+2);

        //copia a stringTemporaria para stringTemporaria2
        for(int k =0; k<auxiliar; k++)
        {
            stringTemporaria2[k] = stringTemporaria[k];
        }
        //na stringTemporaria2 é concatenado o novo caractere lido à stringTemporaria antes armazenada formando uma nova String
        stringTemporaria2[auxiliar] = c;
        stringTemporaria2[auxiliar+1] = '\0';

        //invoca a função procura realizando a busca da stringTemporaria2 no dicionário
        int aux = procura(dicionario, tamanhoDicionario, stringTemporaria2);

        //Se stringTemporaria2 estiver contida no dicionario a stringTemporaria irá receber o conteúdo da stringTemporaria2
        if(aux!=-1)
        {
            stringTemporaria = stringTemporaria2;
        }
        else
        {
            //Se a stringTemporaria2 não estiver no dicionario irá ser feita a busca pela stringTemporia, e a stringTemporaria2 irá ser adicionada ao dicionario
            aux = procura(dicionario, tamanhoDicionario, stringTemporaria);
            //verifica se ainda é possivel adicionar palavras no dicionario
            if(tamanhoResultado<MAX&&tamanhoDicionario<MAX)
            {
                resultado[tamanhoResultado] = aux;
                tamanhoResultado++;
                dicionario[tamanhoDicionario].chave = stringTemporaria2;
                tamanhoDicionario++;
                free(stringTemporaria);
                stringTemporaria = malloc(2);
                stringTemporaria[0] = c;
                stringTemporaria[1] = '\0';
            }
            else
            {
                printf("Erro! Tamanho máximo do dicionário excedido\n");
                return 7;
            }
        }
    }

    //verifica se a stringTemporaria é diferente de vazio, e caos ela não seja adiciona no vetor de resultado pois trata-se da ultima palavra do arquivo
    if(strcmp(stringTemporaria, "")!=0)
    {
        int aux1 = procura(dicionario, tamanhoDicionario, stringTemporaria);
        //verifica se ainda é possivel adicionar palavras no dicionario de resultado
        if(tamanhoResultado<MAX&&tamanhoDicionario<MAX)
        {
            resultado[tamanhoResultado] = aux1;
            tamanhoResultado++;
        }
        else
        {
            printf("Erro! Tamanho maximo do dicionario excedido\n");
            return 7;
        }
    }

    //define um vetor de INT para armazenar os valores do vetor de resultado em um resultadoFinal com o intuito de poupar acessos ao disco
    INT resultadoFinal[tamanhoResultado];
    for(int i=0; i<tamanhoResultado; i++)
    {
        resultadoFinal[i] = resultado[i];
    }
	printf("Gravando novo arquivo compactado...\n");

    //escreve todo o resultadoFinal em um único acesso ao disco
    fwrite(&resultadoFinal, sizeof(resultadoFinal), 1, arquivoSaida);

    // fecha arquivo de entrada
    fclose(arquivoEntrada);

    // fecha arquivo de saída
    fclose(arquivoSaida);
	
	printf("Sucesso na tarefa de compactacao!\n");
    // successo
    return 0;
}


/*
    Esta função recebe duas Strings com nomes de arquivos e realiza a decodificação de um arquivo de entrada
    para um arquivo de saída
*/
int decodifica(char *arqEntrada, char *arqSaida)
{


    //Lendo o arquivo de entrada
    FILE *arquivoEntrada = fopen(arqEntrada, "rb");

    // verificação se a leitura do arquivo de entrada ocorreu com sucesso e se o arquivo não está vazio
    if (arquivoEntrada == NULL)
    {
        printf("Erro! Programa não pôde abrir o arquivo %s.\n", arqEntrada);
        return 2;
    }

    //Abrindo ou criando o arquivo de saída
    FILE *arquivoSaida = fopen(arqSaida, "wb");
    // verificação se a abertura ou criação do arquivo de saída foi efetuado com sucesso
    if (arquivoSaida == NULL)
    {
        fclose(arquivoEntrada);
        printf("Erro! Programa não pôde criar/abrir o arquivo %s.\n", arqSaida);
        return 3;
    }

    //inicializa um dicionário provisorio com todos os caracteres contidos na tabela ASCII no tamanho da constante MAX
    HASHMAP dicionario[MAX];

    //define um controle para o tamanho do dicionario
    int tamanhoDicionario = 0;

    for(int i=0; i<256; i++)
    {
        dicionario[i].indice = i;
        dicionario[i].chave = malloc(2);
        dicionario[i].chave[0] = (char)i;
        dicionario[i].chave[1] = '\0';
        tamanhoDicionario++;
    }

    //define um INT para ler do arquivo de entrada
    INT numero;
	
	printf("Decodificando arquivo...\n");
	//realiza a leitura dos 2 bytes e armazena na variavel numero
    fread(&numero, sizeof(INT), 1, arquivoEntrada);

    //realiza a criação de uma stringTemporaria inicialmente com duas posições
    char *stringTemporaria = malloc(2);
    stringTemporaria[0] = (char) numero;
    stringTemporaria[1] = '\0';

    //escreve o primeiro caractere no arquivo de saída
    fwrite(&numero, sizeof(char), 1, arquivoSaida);
	
    //laço que percorre todo o arquivo de entrada
    while(fread(&numero, sizeof(INT), 1, arquivoEntrada))
    {	
        //define uma variavel do tipo String chamada entrada
        char *entrada;
        //procura o numero lido no dicionario
        int aux = procuraNumero(dicionario, tamanhoDicionario, numero);
        //se o número retornado for diferente de -1 a variavel entrada vai receber a String armazenada no indice aux do dicionario
        if(aux!=-1)
        {
            entrada = dicionario[aux].chave;
        }
        else
        {
            /*  se o número for igual ao tamanho do dicionario isso significa que o número lido é uma palavra ainda desconhecida pelo dicionario
                e a palavra será inferida aplicando o processo inverso que foi realizado na codificação
            */
            if(numero == tamanhoDicionario)
            {
                entrada = malloc(strlen(stringTemporaria)+2);
                for(int i =0, z = strlen(stringTemporaria); i<z; i++)
                {
                    entrada[i] = stringTemporaria[i];
                }
                entrada[strlen(stringTemporaria)] = stringTemporaria[0];
                entrada[strlen(stringTemporaria)+1] = '\0';
            }
            //se nenhuma das hipóteses acima for correta o arquivo provavelmente foi comprimido de forma errada
            else
            {
                printf("ERRO!! ARQUIVO COMPRIMIDO INCORRETAMENTE\n");
                return 8;
            }
        }
        //escreve a palavra decodificada no arquivo de saída
		for(int i =0, j = strlen(entrada); i<j; i++)
            fwrite(&entrada[i], sizeof(char), 1, arquivoSaida);

        //define uma String auxiliar no valor do tamanho da stringTemporaria+2 para concatenação do novo caractere mais o fim de linha
        char *auxiliar = malloc(strlen(stringTemporaria)+2);

        //copia o valor da stringTemporaria para a String auxiliar
        for(int i=0, j=strlen(stringTemporaria); i<j; i++)
        {
            auxiliar[i] = stringTemporaria[i];
        }
        //adiciona o caractere do indice 0 da entrada na String auxiliar
        auxiliar[strlen(stringTemporaria)] = entrada[0];
        auxiliar[strlen(stringTemporaria)+1] = '\0';

        //veridica se há ainda espaço no dicionario e armazena a nova String formada
        if(tamanhoDicionario<MAX)
        {
            dicionario[tamanhoDicionario].indice = tamanhoDicionario;
            dicionario[tamanhoDicionario].chave = auxiliar;
            tamanhoDicionario++;
            stringTemporaria = entrada;
        }
        else
        {
            printf("Erro! Tamanho máximo do dicionário excedido\n");
            return 7;
        }
    }
	
	printf("Sucesso na tarefa de descompactacao!\n");
    // fecha arquivo de entrada
    fclose(arquivoEntrada);

    // fecha arquivo de saída
    fclose(arquivoSaida);

    //sucesso
    return 0;
}
