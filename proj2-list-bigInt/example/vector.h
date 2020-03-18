/*
	This file provides a template of Indefinite-length-array.
	You can define an vector in this way:
		vector vec=New_vector();
	
	You can access elements int these ways:
		int i;
		vector_iterator j;
		for(i=0;i<vec->size;++i)
			printf("%d ",vec->data[i]);
		for(j=vec->begin;j!=vec->end;++j)
			printf("%d ",*j);
			
	Following functions are provided:
	
		void Push_back(vector vec,const __tp_vector x);
			push element x to the back of the vector, size of vec will +1.
			
		void Pop_back(vector vec);
			delete the last element of vec, size of vec will -1.
			
		void Resize(vector vec,const int x);
			resize the vector, new memory will be allocated,
			if x<vec->size then, the rest of elements will be discarded,
			if x>vec->size, the new elements will be set to 
				__INIT_DATA_VECTOR(default 0).
			
		void Free_vector(vector vec);
			data and vec itself will be freed.

	+--------------------------------------------------------+
	|NOTICE: You may NOT copy a vector directly like this:   |
	|        vec1=vec2;                                      |
	| You SHOULD do that this way:                           |
	|   Free_vector(vec1);                                   |
	|   vec1=Cpy_new_vector(vec2);                           |
	+--------------------------------------------------------+
*/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/*You can define __tp_vector as any type which has operator = */
typedef int __tp_vector;
const __tp_vector __INIT_DATA_VECTOR=0;

typedef __tp_vector vec_base;
typedef __tp_vector* vector_iterator;

struct vector_
{
	/*vec->end is a virtual element, you may not access it.*/
	vector_iterator data;
	vector_iterator begin;
	vector_iterator end;
	int size,mx_size;
};

typedef struct vector_* vector;

/*allocate a new vector*/
vector New_vector()
{
	vector t=(vector)malloc(sizeof(struct vector_));
	t->size=0,t->mx_size=1;
	t->data=(vec_base*)malloc(sizeof(vec_base));
	t->begin=t->end=t->data;
	return t;
}

int __min_int_vector(const int x,const int y)
{ return x<y?x:y; } 

void Resize(vector v,const int x)
{
	int i,temp=v->size;
	v->mx_size=x+1;
	__tp_vector* t=(__tp_vector*)malloc(sizeof(__tp_vector)*v->mx_size);
	/*Copy data*/
	memcpy(t,v->data,
		sizeof(__tp_vector)*__min_int_vector(v->mx_size,v->size+1));
	v->size=x;
	free(v->data);
	v->data=t; v->begin=t;
	v->end=v->begin+v->size;
	for(i=temp;i<v->size;++i)
		v->data[i]=__INIT_DATA_VECTOR;
	return ;
}

void Push_back(vector v,const __tp_vector x)
{
	if(v->size<v->mx_size-1) v->data[(++v->size)-1]=x,v->end++;
	else/*memory is not enough allocate a new array double-sized.*/
	{
		v->mx_size<<=1;
		__tp_vector* t=(__tp_vector*)malloc(sizeof(__tp_vector)*v->mx_size);
		memcpy(t,v->data,sizeof(__tp_vector)*(v->size+1));
		free(v->data);
		v->data=t;
		v->data[(++v->size)-1]=x;
		v->begin=t;
		v->end=v->begin+v->size;
	} return ;
}

void Pop_back(vector v)
{
	/*delete the last element without freeing the space*/
	if(v->size==0) return ;
	v->size--; v->end--; return ;
}

void Free_vector(vector v)
{
	/*Free all memory*/
	free(v->data);
	free(v);
}

vector Cpy_new_vector(vector v)
{
	vector u=New_vector();
	Resize(u,v->size);
	memcpy(u->data,v->data,(v->size+1)*sizeof(__tp_vector));
	u->begin=u->data;
	u->end=u->data+u->size;
	return u;
}

