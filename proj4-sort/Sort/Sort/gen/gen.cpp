#include <bits/stdc++.h>

using namespace std;

int main(int argv,char** argc)
{
	assert(argv>=5);
	int n=atoi(argc[1]),begin=atoi(argc[2]),end=atoi(argc[3]);
	int seed=atoi(argc[4]);
	if(seed==-1) seed=time(0);
	mt19937	RND(seed);
	for(int i=1;i<=n;++i)
		cout << RND()%(end-begin+1)+begin << ' ';
	return 0;
}
