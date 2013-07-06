#include <stdlib.h>

#include "list.h"
/**
	*	inicializa uma lista
	*	se NULL for passado, aloca a memoria necessaria
	*	se list for diferente de NULL, utiliza o mesmo ponteiro
	*	retorna o ponteiro para a lista inicializada
	*/
t_list *listInit(t_list *list)
{

	if(list == NULL)
		list = malloc(sizeof(*list));

	list->length = 0;
	list->first = NULL;
	list->last = NULL;

	return list;
}

/**
	*	adiciona um elemento no final da lista
	*/
void listAppend(t_list *list,void *key,void *data)
{
	t_listNode *node = malloc(sizeof(*node));
	node->key = key;
	node->data = data;
	node->next = NULL;
	node->prev = NULL;

	if(list->last == NULL)
		list->first = node;
	else
	{
		node->prev = list->last;
		list->last->next = node;
	}

	list->last = node;
	list->length++;
}

/**
	*	libera a memoria ocupada pelos dados e chaves (utilizando os destrutores dados) e pelos nodos da lista,
	*	mas nao libera o descritor
	* destrutores NULL nao sao usados
	*/
void listDestroyData(t_list *list,void (*keyDestructor)(void *key), void (*dataDestructor)(void *data))
{
	t_listNode *node,*aux;
	for(node = list->first;node!=NULL;node=aux)
	{
		if(dataDestructor!=NULL)
			dataDestructor(node->data);

		if(keyDestructor!=NULL)
			keyDestructor(node->key);

		aux = node->next;
		free(node);
	}
	list->length = 0;
	list->first = NULL;
	list->last = NULL;
}

/**
	*	desaloca os nodos da lista,
	*	mas nao libera a memoria dos dados nem o descritor
	*/
void listDestroy(t_list *list)
{
		t_listNode *node,*aux;
	for(node = list->first;node!=NULL;node=aux)
	{
		aux = node->next;
		free(node);
	}
	list->length = 0;
	list->first = NULL;
	list->last = NULL;
}


/**
	*	concatena as duas listas, aumentando a da esquerda e fazendo uma copia
	*	da lista da direita
	*/
void listExtendCopy(t_list *left,t_list *right)
{

	t_listNode *node;
	for(node=right->first;node!=NULL;node=node->next)
	{
		listAppend(left,node->key,node->data);
	}

}


/**
	*	concatena as duas listas, aumentando a da esquerda e fazendo uma copia
	*	dos dados e das cahves da lista da direita usando as funcoes dadas
	*	a funcao de copia deve alocar um ponteiro para a estrutura desejada, copiar os dados
	*	e retornar esse ponteiro
	*/
void listExtendDataCopy(t_list *left,t_list *right, void* (*keyCopyFunction)(void *),void* (*dataCopyFunction)(void *))
{

	t_listNode *node;
	void *data;
	void *key;
	for(node=right->first;node!=NULL;node=node->next)
	{
		data = dataCopyFunction(node->data);
		key = keyCopyFunction(node->key);
		listAppend(left,key,data);
	}

}


/**
	*	ordena a lista de forma usando a funcao de comparacao dada
	*	a funcao de comparacao deve ser:
	*	f(x,y) < 0 sse x precede y
	*	f(x,y) >=0 caso contrario
	*	usa insertion sort
	*/
void listSort(t_list *list, int (*compare)(void *,void *))
{

	if(list->length < 2)
		return;
	t_listNode *cur,*dest,*aux;
	for(cur = list->first->next;cur!=list->last;cur=aux)
	{
		aux = cur->next;
		dest = cur->prev;
		while(compare(cur->key,dest->key) < 0)
		{
			dest = dest->prev;
			//adiciona no inicio da lista
			if(dest == NULL)
			{
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
				cur->prev = NULL;
				cur->next = list->first;
				list->first->prev = cur;
				list->first = cur;
				break;
			}
		}
		if(dest!=cur->prev)
		{
			cur->prev->next = cur->next;
			cur->next->prev = cur->prev;
			cur->prev = dest;
			cur->next = dest->next;
			dest->next->prev = cur;
			dest->next = cur;
		}
	}

	dest = cur->prev;
	while(compare(cur->key,dest->key) < 0)
	{
		dest = dest->prev;
		//adiciona no inicio da lista
		if(dest == NULL)
		{
			list->last = cur->prev;
			cur->prev->next = NULL;
			cur->prev = NULL;
			cur->next = list->first;
			list->first->prev = cur;
			list->first = cur;

			break;
		}
	}
	if(dest!=NULL && dest != cur->prev)
	{
		list->last = cur->prev;
		cur->prev->next = NULL;
		cur->prev = dest;
		cur->next = dest->next;
		dest->next->prev = cur;
		dest->next = cur;
	}
}


/**
	*	busca um valor na lista
	*	se encontar, retorna o nodo que contem a chave procurada
	*	caso contrario, retorna NULL
	*	a funcao de comparacao deve retornar 0 caso sejam iguais
	*/
t_listNode *listSearch(t_list *list,const void *val, int (*compare)(const void *key,const void *value))
{
	t_listNode *n;

	for(n=list->first;n!=NULL;n=n->next)
	{
		if(compare(n->key,val) == 0)
			return n;
	}
	return NULL;
}










