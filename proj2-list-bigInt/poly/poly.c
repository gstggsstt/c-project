#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "List.h"
#include "BigN.h"
#include "prefix_trie.h"

void Print_help()
{
	printf("DEFINE AND MODIFY VARIABLES (I)\n");
	printf("\tset <var> or s <var>\n");
	printf("\t\tDefine and initialize a variable.\n");
	printf("\tcopy <var1> <var2> or cp <var1> <var2>\n");
	printf("\t\tDefine var2 if it is not defined, and copy var1 to var2.\n");
	printf("\tadd_to <var> or m <var>\n");
	printf("\t\tModify var, add several constant terms to var.\n");
	printf("\n");

	printf("PRINT AND DELETE VARIABLES (II)\n");
	printf("\tprint <var> or p <var>\n");
	printf("\t\tPrint terms in var.\n");
	printf("\tsize <var>\n");
	printf("\t\tGet the number of terms in var.\n");
	printf("\tclear or ca\n");
	printf("\t\tClear all variables, and reset the calculator.\n");
	printf("\n");

	printf("POLYNOMIALS' OPERATORS (III)\n");
	printf("\tadd/minus/mul <var1> <var2>\n");
	printf("\t\tCalculate var1+var2, var1-var2, var1*var2, ");
	printf("and the result is in built-in variable 'ans'.\n");
	printf("\tcalc <var> or cc <var>\n");
	printf("\t\tCalculate the value of polynomial var.\n");
	printf("\tdiff <var>\n");
	printf("\t\tCalcualte the differential coefficient of var.\n");
	printf("\n");

	printf("ORTHER (IV)\n");
	printf("\tcls\n");
	printf("\t\tClear the screen.\n");
	printf("\thelp or h\n");
	printf("\t\tGet help.\n");
	printf("\texit or q\n");
	printf("\t\tQuilt the calculator.\n");
	printf("\n");
}

/*quick_sort*/
void Quick_Sort(Pair* a,const int l,const int r)
{
     if(l>=r) return;
     int i=l,j=r,t=rand()%(r-l+1)+l;
	 Pair temp;
     temp=a[t]; a[t]=a[l];
     while(i<j)
     {
               while(i<j && a[j].b>temp.b) j--;
               if(i<j) a[i]=a[j]; 
               while(i<j && a[i].b<=temp.b) i++;
               if(i<j) a[j]=a[i];
     }
     a[i]=temp;
     Quick_Sort(a,l,i-1);
     Quick_Sort(a,i+1,r);
}

/*sort_and_merge*/
void Simplify_list(List a)
{
	int i,j;
	Pair *v=(Pair*)malloc(sizeof(Pair)*(a->size+1));

	for(i=0;a->begin!=a->end;Delete(a,a->begin))
		v[i++]=a->begin->data;

	Quick_Sort(v,0,i-1);

	v[i].b=-1;
	for(j=0;j<i;++j)
	{
		if(v[j].b==v[j+1].b)
		{
			Add(v[j+1].a,v[j].a,v[j+1].a);
			Free_BigN(v[j].a);
		}
		else if(!(v[j].a->vec->data[0]==0 && v[j].a->vec->size==1))
			Insert_before(a,a->end,v[j]);
	}

	free(v); return ;
}

/*chang num to -num*/
void	Flip_sig(List a)
{
	List_iterator it;
	for(it=a->begin;it!=a->end;it=it->next)
		it->data.a->sig^=1;
	return ;
}

/*check if str is proper*/
int	Check(const char *str)
{
	if(strcmp(str,"ans")==0) return 0;
	if(!(str[0]>='a' && str[0]<='z') && !(str[0]>='A' && str[0]<='Z')
			&& str[0]!='_') return 0;
	int i,len=strlen(str);
	for(i=0;i<len;++i)
		if(!(str[i]>='0' && str[i]<='9') && !(str[i]>='a' && str[i]<='z') 
			&& !(str[i]>='A' && str[i]<='Z') && str[i]!='_') return 0;
	return 1;
}

void	Add_poly(List Ans,List a,List b)
{
	List L1=Cpy_new_list(a),L2=Cpy_new_list(b);
	List_iterator it;
	for(it=L1->begin;it!=L1->end;it=it->next)
	{
		Pair temp;
		temp.a=Cpy_new_BigN(it->data.a);
		temp.b=it->data.b;
		Insert_before(Ans,Ans->end,temp);
	}
	for(it=L2->begin;it!=L2->end;it=it->next)
	{
		Pair temp;
		temp.a=Cpy_new_BigN(it->data.a);
		temp.b=it->data.b;
		Insert_before(Ans,Ans->end,temp);
	}
	Destroy_List(L1); Destroy_List(L2);
	Simplify_list(Ans);
}

