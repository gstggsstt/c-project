#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include "gcalloc.h"
#include "random.h"
#include "strlib.h"
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

#define EPS (1e-6) 

#define NONE -1
#define PAINT_MOVE 1
#define PAINT_RECT 2
#define PAINT_OVAL 3
#define PAINT_BRSH 4
#define PAINT_LINE 5
#define PAINT_TEXT 6
#define PAINT_MODIFY_COLOR_R 7
#define PAINT_MODIFY_COLOR_G 8
#define PAINT_MODIFY_COLOR_B 9
#define PAINT_MODIFY_BUILTIN_COL 10
#define MOVE_SX 11
#define MOVE_SY 12
#define MOVE_TX 13
#define MOVE_TY 14
#define MOVE_LU 15
#define MOVE_RU 16
#define MOVE_LD 17
#define MOVE_RD 18
#define MOVE_OBJ 19

#define LINE 0
#define RECT 1
#define OVAL 2
#define TXTB 3

const double pica=0.5;/*constant variable for coord transform*/
const double pi=acos(-1);

/*Color struct*/
struct Color_
{
	int r,g,b;
};
typedef struct Color_ Color;

struct Shape_
{
	int type;/*Shape type*/
	double sx,sy,tx,ty;/*left up and right down cood*/
	int pensize;/*pensize or fontsize*/
	bool filled;/*whether the shape is filled*/
	char str[110],*font;/*text and font name*/
	Color col,fcol;/*outline color and filled color*/
};typedef struct Shape_ Shape;

double Min(double x,double y) { return x<y ? x:y; }
double Max(double x,double y) { return x>y ? x:y; }

/*Use base_color to set current pencolor*/
void SetColor(Color col)
{
	DefineColor("base_color",col.r/256.0,col.g/256.0,col.b/256.0);
	SetPenColor("base_color");
}

void Set_Painter_Color(Color col)
{
	SetColor(col);
}

void PaintShape(Shape *L)
{
	if(L->type==LINE)
	{
		SetColor(L->col);
		SetPenSize(L->pensize);
		double x=GetCurrentX(),y=GetCurrentY();
		if(fabs(x-L->sx*pica)>1e-6 || fabs(y-(21-L->sy)*pica)>1e-6)
			/*if the pen is not moved, do not move the pen, in order to avoid 
			 leading to assertion from function StartFilledRegion(1)*/
			MovePen(L->sx*pica,(21-L->sy)*pica);
		DrawLine((L->tx-L->sx)*pica,(L->sy-L->ty)*pica);
	}
	if(L->type==OVAL)
	{
		if(L->filled)
		{
			/*fill the region*/
			SetPenSize(1);
			SetColor(L->fcol);
			MovePen(Max(L->sx,L->tx)*pica,(21-(L->sy+L->ty)/2)*pica);
			StartFilledRegion(1);
			DrawEllipticalArc(fabs(L->sx-L->tx)/2*pica,fabs(L->sy-L->ty)/2*pica,0,360);
			EndFilledRegion();
		}
		SetPenSize(L->pensize);
		SetColor(L->col);
		MovePen(Max(L->sx,L->tx)*pica,(21-(L->sy+L->ty)/2)*pica);
		DrawEllipticalArc(fabs(L->sx-L->tx)/2*pica,fabs(L->sy-L->ty)/2*pica,0,360);
	}
	if(L->type==TXTB)
	{
		SetColor(L->col);
		SetPointSize(L->pensize);
		SetFont(L->font);
		if(L->filled)MovePen(L->sx*pica-TextStringWidth(L->str)/2,(21-L->sy)*pica);
		else MovePen(L->sx*pica,(21-L->sy)*pica);
		DrawTextString(L->str);
	}
	if(L->type==RECT)
	{
		Shape temp;
		if(L->filled)
		{
			/*fill region*/
			StartFilledRegion(1);
			PaintShape(&(Shape){LINE,L->sx,L->sy,L->sx,L->ty,1,FALSE,"",NULL,L->fcol,L->fcol});
			PaintShape(&(Shape){LINE,L->sx,L->ty,L->tx,L->ty,1,FALSE,"",NULL,L->fcol,L->fcol});
			PaintShape(&(Shape){LINE,L->tx,L->ty,L->tx,L->sy,1,FALSE,"",NULL,L->fcol,L->fcol});
			PaintShape(&(Shape){LINE,L->tx,L->sy,L->sx,L->sy,1,FALSE,"",NULL,L->fcol,L->fcol});
			EndFilledRegion();
		}
		/*draw 4 lines*/
		PaintShape(&(Shape){LINE,L->sx,L->sy,L->sx,L->ty,L->pensize,FALSE,"",NULL,L->col,L->col});
		PaintShape(&(Shape){LINE,L->sx,L->ty,L->tx,L->ty,L->pensize,FALSE,"",NULL,L->col,L->col});
		PaintShape(&(Shape){LINE,L->tx,L->ty,L->tx,L->sy,L->pensize,FALSE,"",NULL,L->col,L->col});
		PaintShape(&(Shape){LINE,L->tx,L->sy,L->sx,L->sy,L->pensize,FALSE,"",NULL,L->col,L->col});
	}
	return ;
}

