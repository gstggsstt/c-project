#include <stdio.h>

int L[1100],top;

void	Print(const int N)
{
	printf("%d=",N); int i;
	for(i=1;i<top;++i) printf("%d+",L[i]);
	printf("%d\n",L[top]);
	return ;
}

void	Dfs(const int S,const int last,const int depth,const int lim,const int N)
{
	if(depth==lim+1 && !S) { Print(N); return ; }
	int i; for(i=last;i<=S;++i)
		L[++top]=i,Dfs(S-i,i,depth+1,lim,N),top--;
	return ;
}

void	partition_x(const int n)
{ int i; for(i=1;i<=n;++i) Dfs(n,1,1,i,n); return ; }

int main()
{
	int n;
	while(~scanf("%d",&n))
		partition_x(n);
	return 0;
}

