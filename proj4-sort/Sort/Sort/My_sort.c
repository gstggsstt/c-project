#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define THRESHOLD 17

int log_2(int n) { int k; for(k=0;n>1;n>>=1)++k; return k; }

inline void swap(int * _a,int * _b) { int temp=*_a;*_a=*_b,*_b=temp; }

inline void round_swap(int* temp1,int* temp2,int* temp3)
{ int temp=*temp1; *temp1=*temp2; *temp2=*temp3; *temp3=temp; return ; }

void Make_Heap(int* begin,int* end)
{
	int n=end-begin,i,j; begin--;
	for(j=n>>1;j>=1;--j)
	{
		i=j;
		while(i<<1<=n)
		{
			if((i<<1|1)<=n)
			{
				if(begin[i]<begin[i<<1] && begin[i<<1]>=begin[i<<1|1])
					swap(&begin[i],&begin[i<<1]),i<<=1;
				else if(begin[i]<begin[i<<1|1] && begin[i<<1|1]>=begin[i<<1])
					swap(&begin[i],&begin[i<<1|1]),i=i<<1|1;
				else break;
			}
			else
			{
				if(begin[i]<begin[i<<1]) swap(&begin[i],&begin[i<<1]),i<<=1;
				else break;
			}
		}
	} return ;
}

void Pop_Heap(int* begin,int* end)
{
	swap(begin,end-1);
	int n=end-begin-1,i=1; begin--;
	while(i<<1<=n)
	{
		if((i<<1|1)<=n)
		{
			if(begin[i]<begin[i<<1] && begin[i<<1]>=begin[i<<1|1])
				swap(&begin[i],&begin[i<<1]),i<<=1;
			else if(begin[i]<begin[i<<1|1] && begin[i<<1|1]>=begin[i<<1])
				swap(&begin[i],&begin[i<<1|1]),i=i<<1|1;
			else break;
		}
		else
		{
			if(begin[i]<begin[i<<1]) swap(&begin[i],&begin[i<<1]),i<<=1;
			else break;
		}
	} return ;
}

void Insertion_sort(int* begin,int* end)
{
	int *i,*j,temp;
	for(i=begin+1;i<end;++i)
	{
		if(*i<*begin)
		{
			temp=*i;
			for(j=i-1;j>=begin;--j) *(j+1)=*j;
			*(j+1)=temp;
		}
		else
		{
			temp=*i;
			for(j=i-1;*j>temp;--j) *(j+1)=*j;
			*(j+1)=temp;
		}
	} return ;
}

void Heap_Sort(int* begin,int* end)
{
	Make_Heap(begin,end);
	int n=end-begin,i;
	for(i=0;i<n-(THRESHOLD>>1);++i)
		Pop_Heap(begin,end-i);
	return ;
}

void Sort_2_elements(int* temp1,int* temp2)
{
	if(*temp1>*temp2) swap(temp1,temp2);
	return ;
}

void Sort_3_elements(int* temp1,int* temp2,int* temp3)
{
	if(*temp1<=*temp2 && *temp2<=*temp3) return ;
	else if(*temp1<=*temp3 && *temp3<=*temp2) swap(temp2,temp3);
	else if(*temp2<=*temp1 && *temp1<=*temp3) swap(temp1,temp2);
	else if(*temp3<=*temp2 && *temp2<=*temp1) swap(temp1,temp3);
	else if(*temp2<=*temp3 && *temp3<=*temp1) round_swap(temp3,temp1,temp2);
	else if(*temp3<=*temp1 && *temp1<=*temp2) round_swap(temp1,temp3,temp2);
	return ;
}

void Quick_sort(int * begin,int * end,int lim)
{
	while(begin<end)
	{
		if(end-begin<THRESHOLD) { return ; }
		if(lim==0) { Heap_Sort(begin,end); return ; }
		int i=0,j=end-begin-1;
		int t=(i+j)>>1;
		Sort_3_elements(&begin[0],&begin[t],&begin[j]);
		int temp; temp=begin[t];
		while(1)
		{
			while(begin[j]>temp) j--;
			while(begin[i]<temp) i++;
			if(i>=j) break ;
			swap(&begin[i],&begin[j]);
			i++; j--;
		}
		lim--;
		Quick_sort(begin+i,end,lim);
		end=begin+i;
	}
}

void Sort(int* begin,int* end)
{
	int n=end-begin;
	int lg=log_2(n);
	Quick_sort(begin,end,lg<<1);
	Insertion_sort(begin,end);
	return ;
}

int a[1100000],b[1100000];

int main()
{
	int i,n,tot=0,T=100;
	scanf("%d",&n);
	for(i=1;i<=n;++i) scanf("%d",&a[i]);
	memcpy(b,a,sizeof(int)*(n+1));
	while(T--)
	{
		memcpy(a,b,sizeof(int)*(n+1));
		int t0=clock();
		Sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Intro_sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
