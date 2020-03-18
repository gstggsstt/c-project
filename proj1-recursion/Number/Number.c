#include <stdio.h>

int L[1100],top;

void	Print(const int N)
{
	printf("%d=",N); int i;
	for(i=1;i<top;++i) printf("%d+",L[i]);
	printf("%d\n",L[top]);
	return ;
}

void	Dfs(const int S,const int last,const int N)
{
	if(S==0) { Print(N); return ; } int i;
	for(i=last;i<=S;++i)
		L[++top]=i,Dfs(S-i,i,N),top--;
	return ;
}

void	partition_x(const int n)
{
	Dfs(n,1,n);
	return ;
}

int main()
{
	int n;
	while(~scanf("%d",&n))
		partition_x(n);
	return 0;
}

