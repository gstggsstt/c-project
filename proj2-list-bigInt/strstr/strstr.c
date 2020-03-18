#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "List.h"

int main()
{
	int len1,len2,i,j;
	char str1[1100],str2[1100];
	List L1=New_list(),L2=New_list(),L3=New_list();

	scanf("%s%s",str1,str2);
	len1=strlen(str1);
	len2=strlen(str2);

	for(i=len1-1,j=len2-1;i>=0 && j>=0 && str1[i]==str2[j];--i,--j)
		Insert_before(L3,L3->begin,str1[i]);

	if(str1[i+1])printf("str1[%d]=str2[%d]=%c\n",i+1,j+1,str1[i+1]);
	else printf("No common suffix.\n");

	for(;i>=0;--i) Insert_before(L1,L1->begin,str1[i]);
	for(;j>=0;--j) Insert_before(L2,L2->begin,str2[j]);

	L1->end->last->next=L3->begin;
	L2->end->last->next=L3->begin;
	L1->end=L2->end=L3->end;
	RECYCLE(L1->end);
	RECYCLE(L2->end);

	List_iterator it;
	for(it=L1->begin;it!=L1->end;it=it->next)
		printf("%c",it->data);
	printf("\n");
	for(it=L2->begin;it!=L2->end;it=it->next)
		printf("%c",it->data);
	printf("\n");

	Free_all();
	system("pause");
	return 0;
}
