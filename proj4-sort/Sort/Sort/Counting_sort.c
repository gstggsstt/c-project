#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKS 1000100

void Counting_Sort(int* begin,int* end)
{
	int n=end-begin,i,cnt=0;
	int *temp=(int*)calloc(sizeof(int),BLOCKS);
	for(i=0;i<n;++i) temp[begin[i]]++;
	for(i=0;i<BLOCKS && cnt<n;++i)
		while(temp[i])
			begin[cnt++]=i,temp[i]--;
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
		Counting_Sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Counting_Sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
