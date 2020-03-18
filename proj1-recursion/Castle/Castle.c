#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Header contains the outout function*/
#include "prnt_route.h"

int n,m;
int visited[1100][1100];
struct PII  from[1100][1100];
/*Save the route*/

char    Map[1100][1100];

int Bfs(const int Sx,const int Sy, const int Tx,const int Ty)
{
    struct PII Q[(n+5)*(m+5)];/*Queue for Bfs*/
    int front=0,back=0;/*head and tail pointer of the queue*/
    memset(visited,0,sizeof(visited));
    memset(from,0,sizeof(from));
    Q[back++]=make_pair(Sx,Sy);/*push the starting point*/ 
    visited[Sx][Sy]=1;
    while(front!=back && !visited[Tx][Ty])
    {
        struct PII t=Q[front++];/*Pop out*/
        /*Search four directions*/
        if(t.x>0 && !visited[t.x-1][t.y] && Map[t.x-1][t.y]!='#')/*up*/
        {
            Q[back++]=make_pair(t.x-1,t.y);/*push*/
            visited[t.x-1][t.y]=1;
            from[t.x-1][t.y]=make_pair(t.x,t.y);
        }
        if(t.x<n-1 && !visited[t.x+1][t.y] && Map[t.x+1][t.y]!='#')/*down*/
        {
            Q[back++]=make_pair(t.x+1,t.y);/*push*/
            visited[t.x+1][t.y]=1;
            from[t.x+1][t.y]=make_pair(t.x,t.y);
        }
        if(t.y>0 && !visited[t.x][t.y-1] && Map[t.x][t.y-1]!='#')/*left*/
        {
            Q[back++]=make_pair(t.x,t.y-1);/*push*/
            visited[t.x][t.y-1]=1;
            from[t.x][t.y-1]=make_pair(t.x,t.y);
        }
        if(t.y<m-1 && !visited[t.x][t.y+1] && Map[t.x][t.y+1]!='#')/*right*/
        {
            Q[back++]=make_pair(t.x,t.y+1);/*push*/
            visited[t.x][t.y+1]=1;
            from[t.x][t.y+1]=make_pair(t.x,t.y);
        }
    }
    return visited[Tx][Ty];
}

int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int i,j,Sx,Sy,Tx,Ty;
    scanf("%d%d",&n,&m);
    Sx=Sy=1,Tx=n-2,Ty=m-2;
    for(i=0;i<n;++i) scanf("%s",Map[i]);
    
    /*Find the starting and ending points*/
    for(i=0;i<n;++i)
        for(j=0;j<m;++j)
            if(Map[i][j]=='S') Sx=i,Sy=j;
            else if(Map[i][j]=='T') Tx=i,Ty=j;
            
    /*Bfs will return 1 if route is found.*/
    if(Bfs(Sx,Sy,Tx,Ty)) Print_Ans(Sx,Sy,Tx,Ty);
    else printf("No Solution.\n");
    return 0;
}
