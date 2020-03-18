/*
	 Fourier Number Theory Transformation
 */
#pragma once
#include <stdio.h>
#include "vector.h"

const int __MOD_NTT=998244353;
/* 998244353 = 7 * 17 * 2^23, and it is prime.
	therefore, the result mustn't longer than 8388608 elements. */

const int __pmrt_NTT=3;
/* Primitive Root */

int __n_NTT,__m_NTT,__N_NTT;

vector __R_NTT;

void __swap_int_NTT(int *x,int *y)
{ int temp=*y; *y=*x; *x=temp; }

void  __Expand_NTT(const int temp)
{
	int L=0; /*__expand_NTT __N_NTT to 2^k*/
	for(__N_NTT=1;__N_NTT<=temp;__N_NTT<<=1,L++); int i;
	__R_NTT=New_vector(); Resize(__R_NTT,__N_NTT);
	for(i=0;i<__N_NTT;++i)
		__R_NTT->data[i]=__R_NTT->data[i>>1]>>1|(i&1)<<(L-1);
}

int __ntt_pow(const int _a,int _b)
{
	int base=_a,__r_NTT=1;
	while(_b)
	{
	if(_b&1)__r_NTT=1LL*__r_NTT*base%__MOD_NTT;
	base=1LL*base*base%__MOD_NTT;
	_b>>=1;
	} return __r_NTT;
}

void   __FFT_NTT(vector A,const int f)
{
	int i,j;
	for(i=0;i<__N_NTT;++i) if(i<__R_NTT->data[i]) /*butterfly transform*/
	__swap_int_NTT(&A->data[i],&A->data[__R_NTT->data[i]]);
	for(i=1;i<__N_NTT;i<<=1)
	{
	int wn=__ntt_pow(__ntt_pow(__pmrt_NTT,(__MOD_NTT-1)/(i<<1)),f);
	for(j=0;j<__N_NTT;j+=i<<1)
	{
		int w=1,k;
		for(k=0;k<i;++k,w=1LL*w*wn%__MOD_NTT)
		{
		int x=A->data[j+k],y=1LL*A->data[i+j+k]*w%__MOD_NTT;
		A->data[j+k]=(x+y)%__MOD_NTT; A->data[i+j+k]=(x-y+__MOD_NTT)%__MOD_NTT;
	} } }return ;
}

void FFT_mul(vector __r_NTT,vector u,vector v)
{
	vector A=Cpy_new_vector(u),B=Cpy_new_vector(v);

	__n_NTT=A->size,__m_NTT=B->size;
	Resize(__r_NTT,__n_NTT+__m_NTT-1);

	__Expand_NTT(__n_NTT+__m_NTT);
	Resize(A,__N_NTT); Resize(B,__N_NTT);

	__FFT_NTT(A,1); __FFT_NTT(B,1);
	int i;
	for(i=0;i<=__N_NTT;++i) A->data[i]=1LL*A->data[i]*B->data[i]%__MOD_NTT;
	__FFT_NTT(A,__MOD_NTT-2); int base=__ntt_pow(__N_NTT,__MOD_NTT-2);
	for(i=0;i<__n_NTT+__m_NTT-1;++i)
	__r_NTT->data[i]=1LL*A->data[i]*base%__MOD_NTT;
	Free_vector(A); Free_vector(B); Free_vector(__R_NTT);
}
