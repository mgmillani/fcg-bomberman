#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "configLoader.h"
#include "list.h"
#include "abp.h"
#include "debug.h"

#define ISWHITESPACE(c) ((c)==' ' || (c)=='\t')
#define ISEOL(c) ((c)=='\n' || (c)=='\r')

/**
	*	le todos os bytes de um arquivo
	*	retorna o tamanho do arquivo
	*/
int readRaw(const char *filename,char **data)
{
	FILE *fl = fopen(filename,"rb");
	if(fl == NULL)
		return 0;

	int length;
	fseek(fl,0,SEEK_END);
	length = ftell(fl);
	fseek(fl,0,SEEK_SET);
	*data = malloc(length);

	fread(*data,1,length,fl);

	fclose(fl);
	return length;
}

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
t_list* loadConfig(const char *filename,const char *defaultCategory,t_list *errors)
{
	FILE *fl = fopen(filename,"rb");
	if(fl == NULL)
		return NULL;

	int length;
	fseek(fl,0,SEEK_END);
	length = ftell(fl);
	fseek(fl,0,SEEK_SET);

	t_list *cats;
	cats = listInit(NULL);
	addCategory(cats,defaultCategory);
	t_listNode *category = cats->first;

	char *data = malloc(length+1);
	fread(data,1,length,fl);
	fclose(fl);

	int i;
	int line = 1;
	int b;
	int status;
	for(i=0;i<length;i++)
	{
		if(ISWHITESPACE(data[i]))
			continue;
		if(ISEOL(data[i]))
		{
			line++;
			continue;
		}
		//definicao de uma categoria
		if(data[i] == '[')
		{
			int startLine = line;
			//elimina espacos a esquerda
			i++;
			while(i<length)
			{
				if(ISWHITESPACE(data[i]))
					i++;
				else
					break;
			}
			b = i;
			int end = -1;
			//busca o final do nome da categoria
			while(i<length)
			{
				if(ISEOL(data[i]))
				{
					line++;
					end = i;
				}
				if(ISWHITESPACE(data[i]))
					end = i;
				if(data[i] == ']')
				{
					if(end == -1)
						end = i;
					char aux = data[end];
					data[end] = '\0';
					category = addCategory(cats,data+b);
					data[end] = aux;
					break;
				}
				else
					i++;
			}
			if(i>=length)
			{
				addError(errors,ErrorOpenCategory,startLine);
			}
		}
		//se for um comentario, busca um '\n' ou o final do arquivo
		else if(data[i] == COMMENT)
		{
			while(i<length)
				if(ISEOL(data[i]))
				{
					line++;
					break;
				}
				else
				{
					i++;
				}
		}
		//definicao de uma variavel
		else
		{
			int startLine = line;
			char *var = NULL,*val = NULL;
			//busca o '=' ou um espaco em branco
			for(b=i;i<length;i++)
			{
				if(ISEOL(data[i]))
				{
					line++;
					i++;
					break;
				}
				if(data[i] == '=' || ISWHITESPACE(data[i]))
				{
					if(b==i)
						break;
					char aux = data[i];
					data[i] = '\0';
					var = malloc(i-b+1);
					strcpy(var,data+b);
					data[i] = aux;
					//busca o '='
					if(ISWHITESPACE(aux))
					{
						while(i<length)
						{
							if(data[i] !='=')
								i++;
							else
								break;
						}
					}
					i++;
					break;
				}
			}
			if(var == NULL)
				addError(errors,ErrorNoVarName,startLine);

			//busca o valor
			//primeiro, pula os espacos a esquerda
			while(i<length)
				if(ISWHITESPACE(data[i]))
					i++;
				else
					break;
			b = i;
			//busca o final
			while(i<length)
			{
				if(ISEOL(data[i]))
				{
					line++;
					break;
				}
				else if(ISWHITESPACE(data[i]))
				{
					break;
				}
				i++;
			}
			//se houver algum caractere
			if(b!=i)
			{
				val = malloc(i-b+1);
				char aux = data[i];
				data[i] = '\0';
				strcpy(val,data+b);
				data[i] = aux;
			}

			if(val == NULL)
				addError(errors,ErrorEmptyDefinition,startLine);

			if(var!=NULL && val !=NULL)
			{
				category->data = abpAddNode(category->data,var,val,(int (*)(void*, void*))strcmp,&status);
				if(status==-1)
				{
					addError(errors,ErrorRedefinedVariable,startLine);
				}
			}
		}//end else (definicao de uma variavel)
	}//end for
	free(data);
	return cats;
}

/**
	*	adiciona uma categoria a uma lista
	*	copia os bytes do vetor dado
	*	se uma categoria com o mesmo nome existir, nao adiciona
	*	retorna o nodo que contem a categoria
	*/
t_listNode *addCategory(t_list *cats,const char *name)
{
	if(cats == NULL || name == NULL)
		return NULL;

	t_listNode *node;
	//verifica se o nome ja existe
	node = listSearch(cats,name,(int (*)(void*,void*))strcmp);
	//se nao existir
	if(node == NULL)
	{
		char *catName = malloc(strlen(name)+1);
		strcpy(catName,name);
		t_abp *cat = NULL;
		listAppend(cats,catName,cat);
		return cats->last;
	}
	else
	{
		return node;
	}
}

/**
	*	adiciona a mensagem de erro apropriada no final da lista
	*/
void addError(t_list *errors,int errorCode,int line)
{
	if(errors == NULL)
		return;

	char *phrase;
	int len = (1+log10(line));
	switch(errorCode)
	{
		//nada ao lado direito da igualdade
		case ErrorEmptyDefinition:
			len += 32;
			phrase = malloc(len+1);
			sprintf(phrase,"Error: Empty attribution (line %d)%c",line,'\0');
			listAppend(errors,phrase,NULL);
			break;
		//nada a esquerda da igualdade
		case ErrorNoVarName:
			len += 32;
			phrase = malloc(len+1);
			sprintf(phrase,"Error: Nameless variable (line %d)%c",line,'\0');
			listAppend(errors,phrase,NULL);
			break;
		//quando o ']' nao foi encontrado
		case ErrorOpenCategory:
			len += 26;
			phrase = malloc(len+1);
			sprintf(phrase,"Error: Missing ']' (line %d)%c",line,'\0');
			listAppend(errors,phrase,NULL);
			break;
		case ErrorRedefinedVariable:
			len += 33;
			phrase = malloc(len+1);
			sprintf(phrase,"Error: Redefined variable (line %d)%c",line,'\0');
			listAppend(errors,phrase,NULL);
			break;
	}
}

/**
  * libera a memoria alocada por uma chamada a loadConfig
  * se um dos parametros for NULL, ignora-o
  * somente o descritor de config eh liberado
  */
void configDestroy(t_list *config, t_list *errors)
{
	if(config != NULL)
	{
		t_listNode *node;
		for(node=config->first ; node!=NULL ; node=node->next)
		{
			free(node->key);
			abpDestroy(node->data,free,free);
			free(node->data);
		}
		free(config);
	}

	if(errors != NULL)
	{
		listDestroyData(errors,free,NULL);
	}
}























