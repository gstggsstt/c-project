#include <stdio.h>
#include <string.h>

/*Header which contains the output function*/
#include "prnt_route.h"

int	n,m;
int	visited[1100][1100];
struct PII	from[1100][1100];
/*Save the route.*/

const int dx[4]={-1,0,1,0},dy[4]={0,1,0,-1};

char	Map[1100][1100];

int	Dfs(const int Sx,const int Sy, const int Tx,const int Ty)
{
	if(Sx==Tx && Sy==Ty) return 1;
	int i;
	for(i=0;i<4;++i)/*Search four directions*/
	{
		int tx=Sx+dx[i],ty=Sy+dy[i];/*tx,ty refers to the next grid.*/
		if(tx>=0 && ty>=0 && tx<n && ty<m && 
			!visited[tx][ty] && Map[tx][ty]!='#')
		/*check if (tx,ty) is reachable*/
		{
			visited[tx][ty]=1;
			if(Dfs(tx,ty,Tx,Ty))
			{
				/*Route is found.*/
				from[tx][ty]=make_pair(Sx,Sy);
				return 1;
			}
		}
	}
	return 0;
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int i,j,Sx,Sy,Tx,Ty;
	scanf("%d%d",&n,&m);
	Sx=Sy=1,Tx=n-2,Ty=m-2;
	for(i=0;i<n;++i) scanf("%s",Map[i]);
	
	/*Find the starting and ending points.*/
	for(i=0;i<n;++i)
		for(j=0;j<m;++j)
			if(Map[i][j]=='S') Sx=i,Sy=j;
			else if(Map[i][j]=='T') Tx=i,Ty=j;
			
	/*Dfs will return 1 if a solution is found, 0 otherwise.*/
	if(Dfs(Sx,Sy,Tx,Ty)) Print_Ans(Sx,Sy,Tx,Ty);
	else printf("No Solution.\n");
	return 0;
}
