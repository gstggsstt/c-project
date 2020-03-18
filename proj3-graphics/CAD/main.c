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

#include "List.h"/*Declaration and definition of struct List*/
#include "Shape.h"/*Declaration and definition of struct Shape and painter of Shapes*/

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>

Shape SelectFrame[9];/*Location points*/
Shape Box[1100];/*Buttons*/
Shape Text[110];/*Text Display*/
Shape CurrentShape;/*Shape which is created each time*/
List Shape_L;/*layer List*/
List_iterator SelectedObj;/*Record the selected shape, use a list-iterator to
							locate the element in the layer-list*/
/*Flags and default values*/
int cntb,cntr,cntt,Temp_Mode=NONE,Mode=PAINT_BRSH,curpensize=2;
bool Leftbutton_fl=FALSE,Shift_fl=FALSE,FillMode=FALSE;
double Leftbutton_down_x,Leftbutton_down_y,last_x,last_y;

/*recorder of the ratio of height and width*/
double deltax;
double deltay;
/*Outline color and filled color*/
Color PenCol=(Color){0,0,0},FillCol=(Color){255,255,255};

/*Repaint the whole tools bar*/
void RepaintALL()
{
	PaintShape(&(Shape){RECT,0,0,1.1,100,2,TRUE,"",NULL,(Color){255,255,255},(Color){255,255,255}});
	int i; for(i=1;i<=cntb;++i) PaintShape(&Box[i]);
	for(i=1;i<=cntt;++i) PaintShape(&Text[i]); return ;
}

/*Init the tools bar, define buttons*/
void Init_UI()
{
	int i;
	/*MOVE button*/
	Box[++cntb]=(Shape){RECT,0.1,0.1,1.1,1.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*RECT button*/
	Box[++cntb]=(Shape){RECT,0.1,1.1,1.1,2.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*OVAL button*/
	Box[++cntb]=(Shape){RECT,0.1,2.1,1.1,3.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*BRSH button*/
	Box[++cntb]=(Shape){RECT,0.1,3.1,1.1,4.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){195,195,195}};
	/*LINE button*/
	Box[++cntb]=(Shape){RECT,0.1,4.1,1.1,5.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*TEXT button*/
	Box[++cntb]=(Shape){RECT,0.1,5.1,1.1,6.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Red value box*/
	Box[++cntb]=(Shape){RECT,0.1,6.1,1.1,6.6,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Green value box*/
	Box[++cntb]=(Shape){RECT,0.1,6.6,1.1,7.1,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Blue value box*/
	Box[++cntb]=(Shape){RECT,0.1,7.1,1.1,7.6,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	
	/*Red slider box*/
	Box[++cntb]=(Shape){RECT,0.1,7.6,1.0/3+0.1,13,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Green slider box*/
	Box[++cntb]=(Shape){RECT,1.0/3+0.1,7.6,2.0/3+0.1,13,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Blue slider box*/
	Box[++cntb]=(Shape){RECT,2.0/3+0.1,7.6,1.1,13,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	
	/*Red slide block*/
	Box[++cntb]=(Shape){RECT,0.1,7.6,1.0/3+0.1,7.6+1.0/3,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,0,0}};
	/*Green slide block*/
	Box[++cntb]=(Shape){RECT,1.0/3+0.1,7.6,2.0/3+0.1,7.6+1.0/3,2,TRUE,"",NULL,(Color){0,0,0},(Color){0,255,0}};
	/*Blue slide block*/
	Box[++cntb]=(Shape){RECT,2.0/3+0.1,7.6,1.1,7.6+1.0/3,2,TRUE,"",NULL,(Color){0,0,0},(Color){0,0,255}};
	
	/*LINE<->FILL button*/
	Box[++cntb]=(Shape){RECT,0.1,18.0,1.1,18.5,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	/*Color preview window*/
	Box[++cntb]=(Shape){RECT,0.1,18.5,1.1,19.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){0,0,0}};
	
	/*----------------------Pensize selector--------------------------*/
	Box[++cntb]=(Shape){RECT,0.1,19.0,0.6,19.5,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	Box[++cntb]=(Shape){RECT,0.6,19.0,1.1,19.5,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	Box[++cntb]=(Shape){RECT,0.1,19.5,0.6,20.0,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	Box[++cntb]=(Shape){RECT,0.6,19.5,1.1,20.0,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	Box[++cntb]=(Shape){RECT,0.1,20.0,1.1,20.5,2,FALSE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	
	/*--------------------Built-in color selector--------------------*/
	Box[++cntb]=(Shape){RECT,0.1,13.0,0.6,13.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){0,0,0}};
	Box[++cntb]=(Shape){RECT,0.1,13.5,0.6,14.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){127,127,127}};
	Box[++cntb]=(Shape){RECT,0.1,14.0,0.6,14.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){136,0,21}};
	Box[++cntb]=(Shape){RECT,0.1,14.5,0.6,15.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){237,28,36}};
	Box[++cntb]=(Shape){RECT,0.1,15.0,0.6,15.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,127,39}};
	Box[++cntb]=(Shape){RECT,0.1,15.5,0.6,16.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,242,0}};
	Box[++cntb]=(Shape){RECT,0.1,16.0,0.6,16.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){34,177,76}};
	Box[++cntb]=(Shape){RECT,0.1,16.5,0.6,17.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){0,162,232}};
	Box[++cntb]=(Shape){RECT,0.1,17.0,0.6,17.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){63,72,207}};
	Box[++cntb]=(Shape){RECT,0.1,17.5,0.6,18.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){163,73,164}};
	Box[++cntb]=(Shape){RECT,0.6,13.0,1.1,13.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,255,255}};
	Box[++cntb]=(Shape){RECT,0.6,13.5,1.1,14.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){195,195,195}};
	Box[++cntb]=(Shape){RECT,0.6,14.0,1.1,14.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){182,122,87}};
	Box[++cntb]=(Shape){RECT,0.6,14.5,1.1,15.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,174,201}};
	Box[++cntb]=(Shape){RECT,0.6,15.0,1.1,15.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){255,201,14}};
	Box[++cntb]=(Shape){RECT,0.6,15.5,1.1,16.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){239,228,176}};
	Box[++cntb]=(Shape){RECT,0.6,16.0,1.1,16.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){181,230,29}};
	Box[++cntb]=(Shape){RECT,0.6,16.5,1.1,17.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){153,217,234}};
	Box[++cntb]=(Shape){RECT,0.6,17.0,1.1,17.5,2,TRUE,"",NULL,(Color){0,0,0},(Color){112,146,190}};
	Box[++cntb]=(Shape){RECT,0.6,17.5,1.1,18.0,2,TRUE,"",NULL,(Color){0,0,0},(Color){200,191,231}};
	
	/*-----------------------TEXT displayer--------------------------*/
	Text[++cntt]=(Shape){TXTB,0.6,6.5,0,0,17,TRUE,"000","Courier New",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,7,0,0,17,TRUE,"000","Courier New",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,7.5,0,0,17,TRUE,"000","Courier New",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,0.75,0,0,15,TRUE,"MOVE","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,1.75,0,0,15,TRUE,"RECT","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,2.75,0,0,15,TRUE,"OVAL","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,3.75,0,0,15,TRUE,"BRSH","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,4.75,0,0,15,TRUE,"LINE","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,5.75,0,0,15,TRUE,"TEXT","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.6,18.375,0,0,15,TRUE,"LINE","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.35,19.425,0,0,15,TRUE,"<","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.85,19.425,0,0,15,TRUE,">","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.35,19.925,0,0,15,TRUE,"-","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.85,19.925,0,0,15,TRUE,"+","Consolas",(Color){0,0,0},(Color){0,0,0}};
	Text[++cntt]=(Shape){TXTB,0.60,20.425,0,0,17,TRUE,"02","Courier New",(Color){0,0,0},(Color){0,0,0}};

	/*Paint all shapes.*/
	for(i=1;i<=cntb;++i) PaintShape(&Box[i]);
	for(i=1;i<=cntt;++i) PaintShape(&Text[i]);
	return 0;
}

