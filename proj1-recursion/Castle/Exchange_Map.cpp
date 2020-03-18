#include <bits/stdc++.h>

int n,m;
char	Map[1100][1100];

using namespace std;

int main()
{
	freopen("in.txt","r",stdin);
	freopen("view.txt","w",stdout);
	scanf("%d%d",&n,&m);
	for(int i=0;i<n;++i)
		scanf("%s",Map[i]);
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			if(Map[i][j]=='#') printf("¨€");
			else if(Map[i][j]=='.') printf("  ");
			else printf("¡î");
		}
		printf("\n");
	}
	return 0;
}
