#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int a[1100000],b[1100000];

void Merge_Sort(int* begin,int* end,int* temp)
{
	int l=0,r=end-begin;
	int mid=l+((r-l)>>1);
	int *A=begin,*B=temp;
	if(l==r-1) return ;
	Merge_Sort(begin,begin+mid,temp);
	Merge_Sort(begin+mid,end,temp);
	int i,j,k;
	for(i=0,j=mid,k=0;i<mid && j<r;)
	{
		if(A[i]<=A[j]) B[k++]=A[i++];
		else B[k++]=A[j++];
	}
	if(i<mid) while(i<mid)B[k++]=A[i++];
	if(j<r)   while(j<r) B[k++]=A[j++];
	for(i=l;i<r;++i) A[i]=B[i];
	return ;
}

int main()
{
	int i,n;
	scanf("%d",&n);
	for(i=1;i<=n;++i) scanf("%d",&a[i]);
	Merge_Sort(a+1,a+n+1,b);
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	return 0;
}