/*Fix the slide bar*/
void Repaint_UI()
{
	int i;
	for(i=10;i<=12;++i) PaintShape(&Box[i]);
}

/*Apply Mode with Temp_Mode*/
void Apply_temp_mode()
{
	Mode=Temp_Mode;
}

/*Check if a==b. 1 for TRUE, 0 for FALSE*/
int Cmp(Shape a,Shape b)
{
	if(a.type!=b.type)return 1;
	if(a.sx!=b.sx)return 1;
	if(a.tx!=b.tx)return 1;
	if(a.sy!=b.sy)return 1;
	if(a.ty!=b.ty)return 1;
	if(a.filled!=b.filled)return 1;
	if(a.pensize!=b.pensize)return 1;
	if(a.font!=b.font)return 1;
	if(a.col.r!=b.col.r || a.col.g!=b.col.g || a.col.b!=b.col.b)return 1;
	if(a.fcol.r!=b.fcol.r || a.fcol.g!=b.fcol.g || a.fcol.b!=b.fcol.b)return 1;
	return strcmp(a.str,b.str);
}

/*when button_up, add a new shape to the list*/
void Finish_Mode()
{
	/*Move mode do not creats shapes*/
	                 /*if CurrentShape.type==NONE means the shape has been added.*/
	if(Mode==PAINT_MOVE || CurrentShape.type==NONE) return ;
	/*fliter for shapes which are created by a simple click, which is a point*/
	if(CurrentShape.type!=TXTB && (fabs(CurrentShape.sx-CurrentShape.tx)<=EPS &&
		fabs(CurrentShape.sy-CurrentShape.ty)<=EPS))
		return ;
	/*Inser to the List*/
	Insert_before(Shape_L,Shape_L->end,CurrentShape);
	/*Set flag*/
	CurrentShape.type=NONE;
	return ;
}

