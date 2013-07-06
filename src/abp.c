#include <stdlib.h>

#include "abp.h"
#include "debug.h"

/**
	*	adiciona um nodo com a chave e dado recebidos
	*	a funcao de comparacao de chaves deve retorna 0 quando forem iguais,
	*	<0 quando a primeira for menor e >0 quando for maior
	*	se a arvore dada for NULL, aloca uma nova
	*	retorna a raiz da arvore
	*	se ja existir um nodo com a chave dada, escreve -1 em status e retorna a raiz
	*	caso contrario, escreve 0 em status
	*/
t_abp *abpAddNode(t_abp *tree, void *key, void *data, int (*cmp)(void *, void *),int *status)
{
	*status = 0;
	if (tree == NULL)
	{
		tree = malloc(sizeof(*tree));
		tree->key = key;
		tree->data = data;
		tree->left = NULL;
		tree->right = NULL;
		return tree;
	}

	int compare = cmp(key,tree->key);
	if(compare>0)
		tree->right = abpAddNode(tree->right,key,data,cmp,status);
	else if(compare<0)
		tree->left = abpAddNode(tree->left,key,data,cmp,status);
	else
		*status = -1;

	return tree;
}

/**
	*	busca pela chave na abp
	*	retorna o nodo que possui tal chave ou
	*	NULL caso nao encontre
	*/
t_abp *abpSearchNode(const void *key,t_abp *tree,int (*cmp)(const void *,const void *))
{

	if(tree == NULL)
		return NULL;

	int compare = cmp(key,tree->key);

	if(compare == 0)
		return tree;
	else if(compare > 0)
		return abpSearchNode(key,tree->right,cmp);
	else
		return abpSearchNode(key,tree->left,cmp);

}

/**
	*	escreve as chaves de uma abp usando as funcoes de escrita dadas
	*	a profundidade determina quantos espacos haverao a esquerda
	*/
void abpPrint(t_abp *tree,void (*printKey)(void *),void (*printData)(void *),int depth)
{
	if(tree == NULL)
		return;
	int i;
	for(i=0;i<depth;i++)
		printf("|");

	printKey(tree->key);
	printf(":");
	printData(tree->data);
	printf("\n");

	abpPrint(tree->left,printKey,printData,depth+1);
	abpPrint(tree->right,printKey,printData,depth+1);

}

/**
  * funcao basica para ser usada junto de abpPrint
  * simplesmente escreve data como uma string
  */
void abpStringPrint(void *data)
{
	printf("%s\n",(char *)data);
}

/**
  * destroi uma abp usando os destrutores dados
  * se um deles for NULL, nao sera usado
  * destroi os descritores
  */
void abpDestroy(t_abp *tree, void (*keyDestructor)(void *),void (*dataDestructor)(void *))
{
	if(tree == NULL)
		return;

	if(keyDestructor != NULL)
		keyDestructor(tree->key);
	if(dataDestructor != NULL)
		dataDestructor(tree->data);

	abpDestroy(tree->left,keyDestructor,dataDestructor);
	abpDestroy(tree->right,keyDestructor,dataDestructor);
	free(tree);
}



