void	Minus_poly(List Ans,List a,List b)
{
	List L1=Cpy_new_list(a),L2=Cpy_new_list(b);
	List_iterator it;
	Flip_sig(L2);
	for(it=L1->begin;it!=L1->end;it=it->next)
	{
		Pair temp;
		temp.a=Cpy_new_BigN(it->data.a);
		temp.b=it->data.b;
		Insert_before(Ans,Ans->end,temp);
	}
	for(it=L2->begin;it!=L2->end;it=it->next)
	{
		Pair temp;
		temp.a=Cpy_new_BigN(it->data.a);
		temp.b=it->data.b;
		Insert_before(Ans,Ans->end,temp);
	}
	Destroy_List(L1); Destroy_List(L2);
	Simplify_list(Ans);
}

void	Mul_poly(List Ans,List a,List b)
{
	List L1=Cpy_new_list(a),L2=Cpy_new_list(b);
	List_iterator it,jt;
	for(it=L1->begin;it!=L1->end;it=it->next)
		for(jt=L2->begin;jt!=L2->end;jt=jt->next)
		{
			Pair temp;
			temp.a=New_BigN();
			Mul(temp.a,it->data.a,jt->data.a);
			temp.b=it->data.b+jt->data.b;
			Insert_before(Ans,Ans->end,temp);
		}
	Destroy_List(L1); Destroy_List(L2);
	Simplify_list(Ans);
}

void	Print_poly(const char *name,List L)
{
	printf("%s = [ ",name);
	if(L->begin==L->end) printf("0 ");
	else
	{
		List_iterator it;
		for(it=L->begin;it!=L->end->last;it=it->next)
		{
			if(!(it->data.a->vec->data[0]==1 && it->data.a->vec->size==1))
			{
				BigN_print(it->data.a);
				if(it->data.b>1) printf("*x^%d, ",it->data.b);
				if(it->data.b==1) printf("*x, ");
				if(it->data.b==0) printf(", ");
			}
			else 
			{
				if(it->data.b>1) printf("x^%d, ",it->data.b);
				if(it->data.b==1) printf("x, ");
				if(it->data.b==0) printf("1, ");
			}
		}
		if(!(it->data.a->vec->data[0]==1 && it->data.a->vec->size==1))
		{
			BigN_print(it->data.a);
			if(it->data.b>1) printf("*x^%d ",it->data.b);
			if(it->data.b==1) printf("*x ");
			if(it->data.b==0) printf(" ");
		}
		else 
		{
			if(it->data.b>1) printf("x^%d ",it->data.b);
			if(it->data.b==1) printf("x ");
			if(it->data.b==0) printf("1 ");
		}
	}
	printf("]\n");
}

void Calc_and_print(List L1)
{
	List L=Cpy_new_list(L1);
	FILE *out=stdout;
	char X[1100];

	BigN v=New_BigN(),u=New_BigN();
	BigN Ans=New_BigN();

	printf("Input x: "); scanf("%s",X);

	printf("Output to file?(Y/n) ");
	char ch[10];
	scanf("%s",ch);
	if(ch[0]=='y' || ch[0]=='Y')
	{
		char file_name[50];
		printf("File name: ");
		scanf("%s",file_name);
		out=fopen(file_name,"w");
	}

	int t1=clock();
	List_iterator i;
	Set_int(u,1);
	for(i=L->begin;i!=L->end;i=i->next)
	{
		Set_str(v,X);
		if(i==L->begin)BigN_pow(v,v,i->data.b);
		else BigN_pow(v,v,i->data.b-i->last->data.b);
		Mul(u,u,v);
		Mul(i->data.a,i->data.a,u);
		Add(Ans,Ans,i->data.a);
	}
	int t2=clock();

	fprintf(out,"Result = "); BigN_file_print(Ans,out); fprintf(out,"\n");
	fprintf(out,"Length = %d\n",Ans->vec->size);
	fprintf(out,"Time   = %dms\n",1000*(t2-t1)/CLOCKS_PER_SEC);

	if(ch[0]=='y' || ch[0]=='Y') fclose(out);
	Free_BigN(Ans); Free_BigN(v); Free_BigN(u);
	Destroy_List(L);
}

