#include <bits/stdc++.h>

#define LHEIGHT 150
#define RHEIGHT 200

#define LWEIGHT 40
#define RWEIGHT 100

using namespace std;

random_device	seed;
mt19937		RND(seed());

vector<string> vec;

string Get_name()
{
	return vec[RND()%vec.size()];
}

string Random_str()
{
	int len=RND()%10+3;
	string name;
	for(int i=1;i<=len;++i)
		name+=(char)(RND()%26+97);
	name[0]-=32;
	vec.push_back(name);
	return name;
}

string Random_str_()
{
	int len=RND()%10+3;
	string name;
	for(int i=1;i<=len;++i)
		name+=(char)(RND()%26+97);
	name[0]-=32;
	return name;
}

int Random_height()
{
	return RND()%(RHEIGHT-LHEIGHT)+LHEIGHT;
}

int Random_weight()
{
	return RND()%(RWEIGHT-LWEIGHT)+LWEIGHT;
}

int main(int argv,char ** argc)
{
	assert(argv>=3);

	int n=atoi(argc[1]),m=atoi(argc[2]);

	cout << n << ' ' << m << endl;
	for(int i=1;i<=n;++i)
		cout << Random_str() << ' ' << Random_height() << ' ' << Random_weight() << endl;
	for(int i=1;i<=m;++i)
	{
		int op=RND()%100;
		if(op<=20)
		{
			if(RND()%100<50)cout << "f " << Get_name() << endl;
			else cout << "f " << Random_str_() << endl;
		}
		else if(op<=40)
		{
			int x=Random_height();
			int y=Random_height();
			if(x>y) swap(x,y);
			cout << "gh " << x << ' ' << y << endl;
		}
		else if(op<=60)
		{
			int x=Random_weight();
			int y=Random_weight();
			if(x>y) swap(x,y);
			cout << "gw " << x << ' ' << y << endl;
		}
		else if(op<=80)
		{
			int x=RND()%min(n,RHEIGHT-LHEIGHT)+1;
			int y=RND()%min(n,RHEIGHT-LHEIGHT)+1;
			if(x>y) swap(x,y);
			cout << "qh " << x << ' ' << y << endl;
		}
		else 
		{
			int x=RND()%min(n,RWEIGHT-LWEIGHT)+1;
			int y=RND()%min(n,RWEIGHT-LWEIGHT)+1;
			if(x>y) swap(x,y);
			cout << "qw " << x << ' ' << y << endl;
		}
	}
	return 0;
}
