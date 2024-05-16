/*
Exercicio 3: Balancear Árvore Binaria
Igor Correa Domingues de Almeida
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define TAM_PALAVRA 30

// define o tipo de dado a ser colocado na lista
typedef int Tdado;

typedef struct TreeNode {
  Tdado info;             // dado armazenado no nó
  struct TreeNode *left;  // ponteiro para o ramo esquerdo
  struct TreeNode *right; // ponteiro para o nramo direito
} TreeNode;

//Funcao para liberar a arvore
void freeBinTree(TreeNode *bt) {
  if (!bt) {
    return;
  }
  freeBinTree(bt->left);
  freeBinTree(bt->right);
  free(bt);
  bt = NULL;
}

//--------------------
// função de inserção:
//--------------------
TreeNode* insertBinTree(Tdado x, TreeNode *bt) {
   TreeNode *aux;

   // verificar se o ponteiro é nulo
   if (!bt) {
      // insere o novo nó
      aux = (TreeNode*)malloc(sizeof(TreeNode));
      if (!aux) {
        printf("Erro: Memória insuficiente!\n");
        return bt;
      } else {
        aux->info = x;
        aux->left = NULL;
        aux->right = NULL;
        return aux;
      }
   } else if (x < bt->info) {
      // x é menor que o valor do no atual, desce pelo ramo esquerdo
      bt->left = insertBinTree(x, bt->left); // insere no ramo esquerdo
      return bt;
   } else {
      // x é maior que o valor do no atual, desce pelo direito
      bt->right = insertBinTree(x, bt->right); // insere no ramo direito
      return bt;
   }
}

//Funcao para imprimir arvore no arquivo
void printBinTree(const TreeNode *bt, int level, FILE *f) {
   int i ;
   if (!bt) {
       return;
   } else {
      printBinTree(bt->right, level + 1, f);
     for (i = 0; i < level; i++) {
      fprintf(f, "----|");
     }
     fprintf(f, "%d\n", bt->info);
     printBinTree(bt->left, level + 1, f);
   }
}

//Funcao para contar o numero de nós na arvore
int numNodes(TreeNode* bt){
  if(!bt)
    return 0;
  else{
    return 1 + numNodes(bt->right) + numNodes(bt->left);
  }
}

//Funcao para inserir os elementos da arvore no vetor
void emOrdem(TreeNode* bt, int *v){
  static int i = 0;
  if(bt == NULL)
    return;
  
  emOrdem(bt->left, v);
  v[i] = bt->info;
  i++;
  emOrdem(bt->right, v);
}

//Funcao para construir arvore balanceada a partir do vetor
TreeNode* buildBalancedTree(int *v, int low, int high){
  TreeNode* aux = NULL;
  if(low > high) //Parar quando houver sobreposicao
    return aux;
  int mid = (low + high)/2; //Metade entre os parametros

  //Similar à funcao InsertBinTree
  aux = (TreeNode*)malloc(sizeof(TreeNode));
  if (!aux) {
    printf("Erro: Memória insuficiente!\n");
    exit(1);
  } else {
    aux->info = v[mid]; //Inserir o elemento que esta no meio
    // Balanceamento das subárvores
    //Para a esquerda, usar a metade menor do vetor
    aux->left = buildBalancedTree(v, low, mid - 1);
    //Para a direita usar a metade maior do vetor
    aux->right = buildBalancedTree(v, mid + 1, high); 
    return aux;
  }
}

//Funcao juntando as 2 necessarias para o balanceamento
TreeNode* balancedTree(TreeNode* bt, int low, int high){
  int v[numNodes(bt)]; //Vetor criado localmente
  TreeNode* aux = NULL;
  emOrdem(bt, v); //Preencher o vetor e limpar arvore
  //Criar arvore balanceada e retorna-la
  aux = buildBalancedTree(v, low, high); 
  freeBinTree(bt);
  return aux;
}


//Funcao main
int main(int argc, char *argv[]) {
  FILE *fIn, *fOut; // Arquivos de entrada e saida
  char filenameIn[31], filenameOut[31];     // Nome do arquivo de entrada 
  int number; //Recebe os numeros do arquivo
  TreeNode *bt = NULL; //Inicializar a arvore

  //Verificacao de parametros
  if (argc != 3) {
      printf("Erro 1: Numero de argumentos invalido\n");
      printf("Modo de uso: ex3 <arqIN> <arqOut>\n");
      return 1;
   }

  strcpy(filenameIn, argv[1]);
  strcpy(filenameOut, argv[2]);
  

  fIn = fopen(filenameIn, "r"); // Abrir o arquivo de entrada em r
  fOut = fopen(filenameOut, "w"); // Abrir o arquivo de saida em w


  // Testes de erros
  if (!fOut) {
    printf("Erro: Arquivo out.txt não pode ser aberto.\n");
    return 1;
  }
  if (!fIn) {
    printf("Erro: Arquivo %s não pode ser aberto.\n", filenameIn);
    return 1;
  }


  //Enquanto existem numeros para ler e parar no zero
  while (fscanf(fIn, "%d", &number) == 1  && number) {
    bt = insertBinTree(number, bt);
  }
  //Imprimir a arvore original no arquivo
  fprintf(fOut, "Árvore Original:\n");
  printBinTree(bt, 0, fOut);

  //Imprimir a arvore balanceada
  fprintf(fOut, "\nÁrvore Balanceada:\n");
  //Low = primeira posicao do vetor(0)
  //High = tamanho do vetor - 1
  bt = balancedTree(bt, 0, numNodes(bt)-1);
  printBinTree(bt, 0, fOut);


  // Fechar os arquivos e limpar a arvore
  fclose(fIn);
  fclose(fOut);
  freeBinTree(bt);

  return 0;
}