/*when button down, start a paint mode, create a new shape*/
void Start_Mode(double x,double y)
{
	/*if last CurrentShape has not been added to list*/
	if(Shape_L->end->last && Cmp(Shape_L->end->last->data,CurrentShape)) Finish_Mode();
	/*if last text object has an empty 'str', skip*/
	if(Shape_L->end->last && Shape_L->end->last->data.type==TXTB && Shape_L->end->last->data.str[0]==0)
	{
		/*erase old vertical location line*/
		SetEraseMode(TRUE);
		PaintShape(&(Shape){LINE,Shape_L->end->last->data.sx,Shape_L->end->last->data.sy,
			Shape_L->end->last->data.tx,Shape_L->end->last->data.ty-1.2*pica,
				1,FALSE,"",0,(Color){0,0,0},(Color){0,0,0}});
		SetEraseMode(FALSE);
		/*Remove the empty element from list*/
		Delete(Shape_L,Shape_L->end->last);
	}
	if(Mode==PAINT_RECT)/*New rectangle*/
	{
		CurrentShape=(Shape){RECT,x/pica,(21-y/pica),x/pica,(21-y/pica),curpensize,FillMode,"",0,
			PenCol,FillCol};
	}
	if(Mode==PAINT_LINE)/*new line*/
	{
		CurrentShape=(Shape){LINE,x/pica,(21-y/pica),x/pica,(21-y/pica),curpensize,FALSE,"",0,
			PenCol,FillCol};
	}
	if(Mode==PAINT_OVAL)/*new oval*/
	{
		CurrentShape=(Shape){OVAL,x/pica,(21-y/pica),x/pica,(21-y/pica),curpensize,FillMode,"",0,
			PenCol,FillCol};
	}
	if(Mode==PAINT_TEXT)/*New textbox*/
	{
		/*default font and fontsize can not change in UI*/
		CurrentShape=(Shape){TXTB,x/pica,(21-y/pica),x/pica,(21-y/pica),18+curpensize,FillMode,"","Courier New",
			PenCol,FillCol};
		SetFont("Courier New"); SetPointSize(18+curpensize);
		PaintShape(&(Shape){LINE,x/pica,(21-y/pica),x/pica,(21-y/pica)-1.2*pica,1,FALSE,"",0,
			(Color){0,0,0},(Color){0,0,0}});
	}
}

/*after selected a built-in color, we should update the slide bar and the value numbers
 	which are displayed above*/
void Refresh_bar()
{
	/*Erease the old number of RGB*/
	SetEraseMode(TRUE);
	PaintShape(&Box[13]); PaintShape(&Box[14]); PaintShape(&Box[15]);
	PaintShape(&Text[1]); PaintShape(&Text[2]); PaintShape(&Text[3]);
	SetEraseMode(FALSE);
	if(!FillMode)/*Print the Pencolor*/
	{
		sprintf(Text[1].str,"%03d",PenCol.r);
		sprintf(Text[2].str,"%03d",PenCol.g);
		sprintf(Text[3].str,"%03d",PenCol.b);
		/*calc the position of each slide blocks*/
		Box[13].sy=PenCol.r/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[13].ty=Box[13].sy+1.0/3;
		Box[14].sy=PenCol.g/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[14].ty=Box[14].sy+1.0/3;
		Box[15].sy=PenCol.b/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[15].ty=Box[15].sy+1.0/3;
	}
	else/*Print the Fillcolor*/
	{
		sprintf(Text[1].str,"%03d",FillCol.r);
		sprintf(Text[2].str,"%03d",FillCol.g);
		sprintf(Text[3].str,"%03d",FillCol.b);
		/*calc the position of each slide blocks*/
		Box[13].sy=FillCol.r/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[13].ty=Box[13].sy+1.0/3;
		Box[14].sy=FillCol.g/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[14].ty=Box[14].sy+1.0/3;
		Box[15].sy=FillCol.b/255.0*(13-1.0/3-7.6-0.02)+7.6;
		Box[15].ty=Box[15].sy+1.0/3;
	}
	PaintShape(&Box[13]); PaintShape(&Text[1]);
	PaintShape(&Box[14]); PaintShape(&Text[2]);
	PaintShape(&Box[15]); PaintShape(&Text[3]);
}

/*if the slide block is moved, the slide bar may be partly erased, so 
  we repaint the bar to make it looks better.*/
