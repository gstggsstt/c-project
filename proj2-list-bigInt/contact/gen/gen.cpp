#include <bits/stdc++.h>

using namespace std;

char a[100];

int main()
{
	freopen("test.txt","w",stdout);
	mt19937 RND(time(0));
	for(a[0]='a';a[0]<='z';++a[0])
	for(a[1]='a';a[1]<='z';++a[1])
	for(a[2]='a';a[2]<='z';++a[2])
	{
		for(int i=1;i<=5;++i)
		{
			printf("%s ",a);
			int t=RND()%30+1;
			for(int k=1;k<=t;++k)
				printf("%c",RND()%95+32);
			printf("\n");
		}
	}
	return 0;
}
