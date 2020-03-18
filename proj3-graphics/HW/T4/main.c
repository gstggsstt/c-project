#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>

int Cnt_char;
char S[11000],*start=S,*cur=S;

void Move(double x,double y)
{
	double X=GetCurrentX(),Y=GetCurrentY();
	double tx=GetWindowWidth()/2+x,ty=GetWindowHeight()/2+y;
	if(fabs(X-tx)>1e-6 || fabs(Y-ty)>1e-6)
		MovePen(tx,ty);
}

void Line(double lx,double ly,double rx,double ry)
{
	Move(lx,ly);
	DrawLine(rx-lx,ry-ly); 
}

void Rect(double x,double y,double w,double h)
{
	Line(x-w/2,y-h/2,x+w/2,y-h/2);
	Line(x+w/2,y-h/2,x+w/2,y+h/2);
	Line(x+w/2,y+h/2,x-w/2,y+h/2);
	Line(x-w/2,y+h/2,x-w/2,y-h/2);
}

double Curpos()
{
	char str[30];
	memcpy(str,start,cur-start);
	str[cur-start]=0;
	return TextStringWidth(str)-1.8;
}

void Timer(int k)
{
	static double l=0;
	if(k==1)
	{
		bool fl=GetEraseMode();
		if(fl)l=Curpos();
		SetEraseMode(!fl);
		Line(l,-0.1,l,0.1);
	}
}

void Repaint()
{
	char str[30];
	SetEraseMode(TRUE);
	StartFilledRegion(1);
	Rect(0,0,3.9,0.5);
	EndFilledRegion();
	SetEraseMode(FALSE);
	Move(-1.8,-0.1);
	memcpy(str,start,24);
	str[24]=' ';
	str[25]=0;
	DrawTextString(str);
	if(GetEraseMode()) Timer(1);
	else Timer(1),Timer(1);
}

void Insert(char* str,char c)
{
	int len=strlen(str),i;
	for(i=len+1;i>0;--i)
		str[i]=str[i-1];
	str[0]=c;
	return ;
}

void Char(char c)
{
	if(c=='\b')
	{
		if(cur==S) return ;
		strcpy(cur-1,cur); cur--;
		if(cur<=start) start=(start==S?S:start-1);
		Repaint(); return ;
	}
	if(c=='\n' || c=='\r')
	{
		InitConsole();
		printf("%s\n",S);
		FreeConsole();
	}
	Insert(cur,c);
	cur++;
	if(cur-start>24)start++;
	Repaint();
}

void Key(int key,int event)
{
	switch(event)
	{
		case KEY_DOWN:
			switch(key)
			{
				case VK_DELETE:
					if(*cur==0)break;
					strcpy(cur,cur+1);
					Repaint();
					break;
				case VK_LEFT:
					if(cur==S)break;
					cur--;
					if(cur<=start) { start=(start==S?S:start-1); Repaint(); }
					if(GetEraseMode()) Timer(1);
					else Timer(1),Timer(1);
					break;
				case VK_RIGHT:
					if(*cur==0)break;
					cur++;
					if(cur-start>=24) { start++; Repaint(); }
					if(GetEraseMode()) Timer(1);
					else Timer(1),Timer(1);
					break;
				default:;
			} break;
		default:;
	}
}

void Main()
{
	InitGraphics();
	Rect(0,0,4,0.6);
	Move(-1.8,-0.1);
	SetFont("Courier New");
	SetPointSize(24);
	registerKeyboardEvent(Key);
	registerTimerEvent(Timer);
	registerCharEvent(Char);
	startTimer(1,500);
}
