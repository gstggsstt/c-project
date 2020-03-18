#include <stdio.h>
#include <time.h>
#include <string.h>

void Quick_sort(int * begin,int * end)
{
     if(end<=begin) return;
     int i=0,j=end-begin-1;
	 int t=i+((j-i)>>1);
	 int temp; temp=begin[t]; begin[t]=begin[0];
     while(i<j)
     {
               while(i<j && begin[j]>temp) j--;
               if(i<j) begin[i]=begin[j]; 
               while(i<j && begin[i]<=temp) i++;
               if(i<j) begin[j]=begin[i];
     }
     begin[i]=temp;
     Quick_sort(begin,begin+i);
     Quick_sort(begin+i+1,end);
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
		Quick_sort(a+1,a+n+1);
		tot+=clock()-t0;
	}
	for(i=1;i<=n;++i) printf("%d ",a[i]);
	printf("\n");
	FILE* out=fopen("res","a");
	fprintf(out,"Quick_sort:\t%g s.\n",1.0*tot/CLOCKS_PER_SEC);
	return 0;
}
