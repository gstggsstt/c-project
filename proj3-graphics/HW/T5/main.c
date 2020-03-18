#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
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

const double pi=acos(-1);
const double sq=sqrt(3);

/*----CHANGE_THIS_VAR----*/
int Tar_step=4;
/*-----------------------*/

double Angle=0;

void Line(double lx,double ly,double rx,double ry)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w/2; cy=h/2;
	MovePen(cx+lx,cy+ly);
	DrawLine(rx-lx,ry-ly);
}
void Line_(double lx,double ly,double rx,double ry)
{
	MovePen(lx,ly);
	DrawLine(rx-lx,ry-ly);
}
void Circle(double lx,double ly,double r)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w/2; cy=h/2;
	MovePen(cx+lx+r,cy+ly);
	DrawArc(r,0,360.0);
}

void Moveto(double x,double y)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w/2; cy=h/2;
	MovePen(cx+x,cy+y);
}

void Forward(double dis)
{
	double t=Angle/180*pi;
	DrawLine(dis*cos(t),dis*sin(t));
}

void Move(double dis)
{
	double x=GetCurrentX(),y=GetCurrentY();
	double t=Angle/180*pi;
	MovePen(x+dis*cos(t),y+dis*sin(t));
}

void Turn(double ang)
{
	Angle+=ang;
	if(Angle>360)Angle-=360;
	if(Angle<-360)Angle+=360;
}

void Set_color(double pp)
{/*HSB->RGB*/
	double h=pp*360/*色相*/,s=1/*饱和度*/,v=0.85/*亮度*/;
	double r,g,b,hi,f,p,q,t;
	hi=(int)(h/60)%6;
	f=h/60-hi;
	p=v*(1-s);
	q=v*(1-f*s);
	t=v*(1-(1-f)*s);
	if(hi==0) r=v,g=t,b=p;
	if(hi==1) r=q,g=v,b=p;
	if(hi==2) r=p,g=v,b=t;
	if(hi==3) r=p,g=q,b=v;
	if(hi==4) r=t,g=p,b=v;
	if(hi==5) r=v,g=p,b=q;
	DefineColor("temp",r,g,b);
	SetPenColor("temp");
}

double dis(const double sx,const double sy,const double tx,const double ty)
{
	return sqrt((sx-tx)*(sx-tx)+(sy-ty)*(sy-ty));
}

void Paint(const double sx,const double sy,
	const double tx,const double ty,const int step,
	const double col_begin,const double col_end)
{
	if(step>=Tar_step) { Set_color(col_begin); Line(sx,sy,tx,ty); return ; }
	double len=dis(sx,sy,tx,ty);
	double theta=atan((ty-sy)/(tx-sx));
	double X1,X2,X3,X4,X5;
	double Y1,Y2,Y3,Y4,Y5;
	if(sx>tx)theta+=pi;
	X1=sx; Y1=sy;
	X2=sx+(tx-sx)/3; Y2=sy+(ty-sy)/3;
	X3=X2+cos(theta+pi/3)*len/3; Y3=Y2+sin(theta+pi/3)*len/3;
	X4=sx+(tx-sx)*2/3; Y4=sy+(ty-sy)*2/3;
	X5=tx; Y5=ty;
	Paint(X1,Y1,X2,Y2,step+1,col_begin,col_begin+(col_end-col_begin)/4);
	Paint(X2,Y2,X3,Y3,step+1,col_begin+(col_end-col_begin)/4,col_begin+(col_end-col_begin)*2/4);
	Paint(X3,Y3,X4,Y4,step+1,col_begin+(col_end-col_begin)*2/4,col_begin+(col_end-col_begin)*3/4);
	Paint(X4,Y4,X5,Y5,step+1,col_begin+(col_end-col_begin)*3/4,col_begin+(col_end-col_begin)*4/4);
	return ;
}

void Main()
{
	SetWindowSize(GetFullScreenWidth(),GetFullScreenHeight());
	InitGraphics();
	InitConsole();
	printf("Rank: ");
	Tar_step=GetInteger(); 
	if(Tar_step>6)
	{
		printf("Too large!\n"),system("pause");
		ExitGraphics();
		FreeConsole();
		return ;
	}
	printf("Len: ");
	double L=GetReal();
	L/=3;
	FreeConsole();
	SetPenSize(2);
	
	Paint(0,3*L,sq*3/2*L,-1.5*L,0,0,1.0/3);
	Paint(sq*3/2*L,-1.5*L,-sq*3/2*L,-1.5*L,0,1.0/3,2.0/3);
	Paint(-sq*3/2*L,-1.5*L,0,3*L,0,2.0/3,1);
}
