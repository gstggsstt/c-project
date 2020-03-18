#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "prnt_sudoku.h"

struct PII { int x,y; };

/*Sudoku save in array Map, index counts from 0.*/
int	Map[9][9],cnt;
/*F[i][j][k] refers to that, in grid (i,j), number k is used if F[i][j][k]!=0*/
int	F[9][9][10];
/*characters on the left and right of output-numbers.*/
/*this will make user know which numbers are given at first.*/
char	C1[9][9],C2[9][9];

/*Mark Row x and Column y,and the 3*3 grid that number 'data' has been used.*/
void	Update(const int x,const int y,const int data)
{
	int i,px=x/3,py=y/3;
	for(i=0;i<9;++i)
	{
		F[x][i][data]++;/*Row*/
		F[i][y][data]++;/*Column*/
		F[px*3+i/3][py*3+i%3][data]++;/*Block*/
	}
}

/*Undo 'Update'.*/
void	Undo_update(const int x,const int y,const int data)
{
	int i,px=x/3,py=y/3;
	for(i=0;i<9;++i)
	{
		F[x][i][data]--;/*Row*/
		F[i][y][data]--;/*Column*/
		F[px*3+i/3][py*3+i%3][data]--;/*Block*/
	}
}

/*Returns a pair of int (x,y) referring to the next grid should fill.*/ 
struct PII Get_next()
{
	int i,j,k,Min=0x7fffffff;
	struct PII pos;
	for(i=0;i<9;++i)
		for(j=0;j<9;++j)
		{
			if(Map[i][j])continue;
			int cc=0;/*to count how many numbers are not used*/
			for(k=1;k<=9;++k)
				if(!F[i][j][k])cc++;
			if(cc==0) { pos.x=-1; return pos; }
			if(cc<Min)Min=cc,pos.x=i,pos.y=j;
			/*Choose the grid which has least numbers can use.*/
		}
	if(Min!=0x7fffffff) return pos;
	pos.x=-1; return pos;/*No grids can fill in*/
}

int	Dfs(const int rest)
{
	if(rest==0)
	{
		printf("\n\n\tSolution #%d:  \n\n",++cnt);
		Print(Map,C1,C2);
		/*too many solutions.*/
		if(cnt==5000) return 1;
		return 0;
	}
	struct PII pos=Get_next();
	if(pos.x==-1) return 0;
	int i,list[11],top=0;
	/*find numbers not used, and save then in 'list'.*/
	for(i=1;i<=9;++i)
		if(!F[pos.x][pos.y][i]) list[top++]=i;
	for(i=0;i<top;++i)
	{
		/*Mark that number 'i' has been used*/
		Map[pos.x][pos.y]=list[i];
		Update(pos.x,pos.y,list[i]);
		
		if(Dfs(rest-1)) return 1;
		/*if Dfs retruns 1, there would be too many solutions*/
		/*stop searching and return*/
		
		/*Undo*/
		Undo_update(pos.x,pos.y,list[i]);
		Map[pos.x][pos.y]=0;
	}
	return 0;
}

int main()
{
	int op,T=0;
	system("cls");
	printf("Choose input/output way(1.Keyboard/2.File):  ");
	while(1)/*until receiving an expected input.*/
	{
		scanf("%d",&op);
		if(op==1)break;
		if(op==2)
		{
			char File_Name[110];
			printf("Please input File_Name:  ");
			scanf("%s",File_Name);
			printf("\n\tResult Will Save to Result.txt\n\n");
			printf("\tCalculating....\n\n");
			freopen(File_Name,"r",stdin);
			freopen("Result.txt","w",stdout);
			break;
		}
	}

	while(1)/*Support multi-set test, read to EOF.*/
	{
		memset(F,0,sizeof(F));
		memset(Map,0,sizeof(Map));
		cnt=0;
		if(op==1)printf("Please Input 9*9 matrix(0 for space):\n");
		int i,j,data;
		
		/*Read until EOF*/
		for(i=0;i<9;++i) for(j=0;j<9;++j)
		{
			if(!(~scanf("%1d",&data)))
			/*Only ~(-1)==0 while EOF==-1*/
			{
				if(op==1)system("pause");
				fclose(stdin);
				return 0;
			}
			Map[i][j]=data;
		}

		int rest=81;/*Update array 'F'. */
		for(i=0;i<9;++i) for(j=0;j<9;++j)
			if(Map[i][j]) Update(i,j,Map[i][j]),rest--;

		printf("Test Case #%d: ",++T);

		/*Check if the input is corret.*/
		int f=0,k;
		for(i=0;i<9;++i)
		{
			for(j=0;j<9;++j)
			{
				if(Map[i][j] && F[i][j][Map[i][j]]>3) { f=1; break; }
				if(Map[i][j])continue;
				int cc=0;/*Count the numbers can use in (i,j)*/
				for(k=1;k<=9;++k)
					if(!F[i][j][k])cc++;
				if(cc==0) { f=1; break; }
			}
			if(j!=9) break;
		}

		/*Input error*/
		if(f)
		{
			fprintf(stderr,"\n\t\tError in Test Case #%d: ",T);
			fprintf(stderr,"Invalid Input.\n");
			printf("\n\n\tFiled: No Solution Found!\n\n");
			continue;
		}

		/*if a number is known, it will be output as [x]*/
		/*otherwise, there will not be the square brackets, spaces instead.*/
		for(i=0;i<9;++i)
			for(j=0;j<9;++j)
				if(Map[i][j])C1[i][j]='[',C2[i][j]=']';
				else C1[i][j]=C2[i][j]=' ';

		int t1=clock();/*Timer*/
		if(Dfs(rest))
			/*In case of costing too much time and disk storage,*/
			/*Dfs(int,int) will find at most 5000 kinds of solution.*/
		{
			fprintf(stderr,"\n\t\tError in Test Case #%d: ",T);
			fprintf(stderr,"Too much solution! Calculation has broken.\n");
		}

		if(cnt==0) printf("\n\n\tFiled: No Solution Found!\n\n");
		else printf("\n\n\t%d Solution Found In %ldms.\n\n",cnt,1000*(clock()-t1)/CLOCKS_PER_SEC);
	}

	return 0;
}


