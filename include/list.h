#ifndef LIST_H
#define LIST_H

typedef struct s_listNode
{
	void *key;
	void *data;
	struct s_listNode *next;
	struct s_listNode *prev;
}t_listNode;

typedef struct s_list
{
	int length;
	t_listNode *first;
	t_listNode *last;
}t_list;

/**
	*	inicializa uma lista
	*	se NULL for passado, aloca a memoria necessaria
	*	se list for diferente de NULL, utiliza o mesmo ponteiro
	*	retorna o ponteiro para a lista inicializada
	*/
t_list *listInit(t_list *list);

/**
	*	desaloca os nodos da lista,
	*	mas nao libera a memoria dos dados nem o descritor
	*/
void listDestroy(t_list *list);

/**
	*	libera a memoria ocupada pelos dados e chaves (utilizando os destrutores dados) e pelos nodos da lista,
	*	mas nao libera o descritor
	*/
void listDestroyData(t_list *list,void (*keyDestructor)(void *key), void (*dataDestructor)(void *data));

/**
	*	adiciona um elemento no final da lista
	*/
void listAppend(t_list *list,void *key,void *data);

/**
	*	concatena as duas listas, aumentando a da esquerda e fazendo uma copia
	*	da lista da direita
	*/
void listExtendCopy(t_list *left,t_list *right);

/**
	*	concatena as duas listas, aumentando a da esquerda e fazendo uma copia
	*	dos dados e das cahves da lista da direita usando as funcoes dadas
	*	a funcao de copia deve alocar um ponteiro para a estrutura desejada, copiar os dados
	*	e retornar esse ponteiro
	*/
void listExtendDataCopy(t_list *left,t_list *right, void* (*keyCopyFunction)(void *),void* (*dataCopyFunction)(void *));

/**
	*	ordena a lista de forma usando a funcao de comparacao dada
	*	a funcao de comparacao deve ser:
	*	f(x,y) < 0 sse x precede y
	*	f(x,y) >=0 caso contrario
	*	usa insertion sort
	*/
void listSort(t_list *list, int (*compare)(void *,void *));

/**
	*	busca um valor na lista
	*	se encontar, retorna o nodo que contem a chave procurada
	*	caso contrario, retorna NULL
	*	a funcao de comparacao deve retornar 0 caso sejam iguais
	*/
t_listNode *listSearch(t_list *list,const void *val, int (*compare)(const void *key,const void *value));
#endif
