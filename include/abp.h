#ifndef ABP_H
#define ABP_H

typedef struct s_abp
{
	void *key;
	void *data;
	struct s_abp *left;
	struct s_abp *right;
}t_abp;

/**
	*	adiciona um nodo com a chave e dado recebidos
	*	a funcao de comparacao de chaves deve retorna 0 quando forem iguais,
	*	<0 quando a primeira for menor e >0 quando for maior
	*	se a arvore dada for NULL, aloca uma nova
	*	retorna a raiz da arvore
	*/
t_abp *abpAddNode(t_abp *tree, void *key, void *data, int (*cmp)(void *, void *),int *status);

/**
	*	busca pela chave na abp
	*	retorna o nodo que possui tal chave ou
	*	NULL caso nao encontre
	*/
t_abp *abpSearchNode(const void *key,t_abp *tree,int (*cmp)(const void *,const void *));

/**
	*	escreve as chaves de uma abp usando as funcoes de escrita dadas
	*	a profundidade determina quantos espacos haverao a esquerda
	*/
void abpPrint(t_abp *tree,void (*printKey)(void *),void (*printData)(void *),int depth);

/**
  * funcao basica para ser usada junto de abpPrint
  * simplesmente escreve data como uma string
  */
void abpStringPrint(void *data);

/**
  * destroi uma abp usando os destrutores dados
  * se um deles for NULL, nao sera usado
  * destroi os descritores
  */
void abpDestroy(t_abp *tree, void (*keyDestructor)(void *),void (*dataDestructor)(void *));

#endif // ABP_H

