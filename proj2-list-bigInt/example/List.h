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

/*You can define __tp_list as any type which has operator = */
typedef int __tp_list;
const __tp_list __INIT_DATA_LIST=0;

/*struct of list node*/
struct List_base
{
	__tp_list data;
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
List_base*  __Memlist_LIST[31];
List_base** __Tshlist_LIST[31];

int __cntm_LIST,__cntt_LIST,__topm_LIST,__topt_LIST,__Inited_LIST;
int __Sizelist_LIST[31];

void  Free_all()
{
	int i;/*free the memory-pool*/
	for(i=0;i<=__cntm_LIST;++i)
		free(__Memlist_LIST[i]),free(__Tshlist_LIST[i]);
	__Inited_LIST=0; return ;
}

void    Init_allocator()
{
	/*allocate a memory pool, size of which is 1*/
	__Sizelist_LIST[0]=1;
	__cntm_LIST=__cntt_LIST=0;
	__topm_LIST=__topt_LIST=-1;
	__Memlist_LIST[__cntm_LIST]=(List_base*)
		malloc(sizeof(List_base)*__Sizelist_LIST[__cntm_LIST]);
	__Tshlist_LIST[__cntm_LIST]=(List_base**)
		malloc(sizeof(List_base)*__Sizelist_LIST[__cntm_LIST]);
}

List_base*  Get(List_base* t,__tp_list x)
{
	/*initialize a node*/
	t->data=x,t->next=t->last=0;return t;
}

List_base*  ALLOC(__tp_list x)
{
	if(!__Inited_LIST) Init_allocator(),__Inited_LIST=1;
	/*recycle trash nodes first*/
	if(__topt_LIST>=0)
		return Get(__Tshlist_LIST[__cntt_LIST][__topt_LIST--],x);
		/*current stack is empty, use next stack*/
	else if(__cntt_LIST!=0)
	{
		__cntt_LIST--,__topt_LIST=__Sizelist_LIST[__cntt_LIST]-1;
		return Get(__Tshlist_LIST[__cntt_LIST][__topt_LIST--],x);
	}
		/*allocate a node from the memory-pool*/
	if(__topm_LIST<__Sizelist_LIST[__cntm_LIST]-1)
		return Get(&__Memlist_LIST[__cntm_LIST][++__topm_LIST],x);
		/*expand the memory-pool*/
	__cntm_LIST++;
	__Sizelist_LIST[__cntm_LIST]=__Sizelist_LIST[__cntm_LIST-1]<<1;
	__Memlist_LIST[__cntm_LIST]=(List_base*)
		malloc(sizeof(List_base)*__Sizelist_LIST[__cntm_LIST]);
	__Tshlist_LIST[__cntm_LIST]=(List_base**)
		malloc(sizeof(List_base)*__Sizelist_LIST[__cntm_LIST]);
	__topm_LIST=-1;
	return Get(&__Memlist_LIST[__cntm_LIST][++__topm_LIST],x);
}

void    RECYCLE(List_base* t)
{
	/*RECYCLE a node and push it to stack of trash*/
	if(__topt_LIST<__Sizelist_LIST[__cntt_LIST]-1)
	{
		__Tshlist_LIST[__cntt_LIST][++__topt_LIST]=t;
		return ;
	}
		/*if the stack is not enough, use the next stack*/
	__cntt_LIST++; __topt_LIST=-1;
	__Tshlist_LIST[__cntt_LIST][++__topt_LIST]=t;
	return ;
}
/*-------------------------ALLOCATOR-END--------------------------*/

List New_list()
{
	/*Returns a new list*/
	List C=(List)malloc(sizeof(struct List_));
	C->begin=C->end=ALLOC(__INIT_DATA_LIST); return C;
	/*List->end is a virtual node which help us to use*/
}

void    Insert_before(List C,List_iterator t,const __tp_list x)
{
	List_iterator temp=t->last;
		t->last=ALLOC(x);/*new node*/
	t->last->next=t;
	t->last->last=temp;
	if(temp)temp->next=t->last;
	else C->begin=t->last;
}

void  Free_list(List C)
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
	List_iterator temp=t->next;
		RECYCLE(t);/*recycle node*/
	return temp;/*return the next iterator*/
}
