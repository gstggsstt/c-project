#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "ntt.h"

#define DIGITS 10

struct BigN_ { vector vec; int sig; };
typedef struct BigN_* BigN;

/*-------------these functions vailid for users---------------*/

/*Get a new BigN, 0 default.*/
BigN New_BigN();
/*Copy a BigN to a new space, you may use in this way:
	  Free_BigN(x);
	  x=Cpy_new_BigN(y);
  instead of using 
	  x=y;
 */
BigN Cpy_new_BigN(BigN v);
/*Free the memory of BigN*/
void Free_BigN(BigN r);

/*These two functions can initialize a BigN with int or char*. */
void Set_str(BigN r,const char* str);
void Set_int(BigN r,const int x);

/*Calculating functions, r=u?v */
void Add(BigN r,BigN u,BigN v);
void Minus(BigN r,BigN u, BigN v);
void Mul(BigN r,BigN u,BigN v);/*ntt-mul*/
void Div(BigN r,BigN u,BigN v);/*this function is very slow*/
void BigN_pow(BigN r,BigN a,int b);/*quick pow function.*/

/*Print BigN to stdout*/
void BigN_print(BigN r);

/*------------------------------------------------------------*/
/*Do NOT use these functions directly.*/
int __is_zero_BigN(BigN r);
int __Cmp_BigN(BigN u,BigN v);
void __Go_up_BigN(BigN r);
void __Update_length_BigN(BigN r);
void __positive_add_BigN(BigN r,BigN tu,BigN tv);
void __positive_minus_BigN(BigN r,BigN tu,BigN tv);
void __positive_mul_BigN(BigN r,BigN tu,BigN tv);
void __positive_div_BigN(BigN r,BigN tu,BigN tv);

int __is_zero_BigN(BigN r)
{
	if(r->vec->size==1 && r->vec->data[0]==0) return 1;
	return 0;
}

void Free_BigN(BigN r)
{
	Free_vector(r->vec);
	free(r); return ;
}

BigN New_BigN()
{
	BigN t;
	t=(BigN)malloc(sizeof(struct BigN_));
	t->vec=New_vector();
	Push_back(t->vec,0);
	t->sig=0; return t;
}

BigN Cpy_new_BigN(BigN v)
{
	BigN u=New_BigN();
	Free_vector(u->vec);
	u->vec=Cpy_new_vector(v->vec);
	u->sig=v->sig;
	return u;
}

void __Update_length_BigN(BigN r)
{
	while(!r->vec->data[r->vec->size-1] && r->vec->size!=1)
		Pop_back(r->vec);
	return ;
}

void Set_str(BigN r,const char* str)
{
	int _n=strlen(str),data=0,base=1,t=0,i;
	r->sig=0;
	Free_vector(r->vec); r->vec=New_vector();
	if(str[0]=='-') r->sig=1,t=1;
	for(i=_n-1;i>=t;--i)
	{
		data=data+(str[i]-48)*base;
		/*if((_n-i)%3==0)*/
		{
			Push_back(r->vec,data);
			data=0,base=1; continue;
		}
		base*=10;
	}

	Push_back(r->vec,data);
	__Update_length_BigN(r);
}

void Set_int(BigN r,const int x)
{
	char str[10];
	sprintf(str,"%d",x);
	Set_str(r,str);
}

int __Cmp_BigN(BigN u,BigN v)
{
	if(u->vec->size>v->vec->size) return 1;
	if(u->vec->size<v->vec->size) return -1;
	int i;
	for(i=u->vec->size-1;i>=0;--i)
		if(u->vec->data[i]>v->vec->data[i]) return 1;
		else if(u->vec->data[i]<v->vec->data[i]) return -1;
	return 0;
}

void __positive_add_BigN(BigN r,BigN tu,BigN tv)
{
	BigN u=Cpy_new_BigN(tu),v=Cpy_new_BigN(tv);
	int _longer_vec;
	Free_vector(r->vec);
	r->vec=New_vector();
	if(u->vec->size>v->vec->size) _longer_vec=1;
	else _longer_vec=0;

	int i;
	if(_longer_vec)
	{
		Resize(r->vec,u->vec->size+1);
		for(i=0;i<v->vec->size;++i)
		{
			r->vec->data[i+1]=(r->vec->data[i]+u->vec->data[i]+
					v->vec->data[i])/DIGITS;
			r->vec->data[i]=(r->vec->data[i]+u->vec->data[i]+
					v->vec->data[i])%DIGITS;
		}
		for(i=v->vec->size;i<u->vec->size;++i)
		{
			r->vec->data[i+1]=(r->vec->data[i]+u->vec->data[i])/DIGITS;
			r->vec->data[i]=(r->vec->data[i]+u->vec->data[i])%DIGITS;
		}
	}
	else
	{
		Resize(r->vec,v->vec->size+1);
		for(i=0;i<u->vec->size;++i)
		{
			r->vec->data[i+1]=(r->vec->data[i]+u->vec->data[i]+
					v->vec->data[i])/DIGITS;
			r->vec->data[i]=(r->vec->data[i]+u->vec->data[i]+
					v->vec->data[i])%DIGITS;
		}
		for(i=u->vec->size;i<v->vec->size;++i)
		{
			r->vec->data[i+1]=(r->vec->data[i]+v->vec->data[i])/DIGITS;
			r->vec->data[i]=(r->vec->data[i]+v->vec->data[i])%DIGITS;
		}
	}
	__Update_length_BigN(r);
	Free_BigN(u);
	Free_BigN(v);
	return ;
}

