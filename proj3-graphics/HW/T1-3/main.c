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
double Angle=0;

void Line(double lx,double ly,double rx,double ry)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w/4; cy=h*3.0/4;
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
	cx=w/4; cy=h*3.0/4;
	MovePen(cx+lx+r,cy+ly);
	DrawArc(r,0,360.0);
}

void Moveto1(double x,double y)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w*3/4; cy=h*3.0/4;
	MovePen(cx+x,cy+y);
}

void Moveto2(double x,double y)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w/4; cy=h/4;
	MovePen(cx+x,cy+y);
}

void Moveto3(double x,double y)
{
	double h,w,cx,cy;
	w=GetWindowWidth();
	h=GetWindowHeight();
	cx=w*3/4; cy=h/4;
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

void Set_color(char * str,double pp)
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
	DefineColor(str,r,g,b);
}

void Painter(int xx, int yy, int button, int event)
{
	double x=ScaleXInches(xx);
	double y=ScaleYInches(yy);
	static int f=0;
	static double lx=0,ly=0;
	switch(event)
	{
		case BUTTON_DOWN:
			switch(button)
			{
				case LEFT_BUTTON:
					f=1;
					break;
				default: break;
			}
			lx=x,ly=y;
			break;
		case MOUSEMOVE:
			if(f)Line_(lx,ly,x,y);
			lx=x,ly=y;
			break;
		case BUTTON_UP:
			switch(button)
			{
				case LEFT_BUTTON:
					f=0;
					break;
				default: break;
			}
			break;
		default:break;
	}
	return ;			
}

void Work_1()
{
	DefineColor("DarkBlue",14/255.0,84/255.0,188/255.0);
	DefineColor("DarkGreen",13/255.0,140/255.0,0/255.0);
	SetPenColor("DarkBlue");
	SetPenSize(2);
	Line(0.5/1.5,0/1.5,0.5/1.5,-1/1.5);
	Line(0.5/1.5,0/1.5,1/1.5,0/1.5);
	Line(1/1.5,0/1.5,1/1.5,-1/1.5);
	Line(0/1.5,-0.5/1.5,0/1.5,0.5/1.5);
	Line(0/1.5,-0.5/1.5,-1/1.5,-0.5/1.5);
	Line(-1/1.5,0.5/1.5,-1/1.5,-0.5/1.5);
	Line(-1/1.5,0.5/1.5,0/1.5,0.5/1.5);
	Line(-0.5/1.5,-0.5/1.5,-0.5/1.5,0.5/1.5);
	Line(-1/1.5,0/1.5,0/1.5,0/1.5);
	SetPenSize(3);
	Circle(0.875/1.5,-0.5/1.5,0.01/1.5);
	SetPenColor("DarkGreen");
	SetPenSize(2);
	Line(-1/1.5,4.0/3/1.5,-1/1.5,2/1.5);
	Line(-0.5/1.5,5.0/3/1.5,-0.5/1.5,2/1.5);
	Line(-1.2/1.5,2/1.5,-0.3/1.5,2/1.5);
	Line(-1.2/1.5,2/1.5,-1.2/1.5,2.25/1.5);
	Line(-0.3/1.5,2/1.5,-0.3/1.5,2.25/1.5);
	Line(-1.2/1.5,2.25/1.5,-0.3/1.5,2.25/1.5);
	SetPenColor("Black");
	SetPenSize(3);
	Line(-1.5/1.5,-1/1.5,+1.5/1.5,-1/1.5);
	Line(-1.5/1.5,+1/1.5,+1.5/1.5,+1/1.5);
	Line(-1.5/1.5,-1/1.5,-1.5/1.5,+1/1.5);
	Line(+1.5/1.5,-1/1.5,+1.5/1.5,+1/1.5);
	Line(0/1.5,2/1.5,-1.5/1.5,1/1.5);
	Line(0/1.5,2/1.5,1.5/1.5,1/1.5);
	return ;
}

void Work_2()
{
	SetPenSize(2);
	Moveto1(0,0); Angle=90; int i;

	char str[500][20];
	for(i=0;i<20;++i)
	{
		strcpy(str[i],"color_cir_01");
		sprintf(str[i]+10,"%02d",i);
		Set_color(str[i],i/20.0);
	}
	for(i=1;i<=20;++i)
	{
		SetPenColor(str[i-1]);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-80);
	}
	
	SetPenSize(2);
	Moveto2(0,0); Angle=90;
	for(i=0;i<40;++i)
	{
		strcpy(str[i],"color_cir_01");
		sprintf(str[i]+10,"%02d",i);
		Set_color(str[i],i/40.0);
	}
	for(i=1;i<=40;++i)
	{
		SetPenColor(str[i-1]);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-60);
		Forward(0.6);Turn(-70);
	}
	
	SetPenSize(1);
	Moveto3(0,0); Angle=90;
	for(i=0;i<90;++i)
	{
		strcpy(str[i],"color_cir_001");
		sprintf(str[i]+10,"%03d",i);
		Set_color(str[i],i/90.0);
	}
	for(i=1;i<=90;++i)
	{
		SetPenColor(str[i-1]);
		Forward(0.8);Turn(-60);
		Forward(0.8);Turn(-60);
		Forward(0.8);Turn(-60);
		Forward(0.8);Turn(-60);
		Forward(0.8);Turn(-60);
		Forward(0.8);Turn(-64);
	}
	return ;
}

void Work_3()
{
	SetPenSize(1);
	SetPenColor("Black");
	registerMouseEvent(Painter);
}

void Main()
{
	InitGraphics();
	Work_1();
	Work_2();
	Work_3(); 
}