double dis(double xa,double ya,double xb,double yb)
{
	return sqrt((xa-xb)*(xa-xb)+(ya-yb)*(ya-yb));
}

/*Check if (x,y) is in Rectangle L*/
int Inbox(double x,double y,Shape* L)
{
	double temp; x/=pica; y/=pica;
	if(L->sx>L->tx) temp=L->sx,L->sx=L->tx,L->tx=temp;
	if(L->sy>L->ty) temp=L->sy,L->sy=L->ty,L->ty=temp;
	if(x>=L->sx && x<=L->tx && y<=(21-L->sy) && y>=(21-L->ty)) return 1;
	return 0;
}

/*Check if (x,y) is in Oval L*/
int Inoval(double x,double y,Shape* L)
{
	x/=pica; y=21-y/pica;
	double temp;
	if(L->sx>L->tx) temp=L->sx,L->sx=L->tx,L->tx=temp;
	if(L->sy>L->ty) temp=L->sy,L->sy=L->ty,L->ty=temp;
	double ax=L->sx,ay=L->sy,bx=L->tx,by=L->ty;
	double cx=(ax+bx)/2,cy=(ay+by)/2;
	double eps=(x-cx)*(x-cx)/(ax-bx)/(ax-bx)*4+(y-cy)*(y-cy)/(ay-by)/(ay-by)*4;
	if(eps<=1+EPS) return 1;
	return 0;
}

/*Check if dist between (x,y) and line-L is less than 0.1*/
int Online(double x,double y,Shape* L)
{
	double xa=x/pica,ya=21-y/pica;
	double xb=L->sx,xc=L->tx;
	double yb=L->sy,yc=L->ty;
	double xAB=xb-xa,yAB=yb-ya;
	double xAC=xc-xa,yAC=yc-ya;
	double xBC=xc-xb,yBC=yc-yb;
	double r=0.1;
	double d=fabs(xAB*yAC-yAB*xAC)/dis(xb,yb,xc,yc);
	if(d>r) return 0;
	double dot1=-xAB*xBC-yAB*yBC;
	double dot2=xAC*xBC+yAC*yBC;
	if(dot1>=0 && dot2>=0) return 1;
	if(dis(xa,ya,xb,yb)<r || dis(xa,ya,xc,yc)<r) return 1;
	return 0;
}

/*Check if dist between (x,y) and four lines is less 0.1*/
int Onrect(double x,double y,Shape* L)
{
	double temp; x/=pica; y=21-y/pica;
	if(L->sx>L->tx) temp=L->sx,L->sx=L->tx,L->tx=temp;
	if(L->sy>L->ty) temp=L->sy,L->sy=L->ty,L->ty=temp;
	if(x>=L->sx-0.1 && x<=L->tx+0.1 && y>=L->sy-0.1 && y<=L->sy+0.1) return 1;
	if(x>=L->sx-0.1 && x<=L->tx+0.1 && y>=L->ty-0.1 && y<=L->ty+0.1) return 1;
	if(x>=L->sx-0.1 && x<=L->sx+0.1 && y>=L->sy-0.1 && y<=L->ty+0.1) return 1;
	if(x>=L->tx-0.1 && x<=L->tx+0.1 && y>=L->sy-0.1 && y<=L->ty+0.1) return 1;
	return 0;
}

/*Check if (x,y) is in Rectangle that the textbox decides*/
int Intext(double x,double y,Shape* L)
{
	Shape temp;
	SetPointSize(L->pensize);
	SetFont(L->font);
	if(L->filled)
	{
		temp=(Shape){RECT,L->sx-TextStringWidth(L->str)/2/pica,
			L->sy-0.6,L->sx+TextStringWidth(L->str)/2/pica,L->sy};
		return Inbox(x,y,&temp);
	}
	temp=(Shape){RECT,L->sx,L->sy-0.6,L->sx+TextStringWidth(L->str)/pica,L->sy};
	return Inbox(x,y,&temp);
}

/*Check if dist between (x,y) and outline is less than 0.1*/
int Onoval(double x,double y,Shape* L)
{
	x/=pica; y=21-y/pica;
	double temp;
	if(L->sx>L->tx) temp=L->sx,L->sx=L->tx,L->tx=temp;
	if(L->sy>L->ty) temp=L->sy,L->sy=L->ty,L->ty=temp;
	double ax=L->sx,ay=L->sy,bx=L->tx,by=L->ty;
	double cx=(ax+bx)/2,cy=(ay+by)/2;
	double eps=(x-cx)*(x-cx)/(ax-bx)/(ax-bx)*4+(y-cy)*(y-cy)/(ay-by)/(ay-by)*4;
	if(eps>=0.9-EPS && eps<=1.1+EPS) return 1;
	return 0;
}

/*Check if click on (x,y) can select object L*/
int InShape(double x,double y,Shape* L)
{
	if(L->type==LINE) return Online(x,y,L);
	if(L->type==TXTB) return Intext(x,y,L);
	if(L->type==RECT && L->filled) return Onrect(x,y,L) || Inbox(x,y,L);
	if(L->type==OVAL && L->filled) return Onoval(x,y,L) || Inoval(x,y,L);
	if(L->type==RECT) return Onrect(x,y,L);
	if(L->type==OVAL) return Onoval(x,y,L);
}

