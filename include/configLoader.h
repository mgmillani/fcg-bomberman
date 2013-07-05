#ifndef CONFIGLODAER_H
#define CONFIGLODAER_H

#include "abp.h"
#include "list.h"

#define COMMENT '#'

enum {ErrorRedefinedVariable,ErrorEmptyDefinition,ErrorNoVarName,ErrorOpenCategory};

typedef struct s_category
{
	char *name;
	t_abp *variables;
}t_category;

/**
	*	carrega os valores de um arquivo de configuracao
	*	gera uma lista onde cada chave eh o nome da categoria e o dado eh uma ABP
	*	com os nomes das variaveis nas chaves e o valor nos dados
	*	o arquivo deve estar no formato:
	*	[categoria1]
	*	var = valor123
	*	...
	*	comentarios sao descritos pelo simbolo COMMENT a sua esquerda, indo ate o
	*	final da linha
	*	se alguma variavel for definida antes de uma categoria,
	*	ficara em defaultCategory
	*	escreve mensagens de erro nas chaves da lista dada, sendo o mais novo o ultimo
	*/
t_list* loadConfig(const char *filename,const char *defaultCategory,t_list *errors);

/**
	*	adiciona uma categoria a uma lista
	*	copia os bytes do vetor dado
	*	se uma categoria com o mesmo nome existir, nao adiciona
	*	retorna o nodo que contem a categoria
	*/
t_listNode *addCategory(t_list *cats,const char *name);

/**
	*	adiciona a mensagem de erro apropriada no final da lista
	*/
void addError(t_list *errors,int errorCode,int line);

/**
  * libera a memoria alocada por uma chamada a loadConfig
  * se um dos parametros for NULL, ignora-o
  * somente o descritor de config eh liberado
  */
void configDestroy(t_list *config, t_list *errors);

#endif // CONFIGLODAER_H

