/*
Trabalho Laboratorio e Estrutura de Dados. - IFSP
Implementacao de um sisteam de RH de uma empresa utilizando arvores binarias de busca.

programa permite o cadastro, a remoção e a consulta de funcionários por nome e cpf, salva o arquivo de saida e lee arquivo de entrada.

duas árvores: uma ordenada por Nome e uma por CPF. Entretanto, para evitar redundancia, o cadastro feito em uma estrutura separada referenciada pelas arvores



*********** AO INICIAR O EXECUTAVEL: ***************
- PASSAR COMO ARG 1 O ARQUIVO DE LEITURA (ENTRADA) --> arquivo de exemplo/modelo: entrada.txt
- PASSAR COMO ARG 2 O ARQUVIO DE GRAVACAO (SAIDA)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int CPF;
    char Nome[50];
    char Profissao[30];
} Info;

typedef struct t_no
{
    Info *info;
    struct t_no *esq;
    struct t_no *dir;
} No;

//cria um tipo de dado struct Info, retorna o endereco da struct info para ser linkado a um noh
Info* cria_info(char Nome[50], int CPF, char Profissao[30])
{
    Info *newInfo = (Info*)calloc(1, sizeof(Info));
    newInfo->CPF = CPF;
    strcpy(newInfo->Nome, Nome);
    strcpy(newInfo->Profissao, Profissao);

    return newInfo;
}

//FUNCAO CHAMADA POR INSERIR
//cria um tipo de dado struct no, recebe o endereco da raiz e o endereco da struct info a ser linkada. Verifica se o CPF se eh maior ou menor
//caminha para esquerda se menor e para direita se maior, cria o noh e faz a ligacao do noh com a struct info.
int cria_no_cpf(No *&root, Info *&newInfo)
{
    if (root == NULL)
    {
        root = (No*)calloc(1,sizeof(No));
        root->info = newInfo;

        return 0;
    }
    if(root->info->CPF > newInfo->CPF)					//CPF do noh > CPF a ser inserido -> vai pra esquerda
        cria_no_cpf(root->esq, newInfo);
    else												//CPF do noh < CPF a ser inserido -> vai pra direita
        cria_no_cpf(root->dir, newInfo);

    return 0;
}

//FUN��O CHAMADA POR INSERIR
//cria um tipo de dado struct no, recebe o endereco da raiz e o endereco da struct info a ser linkada. Verifica se o Nome eh maior ou menor (ordem alfabetica)
//caminha para esquerda se menor e para direita se maior, cria o noh e faz a ligacao do noh (struct noh) com a struct info.
int cria_no_nome(No *&root, Info *&newInfo)
{
    if (root == NULL)
    {
        root = (No*)calloc(1,sizeof(No));
        root->info = newInfo;

        return 0;
    }
    if(strcmp(newInfo->Nome, root->info->Nome)<0)		//Nome a ser inserido < Nome do noh atual -> vai pra esquerda
        cria_no_nome(root->esq, newInfo);
    else												//Nome a ser inserido > Nome do noh atual -> vai pra esquerda
        cria_no_nome(root->dir, newInfo);

    return 0;
}

//funcao que busca na arvore pelo cpf armazenado em k
// k = cpf procurado.
//Controle == 1 forca a parada das recursoes quando valor eh encontrado.
//Controle inicia com 0, inicio padrao, procura o valor e imprime resultado. -- usado nas funcoes de busca.
//Controle inicia com 2, condicao especial que nao imprime mensagens. -- usado na funcao de insercao.
int busca_cpf(No *root, int k, int *&pControle)
{

    if(root == NULL || *pControle == 1) 				//condicoes de parada raiz nula (root = NULL) ou Controle = 0 => valor ja encontrado.
    {
        if(*pControle == 0)								//condicao de parada especial raiz nula e valor nao encontrado. => valor nao estah na arvore.
            printf("%d nao localizado.", k);

        return 0;
    }

    if(root->info->CPF == k)
    {
        if(*pControle == 0)
            printf("Encontrou: %s %d %s.", root->info->Nome, root->info->CPF, root->info->Profissao);
        *pControle = 1;									//*pControle == 1, criterio de parada, dado jah encontrado.

        return 0;
    }

    if(k > root->info->CPF)								//caso 1: k > root->info->CPF --> vai pra direita
        busca_cpf(root->dir, k, pControle);

    else											 	//caso 2: k <= root->info->CPF --> vai pra esquerda
        busca_cpf(root->esq, k, pControle);

    return 0;
}

//funcao que busca na arvore pelo nome
//nome = nome procurado.
//Controle == 1 forca a parada das recursoes quando valor eh encontrado.
//Controle inicia com 0, inicio padrao, procura o valor e imprime resultado. -- usado nas funcoes de busca.
//Controle inicia com 2, condicao especial que noh imprime mensagens. -- usado na funcos de insercao.
int busca_nome(No *root, char nome[], int *&pControle)
{

    if(root == NULL || *pControle == 1) 				//condicoes de parada raiz nulo (root = NULL) ou Controle = 0 => valor ja encontrado.
    {
        if(*pControle == 0)								//condicao de parada especial raiz nula e valor nao encontrado. => valor nao esta na arvore.
            printf("%s nao localizado. \n\n", nome);

        return 0;
    }

    if(strcmp(root->info->Nome, nome) == 0)
    {
        if(*pControle == 0)								//*pControle = 0 na busca, imprime msg.
            printf("Encontrou: %s %d %s. \n\n", root->info->Nome, root->info->CPF, root->info->Profissao);
        *pControle = 1;									//*pControle = 1, criterio de parada, dado ja encontrado.

        return 0;
    }

    if(strcmp(nome, root->info->Nome) > 0)				//caso 1: nome > root->info->Nome --> vai pra direira
        busca_nome(root->dir, nome, pControle);

    else											 	//caso 2: nome < root->info->Nome --> vai pra esquerda
        busca_nome(root->esq, nome, pControle);


    return 0;
}

//Funcao que insere Noh na arvore CPF e na arvore Nome e efetua ligacao com a struct Info correspondente.
int inserir(No*&rootCPF, No*&rootNome, char texto[])
{
    char *token;
    char Nome[50], Profissao[30];
    int CPF;
    token = strtok(texto, "; \n");						//retira nome da string texto de entrada
    strcpy(Nome, token);

    token = strtok(NULL, "; \n");						//retira CPF da string texto de entrada
    CPF = strtol(token, NULL, 10);

    token = strtok(NULL, ";\n");						//retira Profissao da string texto de entrada
    strcpy(Profissao, token);


    int Controle = 2;
    int *pControle = &Controle;							//*pControle = 2 ==> condicao especial que nao exibe as mensagens da funcao busca_cpf.

    busca_cpf(rootCPF, CPF, pControle);					//checa se CPF de entrada eh unico.
    if(*pControle == 1)
    {
        printf("CPF: %d ja cadastrado. \n", CPF);
        return -1;										//Caso CPF ja esteja cadastrado, retorna -1, encerra a insercao.
    }

    *pControle = 2;										//*pControle = 2 ==> condicao especial que nao exibe as mensagens da funcao busca_nome.

    busca_nome(rootNome, Nome, pControle);				//checa se Nome de entrada eh unico.
    if(*pControle == 1)
    {
        printf("Nome: %s ja cadastrado. \n", Nome);		//Caso Nome ja esteja cadastrado, retorna -1, encerra a insercao.
        return -1;
    }

    Info *newInfo = cria_info(Nome, CPF, Profissao);	//cria struct info com as informacoes de entrada.

    cria_no_cpf(rootCPF, newInfo);						//cria o noh na arvoreCPF e efetua ligacao com a struct Info correspondente.

    cria_no_nome(rootNome, newInfo);					//cria o noh na arvoreNOME e efetua ligacao com a struct Info correspondente.

    printf("Inseriu %s. \n", newInfo->Nome);

    return 0;
}

//funcao que recebe o nome do arquivo de entrada (argv[1]) e faz a leitura dos elementos e manda pra funcao inserir. inserindo os nohs nas duas arvores.
int lee_arquivo(int argc, char *argv[], No *&rootCPF, No*&rootNome)
{

    char texto[150];

    FILE *fpR = fopen(argv[1], "r");
    if(fpR == NULL)
    {
        perror("Erro na abertura do arquivo de leitura");
        return -1;
    }
    printf("Arquivo %s aberto. Iniciando leitura: \n\n", argv[1]);

    while(1)
    {

        fscanf(fpR, "%s", texto);
        if(feof(fpR))									//checa final do arquivo e sai do laco.
            break;
        inserir(rootCPF, rootNome, texto);
    }

    printf("Fim do arquivo. \n\n");
    fclose(fpR);
    return 0;
}

//funcao que salva os dados da memoria no arquivo.
//Controle = controla a abertura de arquivos nas recursoes.
int salva_arquivo(char *argv[], FILE *&fpW, No *&root, int *pControle)
{

    if(*pControle == 0) 								//checa se arquivo ja esta aberto. *pControle = 1 (ARQUIVO ABERTO)
    {
        fpW = fopen(argv[2], "w");
        if(fpW == NULL)
        {
            perror("Erro na abertura do arquivo de gravacao");
            return(-1);
        }
        *pControle = 1;
    }

    if(root == NULL)
        return 0;

    //Salva arquivos percorrendo EM ORDEM -- L V R
    salva_arquivo(argv, fpW, root->esq, pControle);

    fprintf(fpW, "%s;%d;%s\n", root->info->Nome, root->info->CPF, root->info->Profissao);

    salva_arquivo(argv, fpW, root->dir, pControle);

    return 0;
}

// funcao que recebe raiz e imprime EM ORDEM --- L V R.
int imprime_em_ordem(No *root)
{
    if(root == NULL)
        return 0;


    imprime_em_ordem(root->esq);
    printf("nome: %s; cpf: %d; profissao: %s\n", root->info->Nome, root->info->CPF, root->info->Profissao);
    imprime_em_ordem(root->dir);

    return 0;
}

//fun��o que procura e retorna maior valor da sub arvore passada, usado nas funcoes de remocao para procurar e retornar maior valor da SAE.
No *retorna_maior(No *&no)
{
    if(no->dir != NULL)
        return retorna_maior(no->dir);

    else 		//no->dir == NULL
    {
        No *aux = no;
        if(no->esq != NULL) 									//verifica se tem filhos a esquerda e refaz a ligacao, caso tenha, com o noh pai do MAIOR(aux).
            no = no->esq;
        else
            no = NULL;
        return aux;
    }
}

//Funcao que procura e remove por CPF
//k == numero do cpf procurado
//nomeP == nome associado ao CPF removido.
//pControle => criado para controlar a remocao do ponteiro Info* --- *pControle == 1 exclui ponteiro Info.
int remover_cpf(No *&root, int k, char *nomeP, int *pControle)
{

    if(root == NULL)										//caso 0: nao existe mais arvore para percorrer.
        return -1;


    if(k < root->info->CPF) 								//caso 1: k < root->cpf
    {
        remover_cpf(root->esq, k, nomeP, pControle);
        return 0;
    }
    if(k > root->info->CPF) 								//caso 2: k < root->cpf
    {
        remover_cpf(root->dir, k, nomeP, pControle);
        return 0;
    }

    if(k == root->info->CPF) 								//caso 3: k == root->cpf
    {
        No *aux = root;

        if (root->esq == NULL && root->dir == NULL) 		//caso 3.1: noh sem filhos
        {
            strcpy(nomeP, root->info->Nome);
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            root = NULL;

            return 0;
        }

        if (root->esq == NULL && root->dir != NULL) 		//caso 3.2: noh somente com filho da direita
        {
            root = root->dir;
            aux->dir = NULL;
            strcpy(nomeP, aux->info->Nome);
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            aux = NULL;

            return 0;
        }

        if (root->dir == NULL && root->esq != NULL) 		//caso 3.3: noh somente com filho da esquerda
        {
            root = root->esq;
            aux->esq = NULL;
            strcpy(nomeP, aux->info->Nome);
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            aux = NULL;

            return 0;
        }

        if(root->dir != NULL && root->esq != NULL)        	//caso 3.4: noh com filho a direita e a esquerda.
        {
            aux = retorna_maior(root->esq);					//aux = Maior filho direito da SAE.
            aux->esq = root->esq;
            aux->dir = root->dir;
            root->esq = root->dir = NULL;
            strcpy(nomeP, root->info->Nome);
            if(*pControle == 1)
                free(root->info);
            *pControle += 1;
            free(root);
            root = aux;
            aux = NULL;

            return 0;
        }
    }

    return NULL;
}

//Fun�ao que procura e remove por Nome
//nomeP == nome procurado
//k == CPF associado ao Nome removido.
//pK ==  ponteiro para k.
//pControle => criado para controlar a remocao do ponteiro Info* --- *pControle == 1 exclui ponteiro Info.
int remover_nome(No *&root, int *pK, char nomeP[], int *pControle)
{

    if(root == NULL) 										//caso 0: nao existe mais arvore para percorrer.
    {
        printf("Nao localizou %s para remocao. \n", nomeP);

        return -1;
    }

    if(strcmp(nomeP, root->info->Nome) < 0) 				//caso 1: nomeP < root->cpf
    {
        remover_nome(root->esq, pK, nomeP, pControle);
        return 0;
    }

    if(strcmp(nomeP, root->info->Nome) > 0) 				//caso 2: nomeP > root->cpf
    {
        remover_nome(root->dir, pK, nomeP, pControle);
        return 0;
    }

    if(strcmp(nomeP, root->info->Nome)==0) 					//caso 3: k == root->cpf
    {
        No *aux = root;

        if (root->esq == NULL && root->dir == NULL) 		//caso 3.1: noh sem filhos
        {
            *pK = aux->info->CPF;
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            root = NULL;
            printf("Removeu %s. \n\n", nomeP);

            return 1;
        }

        if (root->esq == NULL && root->dir != NULL) 		//caso 3.2: noh somente com filho da direita
        {
            root = root->dir;
            aux->dir = NULL;
            *pK = aux->info->CPF;
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            aux = NULL;
            printf("Removeu %s. \n\n", nomeP);

            return 1;
        }

        if (root->dir == NULL && root->esq != NULL) 		//caso 3.3: noh somente com filho da esquerda
        {
            root = root->esq;
            aux->esq = NULL;
            *pK = aux->info->CPF;
            if(*pControle == 1)
                free(aux->info);
            *pControle += 1;
            free(aux);
            aux = NULL;
            printf("Removeu %s. \n\n", nomeP);

            return 1;
        }

        if(root->dir != NULL && root->esq != NULL)      	//caso 3.4: noh com filho a direita e a esquerda.
        {

            aux = retorna_maior(root->esq);					//aux = Maior noh da SAE.
            aux->esq = root->esq;
            aux->dir = root->dir;
            root->esq = root->dir = NULL;
            *pK = root->info->CPF;
            if(*pControle == 1)
                free(root->info);
            *pControle += 1;
            free(root);
            root = aux;
            aux = NULL;
            printf("Removeu %s. \n\n", nomeP);

            return 1;
        }
    }

    return 0;
}

//funcao que desaloca espaco da struct No e desaloca struct Info linkada ao Noh.
int liberar_No(No *&root, int controle)
{

    if(root == NULL)
        return 0;

    liberar_No(root->esq, controle);
    liberar_No(root->dir, controle);

    if(root->info != NULL && controle == 1) 		//controle evita a remo�ao duplicada da struct Info associada ao noh CPF e noh Nome, evitando bugs.
    {
        free(root->info);
        root->info = NULL;
    }

    free(root);

    return 0;
}


int main(int argc, char *argv[])
{
    No *arvoreCPF = NULL;										// cabeca-raiz principal da arvore ordenada por CPF
    No *arvoreNome = NULL;										// cabeca-raiz principal da arvore ordenada por Nome

    lee_arquivo(argc, argv, arvoreCPF, arvoreNome);				//leitura do arquivo de entrada e criacao das arvores.
    char *token;
    char op1,op2, texto[100];

    do
    {
        printf("\n\n");
        system("pause");
        system("CLS");
        printf(" LISTA DE OPCOES: \n\n");
        printf("i <nome> <CPF> <profissao>          * Insere o funcionario. \n");
        printf("b n <NOME>                          * buscar por nome. \n");
        printf("b c <CPF>                           * buscar por CPF. \n");
        printf("e c                                 * escrever a arvore CPF no arquivo de saida. \n");
        printf("e n                                 * escrever a arvore NOME no arquivo de saida. \n");
        printf("l c                                 * lista clientes na ordem de CPF. \n");
        printf("l n                                 * lista clientes na ordem alfabetica de nomes. \n");
        printf("r c <CPF>                           * Remover por CPF. \n");
        printf("r n <NOME>                          * Remover por nome. \n");
        printf("s                                   * Sair do programa. \n\n");
        printf("Digite a opcao desejada: ");
        fflush(stdin);
        fgets(texto, 100, stdin);
        token = strtok(texto, " ;\n");
        op1 = token[0];

        if(op1 == 'i')
        {
            token = strtok(NULL, "\n");						//retira caracter inicial e manda texto de entrada no formato: <nome> <cpf> <profissao>
            strcpy(texto, token);

            inserir(arvoreCPF, arvoreNome, texto);

            continue;
        }

        if(op1 == 'b')
        {
            token = strtok(NULL, " ");						//retira caracter da string de entrada.
            op2 = token[0];
            int controle = 0;								//variavel criada para forcar parada das recursoes
            int *pControle = &controle;

            if(op2 == 'c')
            {
                token = strtok(NULL, " ");					//retira o n�mero da string de entrada.
                int k = strtol(token, NULL, 10);

                busca_cpf(arvoreCPF, k, pControle);

                continue;
            }
            if(op2 == 'n')
            {
                token = strtok(NULL, " ;");					//retira a parte do nome da string de entrada.
                char nome[50];
                strcpy(nome, token);
                token = strtok(nome, "\n"); 				//retira '\n' da string nome
                strcpy(nome, token);

                int K; 										//recebe CPF associado ao Nome.
                int *pK = &K;

                busca_nome(arvoreNome, nome, pControle);
                continue;
            }
            else
            {
                printf("Opcao invalida.");
                continue;
            }
        }
        if(op1 == 'l')
        {
            token = strtok(NULL, " ");
            op2 = token[0];

            if(op2 == 'c')
            {
                printf("\n\n");
                printf("LISTAR POR CPF: \n\n");
                imprime_em_ordem(arvoreCPF);

                continue;
            }
            if(op2 == 'n')
            {
                printf("\n\n");
                printf("LISTAR POR NOME: \n\n");
                imprime_em_ordem(arvoreNome);

                continue;
            }
            else
            {
                printf("Opcao invalida.");

                continue;
            }
        }

        if(op1 == 'e')
        {
            token = strtok(NULL, " ");
            op2 = token[0];
            int controle = 0;								//variavel criada para controlar a abertura de arquivos nas recursoes.
            int *pControle = &controle;

            if(op2 == 'c')
            {

                FILE *fpW;
                salva_arquivo(argv, fpW, arvoreCPF, pControle);

                if(fpW != NULL)
                {
                    printf("Gravou %s.", argv[2]);
                    fclose(fpW);
                }

                continue;
            }
            if(op2 == 'n')
            {
                FILE *fpW;
                salva_arquivo(argv, fpW, arvoreNome, pControle);

                if(fpW != NULL)
                {
                    printf("Gravou %s.", argv[2]);
                    fclose(fpW);
                }

                continue;
            }
            else
            {
                printf("Opcao invalida.");

                continue;
            }
        }
        if(op1 == 'r')
        {
            token = strtok(NULL, " ");
            op2 = token[0];
            int controle = 0;							//variavel criada para forcar parada das recursoes
            int *pControle = &controle;

            if(op2 == 'c')
            {

                token = strtok(NULL, " ");				//retira o numero da string de entrada.
                int K = strtol(token, NULL, 10);		//K == variavel que armazena o CPF associado ao nome a ser removido.
                int *pK = &K;

                char nome[50];
                strcpy(nome, "\0");						//zera a string nome, evitando problemas com lixo de memoria.

                int Controle = 0;						//variavel criada para controlar remocao do ponteiro info*
                int *pControle = &Controle;


                remover_cpf(arvoreCPF, K, nome, pControle);				//remove da arvoreCPF, guarda o nome associado ao CPF removido em 'nome'.

                remover_nome(arvoreNome, pK, nome, pControle);			//remove da arvoreNOME, com o nome armazenado em 'nome'.

                continue;
            }

            if(op2 == 'n')
            {
                token = strtok(NULL, " ;");				//retira a parte do nome da string de entrada.
                char nome[50];
                strcpy(nome, token);
                token = strtok(nome, "\n"); 			//retira \n da string nome
                strcpy(nome, token);

                int K=0; 								//K == variavel que armazena o CPF associado ao nome a ser removido.
                int *pK = &K;

                int Controle = 0;						//*pControle controla remocao do ponteiro info*
                int *pControle = &Controle;

                remover_nome(arvoreNome, pK, nome, pControle);		//remove da arvoreNOME, guada o CPF associado ao Nome removido em '*pK'.

                remover_cpf(arvoreCPF, K, nome, pControle);			//remove da arvoreCPF, com o CPF armazenado em '*pK'.	*pK == K;

                continue;
            }
        }
        if(op1 == 's')
            break;

        else
        {
            printf("Opcao invalida.");

            continue;
        }

    }
    while(op1 != 's');

    int controle = 0;
    liberar_No(arvoreCPF, controle);    					//controle == 0 nao remove o ponteiro Info associado ao noh, evitando a remocao dupla do Info associado.
    controle = 1;
    liberar_No(arvoreNome, controle);

    return 0;
}
