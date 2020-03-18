//Compile with -std=c++11
#include <bits/stdc++.h>

using namespace std;

const int RATE_KEEP_DIR=000;
const int TWIST_RATE=10;
const int RETURN_RATE=100;

int n,m,Sx,Sy,Tx,Ty,Max;
char    Map[1100][1100];
typedef pair<int,int>   PII;
const int dx[4]={-1,0,1,0},dy[4]={0,1,0,-1};
const int Dx[4]={-2,0,2,0},Dy[4]={0,2,0,-2};

mt19937     RND(time(0));

//Check if (x,y) has a neighbour can go. 
bool    Check(const int x,const int y)
{
    for(int i=0;i<4;++i)
    {
        int nx=x+dx[i],ny=y+dy[i],Nx=x+Dx[i],Ny=y+Dy[i];
        if(Nx>=0 && Nx<n && Ny>=0 && Ny<m && (Map[Nx][Ny]=='#' && Map[nx][ny]=='#'))return true;
    } return false;
}

void    Dfs(const int x,const int y,const int depth,int Lim,const int last_dir)
{
    if(depth>Max)Tx=x,Ty=y,Max=depth;//find a longest route
    if(depth>Lim) return ;
    Map[x][y]='.';
    while(Check(x,y))
    {
        int t=RND()%4;//random direction
        if(RND()%1000<RATE_KEEP_DIR) t=last_dir;//chance of keeping direction. 
        int nx=x+dx[t],ny=y+dy[t],Nx=x+Dx[t],Ny=y+Dy[t];
        if(nx<0 || nx>n-1 || ny<0 || ny>m-1 || Map[nx][ny]!='#')continue;
        if(Nx<0 || Nx>n-1 || Ny<0 || Ny>m-1 || Map[Nx][Ny]!='#')continue;
        if(Nx==0 || Nx==n-1 || Ny==0 || Ny==m-1) { Map[nx][ny]='.'; continue; }
        Map[nx][ny]='.'; Map[Nx][Ny]='.';
        Dfs(Nx,Ny,depth+1,Lim,t);
        
        //chance of returning directly, without expanding, for more branch roads 
        if((int)(RND()%1000)<(min(n,m)<100?0:RETURN_RATE)) return ;
        
        Lim=depth+max(min(n,m)/TWIST_RATE,5);
    }
    return ;
}

int main()
{
    freopen("in.txt","w",stdout);
    scanf("%d%d",&n,&m);
    printf("%d %d\n",n,m);
    for(int i=0;i<n;++i) for(int j=0;j<m;++j) Map[i][j]='#';
    Sx=Sy=1;
    // the length limit of each branch road.
    Dfs(Sx,Sy,0,max(min(n,m)/TWIST_RATE,5),2);
    //set starting and ending points.
    Map[Sx][Sy]='S';
    Map[Tx][Ty]='T';
    for(int i=0;i<n;++i) printf("%s\n",Map[i]);
    return 0;
}

