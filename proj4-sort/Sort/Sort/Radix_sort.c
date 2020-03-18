#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void swap(int * a,int * b) { int temp=*a;*a=*b,*b=temp; }

int T[10][1100000],cnt[10],cnta;

void Radix_sort(int* begin,int* end)
{
	int i,j;
	int n=end-begin,*A=begin;
	long long k;
	for(k=1;k<=1000000000;k*=10)
	{
		memset(cnt,0,sizeof(cnt)); cnta=0;
		for(i=0;i<n;++i) T[A[i]/k%10][cnt[A[i]/k%10]++]=A[i];
		for(i=0;i<=9;++i) for(j=0;j<cnt[i];++j) A[cnta++]=T[i][j];
	}
	return ;
}

int a[1100000],b[1100000];

int main()
{
	int i,n,tot=0,T_=100;
	scanf("%d",&n);
	for(i=1;i<=n;++i) scanf("%d",&a[i]);
	memcpy(b,a,sizeof(int)*(n+1));
	while(T_--)
	{
		memcpy(a,b,sizeof(int)*(n+1));
		int t0=clock();
		Radix_sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Radix_sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
