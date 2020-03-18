#include <stdio.h>

int L[1100],top;

void	Print(const int N)
{
	printf("%d=",N); int i;
	for(i=1;i<top;++i) printf("%d+",L[i]);
	printf("%d\n",L[top]);
	return ;
}

int	min(const int x,const int y)
{
	return x<y?x:y;
}

void	Dfs(const int S,const int last,const int N)
{
	if(S==0) { Print(N); return ; }
	int i;
	for(i=min(S,last);i>=1;--i)
		L[++top]=i,Dfs(S-i,i,N),top--;
	return ;
}

void	partition_x(const int x)
{
	Dfs(x,x,x);
	return ;
}

int main()
{
	int n;
	while(~scanf("%d",&n))
		partition_x(n);
	return 0;
}
