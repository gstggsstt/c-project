#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{ struct node* s[95]; int leaf; char* mean; };

typedef struct node Prefix_Trie_base;
typedef struct node* Prefix_Trie_iterator;

struct Prefix_Trie_ { struct node* root; };

typedef struct Prefix_Trie_* Prefix_Trie;

/*NOTICE: This data structure do not support copy-operation.*/

/*---------following functions are provide for users-----------*/

/*Allocate a new trie-tree*/
Prefix_Trie New_Prefix_Trie();
/*Insert a word 'str' with its meaming 'mn'.*/
void Insert(Prefix_Trie t,const char* str,const char* mn);
/*Find word 'str' in the tree, 
	if found return the meaning, if not return NULL*/
char* Find(Prefix_Trie t,const char* str);
/*Recycle the whole tree, but the memory is not freed 
	 until you call function Free_all() or the program ends.*/
void Free_Prefix_Trie(Prefix_Trie t);
/*Free the memory-pool*/
void Free_all();

/*---------do NOT use these fucntions directly-----------------*/

char* Find_(Prefix_Trie_iterator t,const char* str);
void Insert_(Prefix_Trie_iterator *t,const char* str,const char* mn);
void Free_Prefix_Trie_node(Prefix_Trie_iterator t);

/*-------------------------------------------------------------*/

/*-------------------------ALLOCATOR--------------------------*/
/*
		To help the code access memmory faster, we allocate memory 
		block-by-block, size of which increase by double.
		
		For example, we first allocate a block of memory which size is 1, 
		when you insert the second element, we found memory is not enough,
		then we allocate another block which size is 2, and then 4,8,etc.
		
		This method will make addresses continuously, which is good to 
	improve the speed, and avoid the memory-size alignment when you 
	call 'malloc' one-by-one.
*/
Prefix_Trie_base*  __Memlist_prefix_trie[31];
Prefix_Trie_base** __Tshlist_prefix_trie[31];

int __cntm_prefix_trie,__cntt_prefix_trie,__topm_prefix_trie,
__topt_prefix_trie,__Inited_prefix_trie;
int __Sizelist_prefix_trie[31];

void  Free_all()
{
	int i;/*free the memory-pool*/
	for(i=0;i<=__cntm_prefix_trie;++i)
		free(__Memlist_prefix_trie[i]),free(__Tshlist_prefix_trie[i]);
	__Inited_prefix_trie=0; return ;
}

void    Init_allocator()
{
	/*allocate a memory pool, size of which is 1*/
	__Sizelist_prefix_trie[0]=1; __cntm_prefix_trie=__cntt_prefix_trie=0;
	__topm_prefix_trie=__topt_prefix_trie=-1;
	__Memlist_prefix_trie[__cntm_prefix_trie]=(Prefix_Trie_base*)malloc
	(sizeof(Prefix_Trie_base)*__Sizelist_prefix_trie[__cntm_prefix_trie]);
	__Tshlist_prefix_trie[__cntm_prefix_trie]=(Prefix_Trie_base**)malloc
	(sizeof(Prefix_Trie_base)*__Sizelist_prefix_trie[__cntm_prefix_trie]);
}

Prefix_Trie_base*  Get(Prefix_Trie_base* t)
{
	/*initialize a node*/
	memset(t->s,0,sizeof(t->s));
	t->mean=0;
	t->leaf=0; return t;
}

Prefix_Trie_base*  ALLOC()
{
	/*recycle trash nodes first*/
	if(!__Inited_prefix_trie)
		Init_allocator(),__Inited_prefix_trie=1;
	if(__topt_prefix_trie>=0)
		return
	Get(__Tshlist_prefix_trie[__cntt_prefix_trie][__topt_prefix_trie--]);
		/*current stack is empty, use next stack*/
	else if(__cntt_prefix_trie!=0)
	{
		__cntt_prefix_trie--;
		__topt_prefix_trie=__Sizelist_prefix_trie[__cntt_prefix_trie]-1;
		return
	Get(__Tshlist_prefix_trie[__cntt_prefix_trie][__topt_prefix_trie--]);
	}
		/*allocate a node from the memory-pool*/
	if(__topm_prefix_trie<__Sizelist_prefix_trie[__cntm_prefix_trie]-1)
		return
	Get(&__Memlist_prefix_trie[__cntm_prefix_trie][++__topm_prefix_trie]);
		/*expand the memory-pool*/
	__cntm_prefix_trie++;
	__Sizelist_prefix_trie[__cntm_prefix_trie]=
	__Sizelist_prefix_trie[__cntm_prefix_trie-1]<<1;
	__Memlist_prefix_trie[__cntm_prefix_trie]= (Prefix_Trie_base*)malloc
	(sizeof(Prefix_Trie_base)*__Sizelist_prefix_trie[__cntm_prefix_trie]);
	__Tshlist_prefix_trie[__cntm_prefix_trie]=(Prefix_Trie_base**)malloc
	(sizeof(Prefix_Trie_base)*__Sizelist_prefix_trie[__cntm_prefix_trie]);
	__topm_prefix_trie=-1;
	return
	Get(&__Memlist_prefix_trie[__cntm_prefix_trie][++__topm_prefix_trie]);
}

void    RECYCLE(Prefix_Trie_base* t)
{
	/*RECYCLE a node and push it to stack of trash*/
	if(__topt_prefix_trie<__Sizelist_prefix_trie[__cntt_prefix_trie]-1)
	{
		__Tshlist_prefix_trie[__cntt_prefix_trie][++__topt_prefix_trie]=t;
		return ;
	}
		/*if the stack is not enough, use the next stack*/
	__cntt_prefix_trie++; __topt_prefix_trie=-1;
	__Tshlist_prefix_trie[__cntt_prefix_trie][++__topt_prefix_trie]=t;
	return ;
}
/*-------------------------ALLOCATOR-END--------------------------*/

void Free_Prefix_Trie_node(Prefix_Trie_iterator t)
{
	int i; for(i=0;i<95;++i)
	if(t->s[i]) Free_Prefix_Trie_node(t->s[i]);
	RECYCLE(t);
}

void Free_Prefix_Trie(Prefix_Trie t)
{
	Free_Prefix_Trie_node(t->root);
	free(t);
}

Prefix_Trie New_Prefix_Trie()
{
	Prefix_Trie t=(Prefix_Trie)malloc(sizeof(struct Prefix_Trie_));
	t->root=ALLOC(); return t;
}

void Insert_(Prefix_Trie_iterator *t,const char* str,const char* mn)
{
	if(!*t) *t=ALLOC();
	if(str[0]==0)
	{
		(*t)->mean=(char*)malloc(sizeof(char)*(strlen(mn)+1));
		strcpy((*t)->mean,mn);
		(*t)->leaf=1;
		return ;
	}
	if(str[0]>126 || str[0]<32) return ;
	Insert_(&((*t)->s[str[0]-32]),str+1,mn);
}

void Insert(Prefix_Trie t,const char* str,const char* mn)
{ Insert_(&t->root,str,mn); return ; }

char* Find_(Prefix_Trie_iterator t,const char* str)
{
	if(!t) return 0;
	if(str[0]==0)
	{
		if(t->leaf) return t->mean;
		else return 0;
	}
	if(str[0]>126 || str[0]<32) return 0;
	return Find_(t->s[str[0]-32],str+1);
}

char* Find(Prefix_Trie t,const char* str)
{ return Find_(t->root,str); }

