#include <stdio.h>
#include <time.h>
#include "List.h"
#include "BigN.h"

int main()
{
	int b;
	char a[1100],X[1100];

	List    L=New_list();
	struct Pair temp;

	BigN u=New_BigN(),v=New_BigN();
	BigN Ans=New_BigN();

	printf("Input x:\n\t"); scanf("%s",X);
	printf("Input a,b(a*x^b,end with -1 -1):\n");

	while(1)
	{
		printf("\t"); scanf("%s%d",a,&b); if(b==-1)break;
		if(b<0) { printf("b should not less than 0\n"); continue; }

		Set_str(u,a);
		temp.a=Cpy_new_BigN(u);
		temp.b=b;
		Insert_before(L,L->end,temp);
	}
	
	printf("Output to file?(Y/n) ");
	char ch[10];
	scanf("%s",ch);
	if(ch[0]=='y' || ch[0]=='Y') freopen("Result.txt","w",stdout);

	int t1=clock();
	List_iterator i;
	for(i=L->begin;i!=L->end;i=i->next)
	{
		Set_str(v,X);
		BigN_pow(v,v,i->data.b);
		Mul(i->data.a,i->data.a,v);
		Add(Ans,Ans,i->data.a);
		Free_BigN(i->data.a);
	}
	int t2=clock();

	printf("Result = "); BigN_print(Ans); printf("\n");
	printf("Length = %d\n",Ans->vec->size*3-
	(Ans->vec->data[Ans->vec->size-1]<100)-(Ans->vec->data[Ans->vec->size-1]<10));
	printf("Time   = %dms\n",1000*(t2-t1)/CLOCKS_PER_SEC);

	Free_list(L); Free_all();
	Free_BigN(Ans); Free_BigN(u); Free_BigN(v);

	if(ch[0]!='y' && ch[0]!='Y')system("pause");
	else fclose(stdout);

	return 0;
}
