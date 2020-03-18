#include <stdio.h>
#include "prefix_trie.h"

int main()
{
	FILE* in=fopen("Dict.txt","r");;
	char str[100],mean[100];
	Prefix_Trie T=New_Prefix_Trie();
	while(~fscanf(in,"%s",str))
	{
		fgets(mean,99,in);
		Insert(T,str,mean);
	}
	while(1)
	{
		printf("Please input your word:  ");
		scanf("%s",str);
		if(strcmp(str,"q")==0 || strcmp(str,"Q")==0) break;
		char* ch=Find(T,str);
		if(ch) printf("%s",ch);
		else printf("Not Found!\n");
	}
	Free_Prefix_Trie(T);
	Free_all();
	return 0;
}
