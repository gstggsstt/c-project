#include <stdio.h>
#include <time.h>
#include <string.h>

void swap(int * a,int * b) { int temp=*a;*a=*b,*b=temp; }

void Bubble_Sort(int* begin,int* end)
{
	int n=end-begin,i,j;
	for(i=1;i<n;++i) for(j=0;j<n-i;++j)
		if(begin[j]>begin[j+1])swap(&begin[j],&begin[j+1]);
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
		Bubble_Sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Bubble_Sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