void __positive_minus_BigN(BigN r,BigN tu,BigN tv)
{
	BigN u=Cpy_new_BigN(tu),v=Cpy_new_BigN(tv);
	Free_vector(r->vec);
	r->vec=New_vector();
	Resize(r->vec,u->vec->size);
	int i;
	for(i=0;i<v->vec->size;++i)
	{
		if(u->vec->data[i]+r->vec->data[i]<v->vec->data[i])
			r->vec->data[i]+=DIGITS+u->vec->data[i]-v->vec->data[i],
			r->vec->data[i+1]--;
		else r->vec->data[i]+=u->vec->data[i]-v->vec->data[i];
	}
	for(i=v->vec->size;i<u->vec->size;++i)
		r->vec->data[i]+=u->vec->data[i];
	__Update_length_BigN(r);
	Free_BigN(u);
	Free_BigN(v);
	return ;
}

void __Go_up_BigN(BigN r)
{
	int i,t=r->vec->size;
	for(i=0;i<t-1;++i)
	{
		r->vec->data[i+1]+=r->vec->data[i]/DIGITS;
		r->vec->data[i]%=DIGITS;
	}
	while(r->vec->data[t-1]>=DIGITS)
		Push_back(r->vec,r->vec->data[t-1]/DIGITS),
			r->vec->data[t-1]%=DIGITS,t++;
	return ;
}

void __positive_mul_BigN(BigN r,BigN tu,BigN tv)
{
	BigN u=Cpy_new_BigN(tu),v=Cpy_new_BigN(tv);
	FFT_mul(r->vec,u->vec,v->vec);
	__Update_length_BigN(r);
	__Go_up_BigN(r);
	Free_BigN(u);
	Free_BigN(v);
}

void __positive_div_BigN(BigN r,BigN tu,BigN tv)
{
	if(__is_zero_BigN(tv)) return ;
	BigN u=Cpy_new_BigN(tu),v=Cpy_new_BigN(tv);
	Free_vector(r->vec);
	r->vec=New_vector();
	Resize(r->vec,u->vec->size);
	BigN rest=New_BigN(),ten=New_BigN();
	Set_int(ten,10);
	int i;
	for(i=u->vec->size-1;i>=0;--i)
	{
		Mul(rest,rest,ten);
		rest->vec->data[0]=u->vec->data[i];
		while(__Cmp_BigN(rest,v)>=0)
		{
			Minus(rest,rest,v);
			r->vec->data[i]++;
		}
	} __Update_length_BigN(r);
	Free_BigN(u),Free_BigN(v),Free_BigN(ten),Free_BigN(rest);
}

void Add(BigN r,BigN u,BigN v)
{
	if(u->sig^v->sig)
	{
		if(u->sig) u->sig=0, Minus(r,v,u), u->sig=1;
		else v->sig=0, Minus(r,u,v), v->sig=1;
		return ;
	}
	__positive_add_BigN(r,u,v);
	r->sig=u->sig;
	return ;
}

void Minus(BigN r,BigN u, BigN v)
{
	if(u->sig^v->sig)
	{
		__positive_add_BigN(r,u,v);
		r->sig=u->sig;
		return ;
	}
	else if(__Cmp_BigN(u,v)>=0)
	{
		__positive_minus_BigN(r,u,v);
		r->sig=u->sig;
		return ;
	}
	__positive_minus_BigN(r,v,u);
	r->sig=!u->sig; return ;
}

void Mul(BigN r,BigN u,BigN v)
{
	__positive_mul_BigN(r,u,v);
	r->sig=u->sig^v->sig;
	return ;
}

void Div(BigN r,BigN u,BigN v)
{
	__positive_div_BigN(r,u,v);
	r->sig=u->sig^v->sig;
	return ;
}

void BigN_print(BigN r)
{
	if(r->sig) printf("-");
	int i;
	printf("%d",r->vec->data[r->vec->size-1]);
	for(i=r->vec->size-2;i>=0;--i)
		printf("%d",r->vec->data[i]);
	return ;
}

void BigN_file_print(BigN r,FILE* out)
{
	if(r->sig) fprintf(out,"-");
	int i;
	fprintf(out,"%d",r->vec->data[r->vec->size-1]);
	for(i=r->vec->size-2;i>=0;--i)
		fprintf(out,"%d",r->vec->data[i]);
	return ;
}

void BigN_pow(BigN r,BigN a,int b)
{
	if(b<0) return ;
	BigN base=Cpy_new_BigN(a);
	Set_int(r,1);
	while(b)
	{
		if(b&1) Mul(r,r,base);
		Mul(base,base,base);
		b>>=1;
	} Free_BigN(base); return ;
}