int main()
{
	int b;
	char a[110],op[110],name1[110],name2[110];

	Pair temp;

	BigN	u=New_BigN();
	Prefix_Trie T=New_Prefix_Trie();
	List Ans=New_list();

	Insert(T,"ans",Ans);
	
	printf("Polynomials calculator, enter 'help' to get help.\n");
	while(1)
	{
		printf(">>> ");
		scanf("%s",op);

		if(strcmp(op,"help")==0 || strcmp(op,"h")==0) Print_help();	
		else if(strcmp(op,"exit")==0 || strcmp(op,"q")==0) break;

		else if(strcmp(op,"cls")==0) { system("cls"); }
		else if(strcmp(op,"clear")==0 || strcmp(op,"ca")==0)
		{
			Free_Prefix_Trie(T); T=New_Prefix_Trie();
			Free_BigN(u); u=New_BigN();
			Ans=New_list();
			Insert(T,"ans",Ans);
		}

		else if(strcmp(op,"size")==0)
		{
			scanf("%s",name1); List L=Find(T,name1);
			if(!L) { printf("No variable '%s'.\n",name1); continue; }
			printf("size [ %s ] = %d\n",name1,L->size);
		}

		else if(strcmp(op,"print")==0 || strcmp(op,"p")==0)
		{
			scanf("%s",name1); List L=Find(T,name1);
			if(!L) { printf("No variable '%s'.\n",name1); continue; }
			Print_poly(name1,L);
		}

		else if(strcmp(op,"set")==0 || strcmp(op,"s")==0)
		{
			scanf("%s",name1);
			if(Check(name1)==0) { printf("Invailid variable.\n"); continue; }
			printf("Input a,b(a*x^b,end with -1 -1):\n");
			List L=New_list();
			while(1)
			{
				scanf("%s%d",a,&b); if(b==-1)break;
				if(b<0) { printf("b should not less than 0\n"); continue; }
				Set_str(u,a); temp.a=Cpy_new_BigN(u);
				temp.b=b; Insert_before(L,L->end,temp);
			}
			Simplify_list(L);
			Insert(T,name1,L);
		}

		else if(strcmp(op,"add_to")==0 || strcmp(op,"m")==0)
		{
			scanf("%s",name1);
			List L=Find(T,name1);
			if(!L) { printf("No variable '%s'.\n",name1); continue; }
			List L2=New_list();
			printf("Input a,b(a*x^b,end with -1 -1):\n");
			while(1)
			{
				scanf("%s%d",a,&b); if(b==-1)break;
				if(b<0) { printf("b should not less than 0\n"); continue; }
				Set_str(u,a);
				temp.a=Cpy_new_BigN(u);
				temp.b=b;
				Insert_before(L2,L2->end,temp);
			}
			Ans=New_list();
			Add_poly(Ans,L,L2);
			Destroy_List(L2);
			Insert(T,name1,Ans);
		}

		else if(strcmp(op,"copy")==0 || strcmp(op,"cp")==0)
		{
			scanf("%s%s",name2,name1);

			List L2=Find(T,name2);
			if(!L2) { printf("No variable '%s'.\n",name2); continue; }
			
			List templ=Cpy_new_list(L2);
			Insert(T,name1,templ);
		}

		else if(strcmp(op,"add")==0)
		{
			scanf("%s%s",name1,name2);

			List L1=Find(T,name1),L2=Find(T,name2);
			if(!L1) { printf("No variable '%s'.\n",name1); continue; }
			if(!L2) { printf("No variable '%s'.\n",name2); continue; }

			Ans=New_list();
			Add_poly(Ans,L1,L2);

			Insert(T,"ans",Ans);
			Print_poly("ans",Ans);
		}

		else if(strcmp(op,"minus")==0)
		{
			scanf("%s%s",name1,name2);

			List L1=Find(T,name1),L2=Find(T,name2);
			if(!L1) { printf("No variable '%s'.\n",name1); continue; }
			if(!L2) { printf("No variable '%s'.\n",name2); continue; }

			Ans=New_list();
			Minus_poly(Ans,L1,L2);

			Insert(T,"ans",Ans);
			Print_poly("ans",Ans);
		}

		else if(strcmp(op,"mul")==0)
		{
			scanf("%s%s",name1,name2);

			List L1=Find(T,name1),L2=Find(T,name2);
			if(!L1) { printf("No variable '%s'.\n",name1); continue; }
			if(!L2) { printf("No variable '%s'.\n",name2); continue; }

			Ans=New_list();
			Mul_poly(Ans,L1,L2);

			Insert(T,"ans",Ans);
			Print_poly("ans",Ans);
		}

		else if(strcmp(op,"calc")==0 || strcmp(op,"cc")==0)
		{
			scanf("%s",name1);
			List L=Find(T,name1);
			if(!L) { printf("No variable '%s'.\n",name1); continue; }
			Calc_and_print(L);
		}

		else if(strcmp(op,"diff")==0)
		{
			scanf("%s",name1);
			List L=Find(T,name1);
			if(!L) { printf("No variable '%s'.\n",name1); continue; }
			Ans=New_list();
			List_iterator it;
			for(it=L->begin;it!=L->end;it=it->next)
			{
				if(it->data.b!=0)
				{
					temp.a=New_BigN();
					Set_int(temp.a,it->data.b);
					Mul(temp.a,temp.a,it->data.a);
					temp.b=it->data.b-1;
					Insert_before(Ans,Ans->end,temp);
				}
			}
			Insert(T,"ans",Ans);;
			Print_poly("ans",Ans);
		}

		else printf("Unknown operator '%s'.\n",op);
	}
				
	return 0;
}
