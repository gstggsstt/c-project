/*Output function*/

#include <stdio.h>

struct PII { int x,y; };/*coordinate container*/
struct PII make_pair(const int x,const int y)
{ struct PII t; t.x=x,t.y=y; return t; }

extern int	n,m;
extern int	visited[1100][1100];
extern struct PII	from[1100][1100];

extern char	Map[1100][1100];

void	Print_Ans(const int Sx,const int Sy,const int Tx,const int Ty)
{
	int x=Tx,y=Ty,dir=0,td,i,j,tempx,tempy;
	/*'dir' is the current direction, while 'td' is the last direcion.*/
	while(x!=Sx || y!=Sy)
	{
		/*judge the direction*/
		if(from[x][y].y==y && from[x][y].x==x+1) Map[x][y]='^',td=0;
		if(from[x][y].y==y && from[x][y].x==x-1) Map[x][y]='V',td=1;
		if(from[x][y].x==x && from[x][y].y==y+1) Map[x][y]='<',td=2;
		if(from[x][y].x==x && from[x][y].y==y-1) Map[x][y]='>',td=3;
		/*decide which conner character should be output.*/
		if(dir==0 && td==3) Map[x][y]='}';
		if(dir==2 && td==1) Map[x][y]='}';
		if(dir==3 && td==1) Map[x][y]='{';
		if(dir==0 && td==2) Map[x][y]='{';
		if(dir==1 && td==3) Map[x][y]=']';
		if(dir==2 && td==0) Map[x][y]=']';
		if(dir==3 && td==0) Map[x][y]='[';
		if(dir==1 && td==2) Map[x][y]='[';
		tempx=x,tempy=y; dir=td;
		x=from[tempx][tempy].x;
		y=from[tempx][tempy].y;
	}
	Map[Sx][Sy]='S';
	Map[Tx][Ty]='T';

	for(i=0;i<n;++i)
	{
		for(j=0;j<m;++j)
		{
			/*output with special chars.*/
			if(Map[i][j]=='#') printf("¨€");
			else if(Map[i][j]=='.') printf("  ");
			else if(Map[i][j]=='^') printf("¡ü");
			else if(Map[i][j]=='V') printf("¡ý");
			else if(Map[i][j]=='<') printf("¡û");
			else if(Map[i][j]=='>') printf("¡ú");
			else if(Map[i][j]=='+') printf("©ï");
			else if(Map[i][j]=='[') printf("©³");
			else if(Map[i][j]==']') printf("©·");
			else if(Map[i][j]=='{') printf("©»");
			else if(Map[i][j]=='}') printf("©¿");
			else printf("¡î");
		}
		printf("\n");
	}
	return ;
}
