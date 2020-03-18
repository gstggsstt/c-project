#include <stdio.h>
#include <time.h>
#include <string.h>

void swap(int * _a,int * _b) { int temp=*_a;*_a=*_b,*_b=temp; }

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
				else if(begin[i]<begin[i<<1|1] && begin[i<<1]<=begin[i<<1|1])
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
			else if(begin[i]<begin[i<<1|1] && begin[i<<1]<=begin[i<<1|1])
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

void Heap_Sort(int* begin,int* end)
{
	Make_Heap(begin,end);
	int n=end-begin,i;
	for(i=0;i<n-1;++i)
		Pop_Heap(begin,end-i);
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
		Heap_Sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Heap_Sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
