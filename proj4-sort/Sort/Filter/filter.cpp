#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

const int MOD=1000000007;
const int base=127;

struct PII_ { int first; int second; };
typedef PII_ PII;

struct person
{
	char name[110];
	int h,w;
}p[110000];

int n,m;
PII A[110000],B[110000],C[110000],cnta[110000],cntb[110000],cntc[110000];

PII make_pair(int temp1,int temp2) { return (PII){temp1,temp2}; }

int cmp(PII temp1,PII temp2) { return temp1.first<temp2.first; }

PII* lower_bound(PII* first,PII* last,PII val)
{
	PII* it;
	int count,step;
	count = last-first;
	while (count>0)
	{
		it = first; step=count>>1; it+=step;
		if (cmp(*it,val)) { first=++it; count-=step+1; }
		else count=step;
	} return first;
}

PII* upper_bound(PII* first,PII* last,PII val)
{
	PII* it;
	int count,step;
	count = last-first;
	while (count>0)
	{
		it = first; step=count>>1; it+=step;
		if (!cmp(val,*it)) { first=++it; count-=step+1; }
		else count=step;
	} return first;
}

void Merge_Sort_(PII* begin,PII* end,PII* temp)
{
	int l=0,r=end-begin;
	int mid=l+((r-l)>>1);
	PII *_A=begin,*_B=temp;
	if(l==r-1) return ;
	Merge_Sort_(begin,begin+mid,temp);
	Merge_Sort_(begin+mid,end,temp);
	int i,j,k;
	for(i=0,j=mid,k=0;i<mid && j<r;)
	{
		if(!cmp(_A[j],_A[i])) _B[k++]=_A[i++];
		else _B[k++]=_A[j++];
	}
	if(i<mid) memcpy(_B+k,_A+i,sizeof(PII)*(mid-i));
	if(j<r)   memcpy(_B+k,_A+j,sizeof(PII)*(r-j));
	memcpy(_A,_B,sizeof(PII)*r);
	return ;
}

void Merge_Sort(PII* begin,PII* end)
{
	PII* temp=(PII*)malloc(sizeof(PII)*(end-begin));
	Merge_Sort_(begin,end,temp);
	free(temp); return ;
}

void Init()
{
	Merge_Sort(A+1,A+n+1);
	Merge_Sort(B+1,B+n+1);
	Merge_Sort(C+1,C+n+1);
	int i; for(i=1;i<=n;++i)
	{
		if(A[i].first!=A[i-1].first)cnta[i].first=cnta[i-1].first+1;
		else cnta[i].first=cnta[i-1].first;
		if(B[i].first!=B[i-1].first)cntb[i].first=cntb[i-1].first+1;
		else cntb[i].first=cntb[i-1].first;
		if(C[i].first!=C[i-1].first)cntc[i].first=cntc[i-1].first+1;
		else cntc[i].first=cntc[i-1].first;
		cnta[i].second=A[i].second;
		cntb[i].second=B[i].second;
		cntc[i].second=C[i].second;
	}
}

int Hash(const char* str)
{
	long long temp=0; int i,len=strlen(str);
	for(i=0;i<len;++i)
	{
		temp=temp*base%MOD;
		temp=(temp+str[i])%MOD;
	} return (int)temp;
}

int main()
{
	int i,x,y;
	char op[110],str[110];
	scanf("%d%d",&n,&m);
	for(i=1;i<=n;++i)
	{
		scanf("%s%d%d",p[i].name,&p[i].h,&p[i].w);
		A[i]=make_pair(Hash(p[i].name),i);
		B[i]=make_pair(p[i].h,i);
		C[i]=make_pair(p[i].w,i);
	}
	Init(); PII *temp1,*temp2;
	for(i=1;i<=m;++i)
	{
		scanf("%s",op);
		if(!strcmp(op,"f"))
		{
			scanf("%s",str);
			temp1=lower_bound(A+1,A+n+1,make_pair(Hash(str),0));
			temp2=upper_bound(A+1,A+n+1,make_pair(Hash(str),0));
			if(temp1==temp2) printf("Not Found!\n");
			else for(;temp1<temp2;++temp1)
			{
				int tt=(*temp1).second;
				printf("%s: %d %d\n",p[tt].name,p[tt].h,p[tt].w);
			}
			printf("\n");
		}
		if(!strcmp(op,"gh"))
		{
			scanf("%d%d",&x,&y);
			temp1=lower_bound(B+1,B+n+1,make_pair(x,0));
			temp2=upper_bound(B+1,B+n+1,make_pair(y,0));
			if(temp1==temp2) printf("Not Found!\n");
			else for(;temp1<temp2;++temp1)
			{
				int tt=(*temp1).second;
				printf("%s: %d %d\n",p[tt].name,p[tt].h,p[tt].w);
			}
			printf("\n");
		}
		if(!strcmp(op,"gw"))
		{
			scanf("%d%d",&x,&y);
			temp1=lower_bound(C+1,C+n+1,make_pair(x,0));
			temp2=upper_bound(C+1,C+n+1,make_pair(y,0));
			if(temp1==temp2) printf("Not Found!\n");
			else for(;temp1<temp2;++temp1)
			{
				int tt=(*temp1).second;
				printf("%s: %d %d\n",p[tt].name,p[tt].h,p[tt].w);
			}
			printf("\n");
		}
		if(!strcmp(op,"qh"))
		{
			scanf("%d%d",&x,&y);
			temp1=lower_bound(cntb+1,cntb+n+1,make_pair(x,0));
			temp2=upper_bound(cntb+1,cntb+n+1,make_pair(y,0));
			if(temp1==temp2) printf("Not Found!\n");
			else for(;temp1<temp2;++temp1)
			{
				int tt=(*temp1).second;
				printf("RANK#%d: %s %d %d\n",(*temp1).first,p[tt].name,p[tt].h,p[tt].w);
			}
			printf("\n");
		}
		if(!strcmp(op,"qw"))
		{
			scanf("%d%d",&x,&y);
			temp1=lower_bound(cntc+1,cntc+n+1,make_pair(x,0));
			temp2=upper_bound(cntc+1,cntc+n+1,make_pair(y,0));
			if(temp1==temp2) printf("Not Found!\n");
			else for(;temp1<temp2;++temp1)
			{
				int tt=(*temp1).second;
				printf("RANK#%d: %s %d %d\n",(*temp1).first,p[tt].name,p[tt].h,p[tt].w);
			}
			printf("\n");
		}
	}
	return 0;
}
