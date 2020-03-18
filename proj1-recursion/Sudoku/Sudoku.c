#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "prnt_sudoku.h"

/*Sudoku save in array Map, index counts from 0.*/
int	Map[9][9],cnt;
/*R<->Row  B<->Block  C<->Column*/
int	R[11][11],B[11][11],C[11][11];
/*characters on the left and right of output-numbers.*/
/*this will make user know which numbers are given at first.*/
char	C1[9][9],C2[9][9];

int	Dfs(const int x,const int y)
{
	if(x==9)
	{
		printf("\n\n\tSolution #%d:  \n\n",++cnt);
		Print(Map,C1,C2);
		/*when there are too many solutions return 1*/
		/*and the whole Search algorithm will stop*/
		if(cnt==5000) return 1;
		return 0;
	}

	if(Map[x][y]) { return Dfs(x+(y+1)/9,(y+1)%9); }
	/*if (x,y) is known, then fill the next one.*/

	int i;
	for(i=1;i<=9;++i)
	{
		/*Check if setting i in blank(x,y) is proper.*/
		if(R[x][i] || C[y][i] || B[x/3*3+y/3][i])continue;

		/*update arrays*/
		Map[x][y]=i;
		R[x][i]=1; C[y][i]=1; B[x/3*3+y/3][i]=1;

		if(Dfs(x+(y+1)/9,(y+1)%9)) return 1;
		/*this expression will fill blanks from the left-up one to */
		/*  the right-down one automaticly.*/

		/*undo*/
		R[x][i]=0; C[y][i]=0; B[x/3*3+y/3][i]=0;
		Map[x][y]=0;
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
		if(op==2)/*file input*/
		{
			char File_Name[110];
			printf("Please input File_Name:  ");
			scanf("%s",File_Name);
			printf("\n\tResult Will Save to Result.txt\n\n");
			printf("\tCalculating....\n\n");
			freopen(File_Name,"r",stdin);
			freopen("Result.txt","w",stdout);/*answer file is "Result.txt" */
			break;
		}
	}

	while(1)/*Support multi-set test, read to EOF.*/
	{
		/*Initialize*/
		memset(R,0,sizeof(R));
		memset(C,0,sizeof(C));
		memset(B,0,sizeof(B));
		memset(Map,0,sizeof(Map));
		cnt=0;
		if(op==1)printf("Please Input 9*9 matrix(0 for space):\n");
		int i,j,data;
		/*Read until EOF*/
		for(i=0;i<9;++i) for(j=0;j<9;++j)
		{
			if(!(~scanf("%1d",&data)))
			{
				if(op==1)system("pause");
				fclose(stdin);
				return 0;
			}
			Map[i][j]=data;
			R[i][data]++;
			C[j][data]++;
			B[i/3*3+j/3][data]++;
		}

		printf("Test Case #%d: ",++T);
		int f=0;/*Check if the sudoku is valid.*/
		for(i=0;i<9;++i) for(j=1;j<=9;++j)
			if(R[i][j]>1|| C[i][j]>1 || B[i][j]>1) f=1;
		if(f)/*Invalid input*/
		{
			fprintf(stderr,"\n\t\tError in Test Case #%d: ",T);
			fprintf(stderr,"Invalid Input.\n");
			printf("\n\n\tFiled: No Solution Found!\n\n");
			continue;
		}

		/*Mark the known grids.*/
		for(i=0;i<9;++i) for(j=0;j<9;++j)
				if(Map[i][j])C1[i][j]='[',C2[i][j]=']';
				else C1[i][j]=C2[i][j]=' ';

		int t1=clock();/*Timer*/
		if(Dfs(0,0))
			/*In case of costing too much time and disk storage,*/
			/*Dfs(int,int) will find at most 5000 kinds of solution.*/
		{
			fprintf(stderr,"\n\t\tError in Test Case #%d: ",T);
			fprintf(stderr,"Too much solution! Calculation has broken.\n");
		}

		/*output the time*/
		if(cnt==0) printf("\n\n\tFiled: No Solution Found!\n\n");
		else printf("\n\n\t%d Solution Found In %ldms.\n\n",cnt,1000*(clock()-t1)/CLOCKS_PER_SEC);
	}

	return 0;
}

