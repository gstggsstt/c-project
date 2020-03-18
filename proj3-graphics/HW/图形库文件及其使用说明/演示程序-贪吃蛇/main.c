#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
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


#define  INIT_X                0   
#define  INIT_Y                0    
#define  GAME_WIDE             9    
#define  GAME_HEIGHT           6    
#define  BOX_WIDE              0.3   

const int speed = 200;          
bool isStart = 0;             
int map[30][20]={0};      

struct node{
	int x;
	int y;
};
struct node head;            
struct node tail;              
struct node tmp;                 

static int CurrentDirection=1;   
static int d;                    

void DrawArea();                  
void DrawBox(double x, double y); 
void CreatFood();                 
void DrawSnake();                 
void InitSnake();                 
void KeyboardEventProcess(int key,int event);
void TimerEventProcess(int timerID);

void Main(){
	InitGraphics();
    DrawArea();
	InitSnake();
	CreatFood();
	DrawSnake();
	registerKeyboardEvent(KeyboardEventProcess);/*注册键盘消息回调函数*/
	registerTimerEvent(TimerEventProcess);/*注册定时器消息回调函数*/
}

void DrawArea(){
	SetPenSize(2);
	SetPenColor("blue");
	MovePen(INIT_X, INIT_Y);
    DrawLine(GAME_WIDE, 0);
    DrawLine(0, GAME_HEIGHT);
    DrawLine(-GAME_WIDE,0);
    DrawLine(0, -GAME_HEIGHT);
} 

void DrawBox(double x, double y){  
	SetPenSize(1);
    MovePen(x, y);
    DrawLine(BOX_WIDE, 0);
    DrawLine(0, BOX_WIDE);
    DrawLine(-BOX_WIDE, 0);
    DrawLine(0, -BOX_WIDE);
}

void CreatFood()	
{
	int x, y;
	srand((unsigned)time(NULL));
	while (1)
	{
		x = rand() % 30;  
		y = rand() % 20;

		if (map[x][y] != 1&map[x][y] != 2&&map[x][y] != 3&&&map[x][y] != 4)  
		{
			map[x][y] = 5;                                                   
			SetPenColor("green");                                            
		    DrawBox(x*BOX_WIDE,y*BOX_WIDE);                                  
			break;
		}

	}

}

void DrawSnake(){
	int i,j;
	SetPenColor("red");           
	for(i=0;i<30;i++)             
		for(j=0;j<20;j++){
			if(map[i][j]==1||map[i][j]==2||map[i][j]==3||map[i][j]==4){
				DrawBox(i*BOX_WIDE,j*BOX_WIDE);
				}
		}
}

void InitSnake(){     
	head.x=3;
	head.y=10;
	tail.x=1;
	tail.y=10;
	map[1][10]=1;
	map[2][10]=1;
	map[3][10]=1;
}
 
void KeyboardEventProcess(int key,int event)/*每当产生键盘消息，都要执行*/
{	 
     switch (event) {
	 	case KEY_DOWN:
			 switch (key) {
			     case VK_ESCAPE:/*键盘ESC，打开或关闭定时器，让游戏开始或者暂停*/
                     isStart = !isStart;
                     if (isStart) {
						startTimer(1, speed);
                     } else {
						cancelTimer(1);
					 }
                     break;
                 case VK_UP:/*键盘上键，按下之后如果当前方向和按下不冲突，则改变当前方向，例：当前方向是向右，那么上和下才是非冲突方向，其他三个方向同理*/
                     d=2;
                     if((d+CurrentDirection)%2!=0)
                     	CurrentDirection=2;
                     break;
			     case VK_DOWN:
			     	 d=4;
			     	 if((d+CurrentDirection)%2!=0)
                     	CurrentDirection=4;
                     break;
			     case VK_LEFT:
			     	 d=3;
			     	 if((d+CurrentDirection)%2!=0)
                     	CurrentDirection=3;
                     break;
			     case VK_RIGHT:
			     	 d=1;
			     	 if((d+CurrentDirection)%2!=0)
                     	CurrentDirection=1;
                     break;
			 }
			 break;
		case KEY_UP:
			 break;
	 }	 
}
 
void TimerEventProcess(int timerID)/*定时器回调函数，用来控制蛇的移动*/
{
	  switch (timerID) {
	    case 1: 
		  if (!isStart) return;
          SetEraseMode(TRUE);
		  DrawSnake();	  
          map[head.x][head.y]=CurrentDirection;
          int tailD= map[tail.x][tail.y];            
		  tmp = tail;                                
          map[tail.x][tail.y]=0;                     
          if(tailD==1){                              
          	tail.x+=1;
          }
          else if(tailD==2){
          	tail.y+=1;
          }
          else if(tailD==3){
          	tail.x-=1;
          }
          else if(tailD==4){
          	tail.y-=1;
          }
          
          if(CurrentDirection==1){                    
          	head.x+=1;	
          }
          else if(CurrentDirection==2){
          	head.y+=1;
          }
          else if(CurrentDirection==3){
          	head.x-=1;
          }
          else if(CurrentDirection==4){
          	head.y-=1;
          }
          if(map[head.x][head.y]==5){                
          	tail=tmp;                                 
          	map[tail.x][tail.y]=tailD;
          }
          if(map[head.x][head.y]!=0&&map[head.x][head.y]!=5){  
          	cancelTimer(1);
          	char text[100]="GAME OVER";
          	MovePen(4,3);
          	SetEraseMode(FALSE);
          	SetPenSize(10);
          	DrawTextString(text);
          	break;
          }
          if(head.x<0||head.x>29||head.y<0||head.y>19){        
          	cancelTimer(1);
          	char text[100]="GAME OVER";
          	MovePen(4,3);
          	SetEraseMode(FALSE);
          	SetPenSize(10);
          	DrawTextString(text);
          	break;
          }
          map[head.x][head.y]=CurrentDirection;
          SetEraseMode(FALSE);
		  DrawSnake();
          int i,j;
          int Exist=0;            
          for(i=0;i<30;i++)
		  	for(j=0;j<20;j++){
				if(map[i][j]==5){
				Exist=1;
				}
			}
		  if(Exist==0){           
		  	CreatFood(); 
		  }
		  
		  break;
	    default:
		  break;
	  }
} 
