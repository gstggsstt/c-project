#include <stdio.h>
#include <time.h>
#include <string.h>

void swap(int * a,int * b) { int temp=*a;*a=*b,*b=temp; }

void Insertion_sort(int* begin,int* end)
{
	int i,j,temp,n=end-begin;
	for(i=1;i<n;++i)
	{
		temp=begin[i];
		for(j=i-1;begin[j]>temp && j>=0;--j)
			begin[j+1]=begin[j];
		begin[j+1]=temp;
	} return ;
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
		Insertion_sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Insertion_sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
