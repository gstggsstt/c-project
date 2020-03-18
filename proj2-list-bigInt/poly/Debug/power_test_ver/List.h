/*
  This file provids a template of double-list.
  You may define a double-list L in this way:
	  List L=New_list();
	  
  And you can access elements by 'List_iterator',here is an example.
  
	  List_iterator it;
	  for(it=L->begin;it!=L->end;it=it->next)
		  printf("%d ",it->data); 
		  
  These functions are declared:
  
	  void Insert_before(list_name,list_iterator,data);
		  *Insert element 'data' before 'list_iterator' in List 'list_name'
		  
	  List_iterator Delete(list_name,list_iterator);
		  *Delete element of 'list_iterator' and returns the next iterator
			of this element.
			
	  void Free_list(list_name);
		  *Soft-release the list, give memory back to allocator.
		  
	  void Free_all();
		  *Free all memory allocated, and all your Lists will be destroyed. 
	  
*/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "BigN.h"

struct Pair { BigN a; int b; };
typedef struct Pair _tp_list;

/*struct of list node*/
struct List_base
{
	_tp_list data;
	struct List_base* next;
	struct List_base* last;
};
typedef struct List_base List_base;
typedef List_base* List_iterator;

/*struct of List*/
struct List_
{
	List_iterator begin;
	List_iterator end;
};
typedef struct List_* List;

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
List_base*  Memlist[31];
List_base** Tshlist[31];

int cntm,cntt,topm,topt,Inited;
int Sizelist[31];

void    Free_all()
{
	int i;/*free the memory-pool*/
	for(i=0;i<=cntm;++i)
		free(Memlist[i]),free(Tshlist[i]);
	Inited=0; return ;
}

void    Init_allocator()
{
	/*allocate a memory pool, size of which is 1*/
	Sizelist[0]=1; cntm=cntt=0; topm=topt=-1;
	Memlist[cntm]=(List_base*)malloc(sizeof(List_base)*Sizelist[cntm]);
	Tshlist[cntm]=(List_base**)malloc(sizeof(List_base)*Sizelist[cntm]);
}

List_base*  Get(List_base* t, _tp_list x)
{
	/*initialize a node*/
	t->data=x; t->next=t->last=0;return t;
}

List_base*  ALLOC(_tp_list x)
{
	if(!Inited) Init_allocator(),Inited=1;
	/*recycle trash nodes first*/
	if(topt>=0) return Get(Tshlist[cntt][topt--],x);
	/*current stack is empty, use next stack*/
	else if(cntt!=0)
	{
		cntt--,topt=Sizelist[cntt]-1;
		return Get(Tshlist[cntt][topt--],x);
	}
	/*allocate a node from the memory-pool*/
	if(topm<Sizelist[cntm]-1)
		return Get(&Memlist[cntm][++topm],x);
	/*expand the memory-pool*/
	cntm++;
	Sizelist[cntm]=Sizelist[cntm-1]<<1;
	Memlist[cntm]=(List_base*)malloc(sizeof(List_base)*Sizelist[cntm]);
	Tshlist[cntm]=(List_base**)malloc(sizeof(List_base)*Sizelist[cntm]);
	topm=-1; return Get(&Memlist[cntm][++topm],x);
}

void    RECYCLE(List_base* t)
{
	/*RECYCLE a node and push it to stack of trash*/
	if(topt<Sizelist[cntt]-1)
	{
		Tshlist[cntt][++topt]=t;
		return ;
	}
	/*if the stack is not enough, use the next stack*/
	cntt++; topt=-1;
	Tshlist[cntt][++topt]=t;
	return ;
}
/*-------------------------ALLOCATOR-END--------------------------*/

List New_list()
{
	/*Returns a new list*/
	List C=(List)malloc(sizeof(struct List_));
	C->begin=C->end=ALLOC((_tp_list){0,0}); return C;
	/*List->end is a virtual node which help us to use*/
}

void    Insert_before(List C,List_iterator t,const _tp_list x)
{
	List_iterator temp=t->last;
	t->last=ALLOC(x);/*new node*/
	t->last->next=t;
	t->last->last=temp;
	if(temp)temp->next=t->last;
	else C->begin=t->last;
}

void    Free_list(List C)
{
	List_iterator it;
	for(it=C->begin;it!=C->end;it=it->next)
		RECYCLE(it);
	RECYCLE(C->end); free(C);
}

List_iterator    Delete(List C,List_iterator t)
{
	if(t==C->end || !t) return C->end;
	if(t->last)t->last->next=t->next;
	else C->begin=t->next;/*if begin is deleted, update begin*/
	t->next->last=t->last;
	List_iterator   temp=t->next;
	RECYCLE(t);/*recycle node*/
	return temp;/*return the next iterator*/
}
