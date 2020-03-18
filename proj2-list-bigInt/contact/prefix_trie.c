#include <stdio.h>
#include "prefix_trie.h"

/*Print the whole tree*/
void PP(Prefix_Trie_iterator t,char* str,int step,FILE* out)
{
	if(!t) return ; int i;
	if(t->leaf)
	{
			vector_iterator it;
			for(it=t->vec->begin;it!=t->vec->end;++it)
				fprintf(out,"%s%s\n",str,*it);
	}
	for(i=0;i<256;++i)
	{
		str[step]=i;
		str[step+1]=0;
		PP(t->s[i],str,step+1,out);
	} return ;
}

void	Print_tree(Prefix_Trie T,FILE* out)
{
	char str[1000];	
	if(out==stdout)
	{
		printf("\tDo you want to print the whole contact?(Y/n)");
		char op[3]; scanf("%s",op);
		if(op[0]!='y' && op[0]!='Y') return ;
	}
	PP(T->root,str,0,out);
	if(out!=stdout) fclose(out);
}

void Clear_node_(Prefix_Trie_iterator t,const char* str)
{
	if(!t) return ;
	if(str[0]==0) { t->leaf=0; return ; }
	Clear_node_(t->s[(unsigned char)str[0]],str+1); return ;
}

void Clear_node(Prefix_Trie T,char* str)
{ Clear_node_(T->root,str); }

int main()
{
	int x;
	char op[100],name[100],str[100];
	Prefix_Trie T=New_Prefix_Trie();
	printf("Import Contacts: ");
	scanf("%s",str);
	FILE *in=fopen(str,"r");
	if(in==NULL)
	{
		printf("\tFile <%s> doesn't exist.\n",str);
		system("pause");
		return 0;
	}
	printf("Importing ...\n");
	while(~fscanf(in,"%s",name))
	{
		fgets(str,99,in);
		int l=strlen(str);
		if(str[l-1]=='\n' || str[l-1]=='\r')str[l-1]=0;
		Insert(T,name,str);
	} fclose(in);
	printf("Done.\n");
	while(1)
	{
		printf(">>>");
		scanf("%s",op);
		if(op[0]=='F' || op[0]=='f')
		{
			scanf("%s",str);
			vector v=Find(T,str);
			if(v)
			{
				vector_iterator it;
				for(it=v->begin;it!=v->end;++it)
					printf("\t%2d %s\n",it-v->begin+1,*it);
			}
			else printf("\tNot Found!\n");
		}
		else if(op[0]=='i' || op[0]=='I')
		{
			scanf("%s",name);
			fgets(str,99,stdin);
			int l=strlen(str);
			if(str[l-1]=='\n' || str[l-1]=='\r')str[l-1]=0;
			Insert(T,name,str);
			vector v=Find(T,name);
			vector_iterator it;
			for(it=v->begin;it!=v->end;++it)
				printf("\t%2d %s\n",it-v->begin+1,*it);
		}
		else if(op[0]=='D' || op[0]=='d')
		{
			scanf("%s",str);
			vector v=Find(T,str);
			scanf("%d",&x); x--;
			if(v && x>=0 && x<v->size)
			{
				char* temp=v->data[x];
				v->data[x]=v->data[v->size-1];
				v->data[v->size-1]=temp;

				Pop_back(v);
				vector_iterator it;
				for(it=v->begin;it!=v->end;++it)
					printf("\t%2d %s\n",it-v->begin+1,*it);
				if(v->size==0) Clear_node(T,str);
			}	
			else if(v && x==-1)
			{
				while(v->size) Pop_back(v);
				Clear_node(T,str);
			}
			else printf("\tNot found!\n");
		}
		else if(op[0]=='P' || op[0]=='p') Print_tree(T,stdout);
		else if(op[0]=='E' || op[0]=='e')
		{
			scanf("%s",str);
			Print_tree(T,fopen(str,"w"));
		}
		else if(op[0]=='Q' || op[0]=='q')break;
		else if(op[0]=='H' || op[0]=='h')
		{
			printf("\ti <name> <info>\n");
			printf("\t  Insert a piece of info to person <name>.\n");
			printf("\td <name> <num>\n");
			printf("\t  Delete <num>-th info of <name>, if num==0,"
					"\n\t\t<name> will be removed.\n");
			printf("\tf <name>\n");
			printf("\t  Find <name> in the contacts.\n");
			printf("\te <file_name>\n");
			printf("\t  Export contacts to <file_name>.\n");
			printf("\tp\n");
			printf("\t  Print the whole contacts.\n");
			printf("\tq\n");
			printf("\t  Quit.\n");
		}

		else printf("Unknown command: %s\nUse command 'h' to get help.\n",op);
	}
	Free_Prefix_Trie(T);
	Free_all();
	return 0;
}