void Fix_Move()
{
	List_iterator it; int i;
	/*Repaint location points*/
	SetEraseMode(TRUE);
	for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	SetEraseMode(FALSE);
	double sx=SelectedObj->data.sx,sy=SelectedObj->data.sy;
	double tx=SelectedObj->data.tx,ty=SelectedObj->data.ty;
	SelectFrame[0]=(Shape){RECT,sx-0.1,sy-0.1,sx+0.1,sy+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[1]=(Shape){RECT,sx-0.1,ty-0.1,sx+0.1,ty+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[2]=(Shape){RECT,tx-0.1,ty-0.1,tx+0.1,ty+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[3]=(Shape){RECT,tx-0.1,sy-0.1,tx+0.1,sy+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	
	SelectFrame[4]=(Shape){RECT,(sx+tx)/2-0.1,sy-0.1,(sx+tx)/2+0.1,sy+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[5]=(Shape){RECT,tx-0.1,(sy+ty)/2-0.1,tx+0.1,(sy+ty)/2+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[6]=(Shape){RECT,(sx+tx)/2-0.1,ty-0.1,(sx+tx)/2+0.1,ty+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[7]=(Shape){RECT,sx-0.1,(sy+ty)/2-0.1,sx+0.1,(sy+ty)/2+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	
	SelectFrame[8]=(Shape){NONE,sx,sy,tx,ty,1,FALSE,"",0,(Color){0,0,0},(Color){255,255,255}};
	Shape* L=&SelectedObj->data;
	if(SelectedObj->data.type==TXTB)
	{
		SetPointSize(L->pensize);
		SetFont(L->font);
		if(L->filled) SelectFrame[8]=(Shape){RECT,L->sx-TextStringWidth(L->str)/2/pica,
				L->sy-0.6,L->sx+TextStringWidth(L->str)/2/pica,L->sy,1,FALSE};
		else SelectFrame[8]=(Shape){RECT,L->sx,L->sy-0.6,L->sx+TextStringWidth(L->str)/pica,L->sy,1,FALSE};
	}
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	if(SelectedObj->data.type!=TXTB)
		for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	else PaintShape(&SelectFrame[8]);
}

/*erase the eight white points around*/
void Clear_Move()
{
	List_iterator it; int i;
	SetEraseMode(TRUE);
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	SetEraseMode(FALSE);
}

/*paint when mouse moves*/
void Paint(double lx,double ly,double x,double y)
{
	Set_Painter_Color(PenCol);
	if(Mode==PAINT_BRSH)/*paint with brush*/
	{ 
		SetPenSize(curpensize);
		MovePen(lx,ly);
		DrawLine(x-lx,y-ly);
	}
	if(Mode==PAINT_RECT || Mode==PAINT_OVAL)/*erase the old one and draw a new one*/
	{
		List_iterator it;
		SetEraseMode(TRUE);
		PaintShape(&CurrentShape);
		SetEraseMode(FALSE);
		if(Shift_fl)/*if SHIFT is down*/
		{
			/*paint square or circle*/
			double delta1=x/pica-CurrentShape.sx;
			double delta2=21-y/pica-CurrentShape.sy;
			if(fabs(delta1)<fabs(delta2))
			{
				if((delta1>=0) ^ (delta2>=0))CurrentShape.tx=CurrentShape.sx+delta1,CurrentShape.ty=CurrentShape.sy-delta1;
				else CurrentShape.tx=CurrentShape.sx+delta1,CurrentShape.ty=CurrentShape.sy+delta1;
			}
			else
			{
				if((delta1>=0) ^ (delta2>=0))CurrentShape.tx=CurrentShape.sx-delta2,CurrentShape.ty=CurrentShape.sy+delta2;
				else CurrentShape.tx=CurrentShape.sx+delta2,CurrentShape.ty=CurrentShape.sy+delta2;
			}
		}
		else CurrentShape.tx=x/pica,CurrentShape.ty=(21-y/pica);
		for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
			PaintShape(&it->data);
		PaintShape(&CurrentShape);
	}
	if(Mode==PAINT_LINE)
	{
		List_iterator it;
		/*erase the old one*/
		SetEraseMode(TRUE);
		PaintShape(&CurrentShape);
		SetEraseMode(FALSE);
		/*if SHIFT is down then align to 45 degrees*/
		if(Shift_fl)
		{
			double delta1=x/pica-CurrentShape.sx;
			double delta2=21-y/pica-CurrentShape.sy;
			/*horizontal*/
			if(atan(fabs(delta2/delta1))<=pi/8)
			{
				CurrentShape.tx=CurrentShape.sx+delta1;
				CurrentShape.ty=CurrentShape.sy;
			}
			/*vertical*/
			else if(atan(fabs(delta2/delta1))>=pi*3/8)
			{
				CurrentShape.tx=CurrentShape.sx;
				CurrentShape.ty=CurrentShape.sy+delta2;
			}
			/*left-down to right-up*/
			else if(fabs(delta1)<fabs(delta2))
			{
				if((delta1>=0) ^ (delta2>=0))CurrentShape.tx=CurrentShape.sx+delta1,CurrentShape.ty=CurrentShape.sy-delta1;
				else CurrentShape.tx=CurrentShape.sx+delta1,CurrentShape.ty=CurrentShape.sy+delta1;
			}
			/*left-up to right-down*/
			else
			{
				if((delta1>=0) ^ (delta2>=0))CurrentShape.tx=CurrentShape.sx-delta2,CurrentShape.ty=CurrentShape.sy+delta2;
				else CurrentShape.tx=CurrentShape.sx+delta2,CurrentShape.ty=CurrentShape.sy+delta2;
			}
		}
		else CurrentShape.tx=x/pica,CurrentShape.ty=(21-y/pica);
		for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
			PaintShape(&it->data);
		PaintShape(&CurrentShape);
	}
	/*Move the red slide block*/
	if(Mode==PAINT_MODIFY_COLOR_R)
	{
		if(Box[13].sy>=7.6-EPS && Box[13].ty<=13+EPS && 
		Box[13].sy-(y-ly)/pica>=7.6-EPS && Box[13].ty-(y-ly)/pica<=13+EPS)
		{
			SetEraseMode(TRUE);
			PaintShape(&Box[13]);
			SetEraseMode(FALSE);
		}
		Box[13].sy-=(y-ly)/pica;
		Box[13].ty-=(y-ly)/pica;
		if(Box[13].sy>=7.6-EPS && Box[13].ty<=13+EPS)
		{
			PaintShape(&Box[13]);
			if(!FillMode)PenCol.r=(Box[13].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=PenCol,PaintShape(&Box[17]);
			else FillCol.r=(Box[13].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=FillCol,PaintShape(&Box[17]);
			if(Temp_Mode==PAINT_MOVE && SelectedObj)
				if(!FillMode)SelectedObj->data.col=PenCol,Fix_Move();
				else SelectedObj->data.fcol=FillCol,Fix_Move();
		}
		SetEraseMode(TRUE);
		PaintShape(&Text[1]);
		SetEraseMode(FALSE);
		if(!FillMode)sprintf(Text[1].str,"%03d",PenCol.r);
		else sprintf(Text[1].str,"%03d",FillCol.r);
		PaintShape(&Text[1]);
	}
	/*Move the green slide block*/
	if(Mode==PAINT_MODIFY_COLOR_G)
	{
		if(Box[14].sy>=7.6-EPS && Box[14].ty<=13+EPS && 
		Box[14].sy-(y-ly)/pica>=7.6-EPS && Box[14].ty-(y-ly)/pica<=13+EPS)
		{
			SetEraseMode(TRUE);
			PaintShape(&Box[14]);
			SetEraseMode(FALSE);
		}
		Box[14].sy-=(y-ly)/pica;
		Box[14].ty-=(y-ly)/pica;
		if(Box[14].sy>=7.6-EPS && Box[14].ty<=13+EPS)
		{
			PaintShape(&Box[14]);
			if(!FillMode)PenCol.g=(Box[14].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=PenCol,PaintShape(&Box[17]);
			else FillCol.g=(Box[14].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=FillCol,PaintShape(&Box[17]);
			if(Temp_Mode==PAINT_MOVE && SelectedObj)
				if(!FillMode)SelectedObj->data.col=PenCol,Fix_Move();
				else SelectedObj->data.fcol=FillCol,Fix_Move();
		}
		SetEraseMode(TRUE);
		PaintShape(&Text[2]);
		SetEraseMode(FALSE);
		if(!FillMode)sprintf(Text[2].str,"%03d",PenCol.g);
		else sprintf(Text[2].str,"%03d",FillCol.g);
		PaintShape(&Text[2]);
	}
	/*Move the blue slide block*/
	if(Mode==PAINT_MODIFY_COLOR_B)
	{
		if(Box[15].sy>=7.6-EPS && Box[15].ty<=13+EPS && 
		Box[15].sy-(y-ly)/pica>=7.6-EPS && Box[15].ty-(y-ly)/pica<=13+EPS)
		{
			SetEraseMode(TRUE);
			PaintShape(&Box[15]);
			SetEraseMode(FALSE);
		}
		Box[15].sy-=(y-ly)/pica;
		Box[15].ty-=(y-ly)/pica;
		if(Box[15].sy>=7.6-EPS && Box[15].ty<=13+EPS)
		{
			PaintShape(&Box[15]);
			if(!FillMode)PenCol.b=(Box[15].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=PenCol,PaintShape(&Box[17]);
			else FillCol.b=(Box[15].sy-7.6)/(13-1.0/3-7.6-0.02)*255,Box[17].fcol=FillCol,PaintShape(&Box[17]);
			if(Temp_Mode==PAINT_MOVE && SelectedObj)
				if(!FillMode)SelectedObj->data.col=PenCol,Fix_Move();
				else SelectedObj->data.fcol=FillCol,Fix_Move();
		}
		SetEraseMode(TRUE);
		PaintShape(&Text[3]);
		SetEraseMode(FALSE);
		if(!FillMode)sprintf(Text[3].str,"%03d",PenCol.b);
		else sprintf(Text[3].str,"%03d",FillCol.b);
		PaintShape(&Text[3]);
	}
	/*-------------------In move mode, moving eight location points----------------------*/
	if(Mode==MOVE_SX)
	{
		Clear_Move();
		SelectedObj->data.sx=x/pica;
		if(Shift_fl)SelectedObj->data.sy=SelectedObj->data.ty+
			(SelectedObj->data.sx-SelectedObj->data.tx)*deltay/deltax;
		Fix_Move();
	}
	if(Mode==MOVE_TX)
	{
		Clear_Move();
		SelectedObj->data.tx=x/pica;
		if(Shift_fl)SelectedObj->data.ty=SelectedObj->data.sy+
			(SelectedObj->data.tx-SelectedObj->data.sx)*deltay/deltax;
		Fix_Move();
	}
	if(Mode==MOVE_SY)
	{
		Clear_Move();
		SelectedObj->data.sy=21-y/pica;
		if(Shift_fl)SelectedObj->data.sx=SelectedObj->data.tx+
			(SelectedObj->data.sy-SelectedObj->data.ty)*deltax/deltay;
		Fix_Move();
	}
	if(Mode==MOVE_TY)
	{
		Clear_Move();
		SelectedObj->data.ty=21-y/pica;
		if(Shift_fl)SelectedObj->data.tx=SelectedObj->data.sx+
			(SelectedObj->data.ty-SelectedObj->data.sy)*deltax/deltay;
		Fix_Move();
	}
	if(Mode==MOVE_LU)
	{
		Clear_Move();
		SelectedObj->data.sx=x/pica;
		SelectedObj->data.sy=21-y/pica;
		Fix_Move();
	}
	if(Mode==MOVE_RU)
	{
		Clear_Move();
		SelectedObj->data.tx=x/pica;
		SelectedObj->data.sy=21-y/pica;
		Fix_Move();
	}
	if(Mode==MOVE_LD)
	{
		Clear_Move();
		SelectedObj->data.sx=x/pica;
		SelectedObj->data.ty=21-y/pica;
		Fix_Move();
	}
	if(Mode==MOVE_RD)
	{
		Clear_Move();
		SelectedObj->data.tx=x/pica;
		SelectedObj->data.ty=21-y/pica;
		Fix_Move();
	}
	if(Mode==MOVE_OBJ)
	{
		Clear_Move();
		SelectedObj->data.sx+=(x-lx)/pica;
		SelectedObj->data.sy-=(y-ly)/pica;
		SelectedObj->data.tx+=(x-lx)/pica;
		SelectedObj->data.ty-=(y-ly)/pica;
		Fix_Move();
	}
	/*--------------------------------------------------------------*/
	return ;
}

/*slide block can not be moved out of the slide bar*/
void Fix_button_y()
{
	Box[13].sy=Max(Box[13].sy,7.6);
	Box[13].sy=Min(Box[13].sy,13-1.0/3);
	Box[13].ty=Max(Box[13].ty,7.6+1.0/3);
	Box[13].ty=Min(Box[13].ty,13);

	Box[14].sy=Max(Box[14].sy,7.6);
	Box[14].sy=Min(Box[14].sy,13-1.0/3);
	Box[14].ty=Max(Box[14].ty,7.6+1.0/3);
	Box[14].ty=Min(Box[14].ty,13);

	Box[15].sy=Max(Box[15].sy,7.6);
	Box[15].sy=Min(Box[15].sy,13-1.0/3);
	Box[15].ty=Max(Box[15].ty,7.6+1.0/3);
	Box[15].ty=Min(Box[15].ty,13);
}

/*Char event recall*/
void Char_recall(char ch)
{
	if(Mode!=PAINT_TEXT)return ;
	
	SetEraseMode(TRUE);
	
	PaintShape(&(Shape){LINE,Shape_L->end->last->data.sx,Shape_L->end->last->data.sy,
		Shape_L->end->last->data.tx,Shape_L->end->last->data.ty-1.2*pica,
			1,FALSE,"",0,(Color){0,0,0},(Color){0,0,0}});
			
	PaintShape(&Shape_L->end->last->data);
	
	SetEraseMode(FALSE);
	
	if(ch=='\b')Shape_L->end->last->data.str[strlen(Shape_L->end->last->data.str)-1]=0;
	else { char s[2]; s[0]=ch; s[1]=0; strcat(Shape_L->end->last->data.str,s); }
	
	List_iterator it;
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	PaintShape(&Shape_L->end->last->data);
	return ;
}

/*Keyboard event recall*/
void Key_recall(int key, int event)
{
	if(key==VK_SHIFT && event==KEY_DOWN)
	{
		if(SelectedObj && !Shift_fl)deltax=SelectedObj->data.sx-SelectedObj->data.tx;
		if(SelectedObj && !Shift_fl)deltay=SelectedObj->data.sy-SelectedObj->data.ty;
		Shift_fl=TRUE;
		if(Leftbutton_fl)Paint(last_x,last_y,last_x,last_y); 
	}
	if(key==VK_SHIFT && event==KEY_UP)
	{
		Shift_fl=FALSE;
		if(Leftbutton_fl)Paint(last_x,last_y,last_x,last_y);
	}
	if(key==VK_DELETE && event==KEY_UP)
	{
		if(SelectedObj)
		{
			int i; List_iterator it;
			Delete(Shape_L,SelectedObj);
			SetEraseMode(TRUE);
			PaintShape(&SelectedObj->data);
			for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
			SetEraseMode(FALSE);
			for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
				PaintShape(&it->data);
			SelectedObj=0;
		}
	}
	return ;
}

/*after click in MOVE Mode select an object*/
Shape* Select(double x,double y)
{
	List_iterator it; int i;
	/*clean the old location points*/
	SetEraseMode(TRUE);
	for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	SetEraseMode(FALSE);
	/*Repaint all shapes*/
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	/*check from top to buttom, if clicked point is in the shape.*/
	for(it=Shape_L->end->last;it;it=it->last)
		/*if clicked this point*/
		if(InShape(x,y,&it->data))
		{
			/*Move the object to the top layer*/
			Insert_before(Shape_L,Shape_L->end,it->data);
			Delete(Shape_L,it);
			break;
		}
	/*No object selected*/
	if(!it) return 0;
	/*Select a shape*/
	SelectedObj=Shape_L->end->last;
	double sx=SelectedObj->data.sx,sy=SelectedObj->data.sy;
	double tx=SelectedObj->data.tx,ty=SelectedObj->data.ty;
	/*---------------------Define the eight location points --------------------*/
	SelectFrame[0]=(Shape){RECT,sx-0.1,sy-0.1,sx+0.1,sy+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[1]=(Shape){RECT,sx-0.1,ty-0.1,sx+0.1,ty+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[2]=(Shape){RECT,tx-0.1,ty-0.1,tx+0.1,ty+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[3]=(Shape){RECT,tx-0.1,sy-0.1,tx+0.1,sy+0.1,1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	
	SelectFrame[4]=(Shape){RECT,(sx+tx)/2-0.1,sy-0.1,(sx+tx)/2+0.1,sy+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[5]=(Shape){RECT,tx-0.1,(sy+ty)/2-0.1,tx+0.1,(sy+ty)/2+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[6]=(Shape){RECT,(sx+tx)/2-0.1,ty-0.1,(sx+tx)/2+0.1,ty+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	SelectFrame[7]=(Shape){RECT,sx-0.1,(sy+ty)/2-0.1,sx+0.1,(sy+ty)/2+0.1,
			1,TRUE,"",0,(Color){0,0,0},(Color){255,255,255}};
	
	SelectFrame[8]=(Shape){NONE,sx,sy,tx,ty,1,FALSE,"",0,(Color){0,0,0},(Color){255,255,255}};
	Shape* L=&SelectedObj->data;
	/*No location points for TextBox*/
	if(SelectedObj->data.type==TXTB)
	{
		SetPointSize(L->pensize);
		SetFont(L->font);
		/*calculate the bound of textbox*/
		if(L->filled) SelectFrame[8]=(Shape){RECT,L->sx-TextStringWidth(L->str)/2/pica,
				L->sy-0.6,L->sx+TextStringWidth(L->str)/2/pica,L->sy,1,FALSE};/*middle*/
		else SelectFrame[8]=(Shape){RECT,L->sx,L->sy-0.6,L->sx+TextStringWidth(L->str)/pica,L->sy,1,FALSE};/*left*/
	}
	/*Repaint location points*/
	if(SelectedObj->data.type!=TXTB)
		for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	else PaintShape(&SelectFrame[8]);
	/*--------------------------------------------------------------------------*/
	return SelectedObj;
}

/*select a built-in color and change pencolor or fillcolor*/
void SelectBox(int x)
{
	int i;
	for(i=1;i<=6;++i) if(i!=x) Box[i].fcol=(Color){255,255,255};
		else Box[i].fcol=(Color){195,195,195};
}

/*change current color, decide to change FillCol or PenCol*/
void Change_Color(Color x)
{
	if(FillMode)FillCol=x,Box[17].fcol=x;
	else PenCol=x,Box[17].fcol=x;
	Refresh_bar();
}

/*Flip Mode LINE & FILL*/
void Flip()
{
	FillMode=!FillMode;
	if(FillMode)strcpy(Text[10].str,"FILL"),Box[17].fcol=FillCol;
	else strcpy(Text[10].str,"LINE"),Box[17].fcol=PenCol;
	Refresh_bar();
	/*RepaintALL();*/
}

/*after flipping the mode, slide bar should recalculate*/
void Refresh_selected_col()
{
	if(!SelectedObj)return ;
	SelectedObj->data.col=PenCol;
	SelectedObj->data.fcol=FillCol;
	Fix_Move();
}

void EraseAllShape()
{
	List_iterator it; int i;
	SetEraseMode(TRUE);
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	if(SelectedObj && SelectedObj->data.type!=TXTB)
		for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	else PaintShape(&SelectFrame[8]);
	SetEraseMode(FALSE);
}

void PaintAllShape()
{
	List_iterator it; int i;
	for(it=Shape_L->begin;it!=Shape_L->end;it=it->next)
		PaintShape(&it->data);
	if(!SelectedObj) return ;
	Shape* L=&SelectedObj->data;
	if(SelectedObj->data.type==TXTB)
	{
		SetPointSize(L->pensize);
		SetFont(L->font);
		/*calculate the bound of textbox*/
		if(L->filled) SelectFrame[8]=(Shape){RECT,L->sx-TextStringWidth(L->str)/2/pica,
				L->sy-0.6,L->sx+TextStringWidth(L->str)/2/pica,L->sy,1,FALSE};/*middle*/
		else SelectFrame[8]=(Shape){RECT,L->sx,L->sy-0.6,L->sx+TextStringWidth(L->str)/pica,L->sy,1,FALSE};/*left*/
	}
	if(SelectedObj->data.type!=TXTB)
		for(i=8;i>=0;--i) PaintShape(&SelectFrame[i]);
	else PaintShape(&SelectFrame[8]);
}

/*Repaint pensize box*/
void Repaint_pensizebox()
{
	PaintAllShape();
	SetPenSize(curpensize);
	SetEraseMode(TRUE);
	PaintShape(&Text[cntt]);
	SetEraseMode(FALSE);
	sprintf(Text[cntt].str,"%02d",curpensize);
	PaintShape(&Text[cntt]);
}

/*mouse event recall*/
void Mouse_recall(int xx,int yy,int button,int event)
{
	int i;
	/*transform pixel to inch*/
	double x=ScaleXInches(xx),y=ScaleYInches(yy);
	/*leftbutton down*/
	if(event==BUTTON_DOWN && button==LEFT_BUTTON)
	{
		Leftbutton_fl=TRUE; Leftbutton_down_x=x; Leftbutton_down_y=y;
		if(Inbox(x,y,&Box[1])) Temp_Mode=PAINT_MOVE,SelectBox(1);/*MOVE mode button*/
		else if(Inbox(x,y,&Box[2])) Temp_Mode=PAINT_RECT,SelectBox(2);/*RECT mode button*/
		else if(Inbox(x,y,&Box[3])) Temp_Mode=PAINT_OVAL,SelectBox(3);/*OVAL mode button*/
		else if(Inbox(x,y,&Box[4])) Temp_Mode=PAINT_BRSH,SelectBox(4);/*BRSh mode button*/
		else if(Inbox(x,y,&Box[5])) Temp_Mode=PAINT_LINE,SelectBox(5);/*LINE mode button*/
		else if(Inbox(x,y,&Box[6])) Temp_Mode=PAINT_TEXT,SelectBox(6);/*TEXT mode button*/
		
		else if(Inbox(x,y,&Box[13])) Temp_Mode=Mode,Mode=PAINT_MODIFY_COLOR_R;/*Sliding Red Block*/
		else if(Inbox(x,y,&Box[14])) Temp_Mode=Mode,Mode=PAINT_MODIFY_COLOR_G;/*Sliding Green Block*/
		else if(Inbox(x,y,&Box[15])) Temp_Mode=Mode,Mode=PAINT_MODIFY_COLOR_B;/*Sliding Blue Block*/
		else if(Inbox(x,y,&Box[16])) Temp_Mode=Mode,Mode=NONE,Flip();/*Button of flipping LINE Mode and FILL Mode*/
		else if(Inbox(x,y,&Box[18]))
		{
			Temp_Mode=Mode,Mode=NONE;
			EraseAllShape();
			if(curpensize>5)curpensize-=5;
			if(SelectedObj)
			{
				SelectedObj->data.pensize=curpensize;
				if(SelectedObj->data.type==TXTB)SelectedObj->data.pensize=1*SelectedObj->data.pensize+18;
			}
			Repaint_pensizebox();
			
		}/*Set pen size +5*/
		else if(Inbox(x,y,&Box[19]))
		{
			Temp_Mode=Mode,Mode=NONE;
			EraseAllShape();
			curpensize+=5;
			if(SelectedObj)
			{
				SelectedObj->data.pensize=curpensize;
				if(SelectedObj->data.type==TXTB)SelectedObj->data.pensize=1*SelectedObj->data.pensize+18;
			}
			Repaint_pensizebox();
		}/*Set pen size -5*/
		else if(Inbox(x,y,&Box[20]))
		{
			Temp_Mode=Mode,Mode=NONE;
			EraseAllShape();
			if(curpensize>1)curpensize--;
			if(SelectedObj)
			{
				SelectedObj->data.pensize=curpensize;
				if(SelectedObj->data.type==TXTB)SelectedObj->data.pensize=1*SelectedObj->data.pensize+18;
			}
			Repaint_pensizebox();
		}/*Set pen size +1*/
		else if(Inbox(x,y,&Box[21]))
		{
			Temp_Mode=Mode,Mode=NONE;
			EraseAllShape();
			curpensize++;
			if(SelectedObj)
			{
				SelectedObj->data.pensize=curpensize;
				if(SelectedObj->data.type==TXTB)SelectedObj->data.pensize=1*SelectedObj->data.pensize+18;
			}
			Repaint_pensizebox();
		}/*Set pen size -1*/
		
		/*20 Built-in color button*/
		else for(i=cntb-19;i<=cntb;++i)
			if(Inbox(x,y,&Box[i]))
			{ Temp_Mode=Mode,Mode=PAINT_MODIFY_BUILTIN_COL;  
				/*Change color*/ Change_Color(Box[i].fcol); Refresh_selected_col(); Refresh_bar(); }
			else Start_Mode(x,y);
		if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[0]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_LU;/*Modifier left-up*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[1]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_LD;/*Modifier left-down*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[2]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_RD;/*Modifier right-down*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[3]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_RU;/*Modifier right-up*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[4]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_SY;/*Modifier up*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[5]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_TX;/*Modifier right*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[6]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_TY;/*Modifier down*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[7]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_SX;/*Modifier left*/
		else if(Mode==PAINT_MOVE && Inbox(x,y,&SelectFrame[8]) && SelectedObj) Temp_Mode=Mode,Mode=MOVE_OBJ;/*Modifier centre*/
		else if(Mode==PAINT_MOVE) SelectedObj=Select(x,y);/*Not clicking on a button try to select a new object*/
		/*Record the ratio of height and width prepare for SHIFT*/
		if(SelectedObj)deltax=SelectedObj->data.sx-SelectedObj->data.tx;
		if(SelectedObj)deltay=SelectedObj->data.sy-SelectedObj->data.ty;
	}
	/*Button is up*/
	if(event==BUTTON_UP && button==LEFT_BUTTON)
	{
		/*Set button down flag FALSE*/
		Leftbutton_fl=FALSE;
		/*New Mode or Recover Mode*/
		if(Temp_Mode!=NONE)Apply_temp_mode(),Temp_Mode=NONE;
		/*if current mode may creat new shapes*/
		if(Mode==2 || Mode==3 || Mode==5 || Mode==6) Finish_Mode();
		Fix_button_y();
		/*painting may erase the tools bar*/
		RepaintALL();
	}
	if(event==MOUSEMOVE)
	{
		/*if button is down start painting*/
		if(Leftbutton_fl) Paint(last_x,last_y,x,y);
		Repaint_UI();
	}
	/*Record last coord*/
	last_x=x,last_y=y; 
	return ;
}

void Main()
{
	Shape_L=New_list();/*New list storing shapes*/
	SetWindowSize(16,10.5);
	InitGraphics();
	SetPenSize(2);/*default pensize*/
	Init_UI();
	registerCharEvent(Char_recall);
	registerMouseEvent(Mouse_recall);
	registerKeyboardEvent(Key_recall);
	return ;
}
