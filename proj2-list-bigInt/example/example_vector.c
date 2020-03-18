#include <stdio.h>
#include "vector.h"

int main()
{
	int n,x,i;
	vector vec=New_vector();

	scanf("%d",&n);
	for(i=1;i<=n;++i) scanf("%d",&x), Push_back(vec,x);

	for(i=0;i<vec->size;++i)
		printf("%d ",vec->data[i]);
	printf("\n");

	Pop_back(vec);
	Pop_back(vec);
	Pop_back(vec);

	vector_iterator it;
	for(it=vec->begin;it!=vec->end;++it)
		printf("%d ",*it);
	printf("\n");

	Free_vector(vec);
	
	return 0;
}
