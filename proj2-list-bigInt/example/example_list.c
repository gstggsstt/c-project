#include <stdio.h>
#include "List.h"

int main()
{
	int x;
	List L=New_list();
	while(1)
	{
		scanf("%d",&x);
		if(x==-1)break;
		Insert_before(L,L->end,x);
	}
	List_iterator it;

	for(it=L->begin;it!=L->end;it=it->next)
		printf("%d ",it->data);
	printf("\n");

	for(it=L->begin;it!=L->end;it=it->next)
		if(it->data%2==0) Delete(L,it);

	for(it=L->begin;it!=L->end;it=it->next)
		printf("%d ",it->data);
	printf("\n");

	for(it=L->end->last;it;it=it->last)
		printf("%d ",it->data);
	printf("\n");

	Free_all();

	return 0;
}

	
