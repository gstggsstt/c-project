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

double INITBALLSIZE = 0.8;

double pica = 0.5;
double pi;
double BALLSIZE, last_size;
double pw;

char * Level;
double sumd;
int Inited;
int mousefl = 0, started = 0, t0, coins;
int lastmousex = 0, lastmousey = 0;
int sw[100] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1
              };
int eaten[110];

static int T_mc[2] = {100, 0};
static int d_mc[2] = { -1, +1};
static int dir_mb = 0, dy_mb = -1;
static int fl_rm[100] = {0};
static int fl_tr = 0, fl1_tr = 0, ballfl_tr = 0, finalfl = 0;

/*struct color*/
struct Color_
{
	short int r, g, b;
};
typedef struct Color_ Color;

/*struct line which can move*/
struct line_
{
	double sx, sy, tx, ty;
	bool dangerous;
	Color col;
}; typedef struct line_ line;

/*struct line which can rotate*/
struct rline_
{
	double sx, sy, len, angle;
	bool dangerous;
	double speed, mxang, mnang;
	Color col;
}; typedef struct rline_ rline;

/*struct arc can not move*/
struct arc_
{
	double rx, ry, R;
	double start_angle, end_angle;
	bool dangerous;
	Color col;
}; typedef struct arc_ arc;

/*rectangle*/
struct rect_
{
	double sx, sy, tx, ty;
	bool dangerous;
	Color col, fcol;
}; typedef struct rect_ rect;

/*textbox*/
struct textbox_
{
	char str[110];
	double cx, cy;
	int pointsize;
	Color col;
	char *font;
}; typedef struct textbox_ textbox;

line  A[110], RepA[1100], F[110], Catch[110];
rline B[110];
arc   C[110], Mark[110];
rect  D[110], Switch[110];
textbox T[110];
int cnta, cntb, cntc, cntr, cntd, cntf, cnts, cnth, cntt, cntm;

void SetColor(Color col)
{
	DefineColor("base_color", col.r / 255.0, col.g / 255.0, col.b / 255.0);
	SetPenColor("base_color");
}

void PaintLine(line L)
{
	SetColor(L.col);
	double x = GetCurrentX(), y = GetCurrentY();
	if (fabs(x - L.sx * pica) > 1e-6 || fabs(y - (18 - L.sy)*pica) > 1e-6)
		MovePen(L.sx * pica, (18 - L.sy)*pica);
	DrawLine((L.tx - L.sx)*pica, (L.sy - L.ty)*pica);
}
void PaintRLine(rline L)
{
	SetColor(L.col);
	double x = GetCurrentX(), y = GetCurrentY();
	double tx = L.sx + cos(L.angle) * L.len, ty = L.sy + sin(L.angle) * L.len;
	if (fabs(x - L.sx * pica) > 1e-6 || fabs(y - (18 - L.sy)*pica) > 1e-6)
		MovePen(L.sx * pica, (18 - L.sy)*pica);
	DrawLine((tx - L.sx)*pica, (ty - L.sy)*pica);
}

void PaintArc(arc L)
{
	SetColor(L.col);
	MovePen((L.rx + cos(L.start_angle)*L.R)*pica, (18 - L.ry + sin(L.start_angle)*L.R)*pica);
	DrawArc(L.R * pica, L.start_angle / pi * 180, (L.end_angle - L.start_angle) / pi * 180);
}

void PaintText(textbox L)
{
	SetColor(L.col);
	SetPointSize(L.pointsize);
	SetFont(L.font);
	MovePen(L.cx * pica - TextStringWidth(L.str) / 2, (18 - L.cy)*pica);
	DrawTextString(L.str);
}

/*Insert four lines which can not touch*/
void Add_Line(rect L)
{
	RepA[++cntr] = (line) {L.sx, L.sy, L.sx, L.ty, L.dangerous, L.col};
	RepA[++cntr] = (line) {L.sx, L.ty, L.tx, L.ty, L.dangerous, L.col};
	RepA[++cntr] = (line) {L.tx, L.ty, L.tx, L.sy, L.dangerous, L.col};
	RepA[++cntr] = (line) {L.tx, L.sy, L.sx, L.sy, L.dangerous, L.col};
}

void PaintRect(rect L)
{
	line temp;
	StartFilledRegion(1);
	temp = (line) {L.sx, L.sy, L.sx, L.ty, TRUE, L.fcol}; PaintLine(temp);
	temp = (line) {L.sx, L.ty, L.tx, L.ty, TRUE, L.fcol}; PaintLine(temp);
	temp = (line) {L.tx, L.ty, L.tx, L.sy, TRUE, L.fcol}; PaintLine(temp);
	temp = (line) {L.tx, L.sy, L.sx, L.sy, TRUE, L.fcol}; PaintLine(temp);
	EndFilledRegion();
	return ;
}

void Rotate_RLine(rline* L)
{
	(*L).angle += (*L).speed;
	if ((*L).mxang && (*L).angle > (*L).mxang)(*L).angle = (*L).mxang, (*L).speed = -(*L).speed;
	if ((*L).mnang && (*L).angle < (*L).mnang)(*L).angle = (*L).mnang, (*L).speed = -(*L).speed;
	return ;
}

void InitMap()
{
	int i;
	SetPenSize(2);
	A[++cnta] = (line) {1, 1, 31, 1, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {31, 1, 31, 17, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {31, 17, 1, 17, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {1, 17, 1, 1, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {1, 6, 6, 6, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {6, 6, 7, 5, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {7, 5, 26.5, 5, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {26, 7, 20, 7, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {28.5, 9, 16.8, 9, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {18, 5, 18, 7, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {5, 9, 11.2, 9, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {8, 7, 10, 7, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {1, 12, 11.75, 12, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {16.25, 12, 28, 12, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {28, 12, 31, 12, TRUE, (Color) {255, 185, 111}};
	A[++cnta] = (line) {16.25, 12, 28, 12, TRUE, (Color) {0, 0, 0}};
	A[++cnta] = (line) {14, 13, 14, 15, TRUE, (Color) {0, 0, 0}};

	Catch[++cnth] = (line) {7.25, 1, 7.25, 1.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7.75, 1, 7.75, 1.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.25, 1, 10.25, 1.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.75, 1, 10.75, 1.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7.25, 4.5, 7.25, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7.75, 4.5, 7.75, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.25, 4.5, 10.25, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.75, 4.5, 10.75, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7.25, 1.5, 7, 1.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7.75, 1.5, 8, 1.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.25, 1.5, 10, 1.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10.75, 1.5, 11, 1.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7, 1.75, 7, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8, 1.75, 8, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10, 1.75, 10, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11, 1.75, 11, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7, 2, 8, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10, 2, 11, 2, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7, 4.25, 7.25, 4.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8, 4.25, 7.75, 4.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10, 4.25, 10.25, 4.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11, 4.25, 10.75, 4.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7, 4.25, 7, 4, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8, 4.25, 8, 4, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10, 4.25, 10, 4, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11, 4.25, 11, 4, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {7, 4, 8, 4, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {10, 4, 11, 4, TRUE, (Color) {235, 64, 255}};

	/*********************************************************************************/
	Catch[++cnth] = (line) {8.75, 1, 8.75, 2.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.25, 1, 9.25, 2.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.75, 1, 11.75, 2.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.25, 1, 12.25, 2.5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.75, 3.5, 8.75, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.25, 3.5, 9.25, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.75, 3.5, 11.75, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.25, 3.5, 12.25, 5, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.5, 3, 9.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.5, 3, 12.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.75, 2.5, 8.5, 2.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.25, 2.5, 9.5, 2.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.75, 2.5, 11.5, 2.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.25, 2.5, 12.5, 2.75, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.5, 2.75, 8.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.5, 2.75, 9.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.5, 2.75, 11.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.5, 2.75, 12.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.75, 3.5, 8.5, 3.25, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.25, 3.5, 9.5, 3.25, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.75, 3.5, 11.5, 3.25, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.25, 3.5, 12.5, 3.25, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.5, 3.25, 8.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {9.5, 3.25, 9.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.5, 3.25, 11.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {12.5, 3.25, 12.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {8.5, 3, 9.5, 3, TRUE, (Color) {235, 64, 255}};
	Catch[++cnth] = (line) {11.5, 3, 12.5, 3, TRUE, (Color) {235, 64, 255}};

	B[++cntb] = (rline) {14, 10, 3 - 0.1, pi / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {14, 10, 3 - 0.1, pi * 3 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {14, 10, 3 - 0.1, pi * 5 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {14, 10, 3 - 0.1, pi * 7 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};

	B[++cntb] = (rline) {28.5, 6.5, 2.5 - 0.1, pi / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {28.5, 6.5, 2.5 - 0.1, pi * 3 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {28.5, 6.5, 2.5 - 0.1, pi * 5 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};
	B[++cntb] = (rline) {28.5, 6.5, 2.5 - 0.1, pi * 7 / 4, TRUE, pi / 200, 0, 0, (Color) {255, 0, 0}};

	B[++cntb] = (rline) {19, 4.5, 3.5 - 0.1, pi, TRUE, -pi / 50, pi, 0.01, (Color) {3, 64, 220}};

	Mark[++cntm] = (arc) {5.5, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {7.5, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {9, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {10.5, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {12.5, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {14, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {17, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {21, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {24, 3, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {26, 2, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {28, 2, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {30, 2, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {28.5, 5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {9, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {14, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {19, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {21, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {23, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {25, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {27, 6, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {7, 6.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {11, 7, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {17, 7, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {30, 7, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {4, 7.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {5.25, 7.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {7, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {9, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {14, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {19, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {21, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {23, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {25, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {28.5, 8, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {2.5, 9, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {4, 9, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {4, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {6.75, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {9, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {12, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {16, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {18.25, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {20, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {21.75, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {23.75, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {25, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {26.75, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {29, 10.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {14, 12, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {4, 13, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {7, 13, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {10, 13, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {15.5, 14, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {29, 14, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {4, 14.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {7, 14.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {10, 14.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {12.5, 14.5, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};

	Mark[++cntm] = (arc) {4, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {7, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {10, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {12.5, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {15.5, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};
	Mark[++cntm] = (arc) {29, 16, 0.125, -pi, pi, FALSE, (Color) {255, 156, 0}};


	C[++cntc] = (arc) {28.5, 6.5, 2.5, 0, -pi / 2, TRUE, (Color) {0, 0, 0}};

	C[++cntc] = (arc) {14, 10, 3, asin(1.0 / 3), pi - asin(1.0 / 3), TRUE, (Color) {0, 0, 0}};
	C[++cntc] = (arc) {14, 10, 3, -asin(2.0 / 3), -pi + asin(2.0 / 3), TRUE, (Color) {0, 0, 0}};
	C[++cntc] = (arc) {14, 10, 5, asin(3.0 / 5), pi - asin(3.0 / 5), TRUE, (Color) {0, 0, 0}};

	C[++cntc] = (arc) {19, 4.5, 0.5, 0, pi * 2, TRUE, (Color) {255, 0, 0}};

	D[++cntd] = (rect) {16.5, 12.5, 27, 16.5, FALSE, (Color) {0, 155, 153}, (Color) {175, 231, 231}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {1.5, 12.5, 3.5, 16.5, FALSE, (Color) {20, 121, 0}, (Color) {146, 220, 132}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {2, 1.5, 4, 5, FALSE, (Color) {20, 121, 0}, (Color) {146, 220, 132}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {2, 6, 3, 6.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {2, 11.5, 3, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5, 6, 5.5, 6.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5, 8.5, 5.5, 9, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5, 9, 5.5, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5, 11.5, 5.5, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8, 9, 8.5, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8, 11.5, 8.5, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {18, 9, 18.5, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {18, 11.5, 18.5, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {21.5, 9, 22, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {21.5, 11.5, 22, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {23.5, 9, 24, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {23.5, 11.5, 24, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {26.5, 9, 27, 9.5, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {26.5, 11.5, 27, 12, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);

	D[++cntd] = (rect) {4.25, 12, 4.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {4.75, 12, 5  , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5.25, 12, 5.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5.75, 12, 6  , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {6.25, 12, 6.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {6.75, 12, 7  , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {7.25, 12, 7.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {7.75, 12, 8  , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8.25, 12, 8.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8.75, 12, 9  , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {9.25, 12, 9.5, 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {9.75, 12, 10 , 12.25, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);

	D[++cntd] = (rect) {4.25, 16.75, 4.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {4.75, 16.75, 5  , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5.25, 16.75, 5.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {5.75, 16.75, 6  , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {6.25, 16.75, 6.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {6.75, 16.75, 7  , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {7.25, 16.75, 7.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {7.75, 16.75, 8  , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8.25, 16.75, 8.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {8.75, 16.75, 9  , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {9.25, 16.75, 9.5, 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {9.75, 16.75, 10 , 17, TRUE, (Color) {0, 0, 0}, (Color) {255, 246, 0}}; Add_Line(D[cntd]);

	Switch[++cnts] = (rect) {1, 8.5, 1.5, 9.5, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);
	Switch[++cnts] = (rect) {9, 9, 10, 9.5, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);
	Switch[++cnts] = (rect) {9, 11.5, 10, 12, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);
	Switch[++cnts] = (rect) {19.5, 9, 20.5, 9.3, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);
	Switch[++cnts] = (rect) {19.5, 11.7, 20.5, 12, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);
	Switch[++cnts] = (rect) {30.5, 10, 31, 11, FALSE, (Color) {205, 0, 236}, (Color) {14, 182, 41}}; Add_Line(Switch[cnts]);

	D[++cntd] = (rect) {25.5, 1.5, 26, 2, TRUE, (Color) {255, 151, 40}, (Color) {200, 255, 105}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {26.5, 1, 27, 1.5, TRUE, (Color) {255, 151, 40}, (Color) {200, 255, 105}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {27.5, 1, 28, 1.5, TRUE, (Color) {255, 151, 40}, (Color) {200, 255, 105}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {28.5, 1, 29, 1.5, TRUE, (Color) {255, 151, 40}, (Color) {200, 255, 105}}; Add_Line(D[cntd]);
	D[++cntd] = (rect) {29.5, 1, 30, 1.5, TRUE, (Color) {255, 151, 40}, (Color) {200, 255, 105}}; Add_Line(D[cntd]);

	F[++cntf] = (line) {2, 6.5, 2, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {2.5, 6.5, 2.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {3, 6.5, 3, 11.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {5, 6.5, 5, 8.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5.5, 6.5, 5.5, 8.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {5, 9.5, 8, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5.5, 9.5, 8.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8, 9.5, 5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8.5, 9.5, 5.5, 11.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {18, 9.5, 18, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {18.5, 9.5, 18.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {21.5, 9.5, 21.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {22, 9.5, 22, 11.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {18.5, 9.5, 21.5, 9.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {18.5, 11.5, 21.5, 11.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {4.25, 12.25, 4.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {4.5 , 12.25, 5   , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5.25, 12.25, 5.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5.5 , 12.25, 6   , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {6.25, 12.25, 6.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {6.5 , 12.25, 7   , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {7.25, 12.25, 7.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {7.5 , 12.25, 8   , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8.25, 12.25, 8.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8.5 , 12.25, 9   , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {9.25, 12.25, 9.75, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {9.5 , 12.25, 10  , 16.75, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {4.75, 12.25, 4.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5   , 12.25, 4.5 , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {5.75, 12.25, 5.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {6   , 12.25, 5.5 , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {6.75, 12.25, 6.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {7   , 12.25, 6.5 , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {7.75, 12.25, 7.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8   , 12.25, 7.5 , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {8.75, 12.25, 8.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {9   , 12.25, 8.5 , 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {9.75, 12.25, 9.25, 16.75, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {10  , 12.25, 9.5 , 16.75, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {23.5, 9.5, 23.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {24, 9.5, 24, 11.5, TRUE, (Color) {126, 208, 255}};

	F[++cntf] = (line) {26.5, 9.5, 26.5, 11.5, TRUE, (Color) {126, 208, 255}};
	F[++cntf] = (line) {27, 9.5, 27, 11.5, TRUE, (Color) {126, 208, 255}};

	T[++cntt] = (textbox) {"Growing Zone!", 22, 14.5, 25, (Color) {0, 180, 106}, "Consolas"};
	T[++cntt] = (textbox) {"Starting", 3, 3, 17, (Color) {0, 180, 106}, "Consolas"};
	T[++cntt] = (textbox) {"Zone", 3, 3.5, 17, (Color) {0, 180, 106}, "Consolas"};
	T[++cntt] = (textbox) {"Final", 2.5, 14.25, 17, (Color) {0, 180, 106}, "Consolas"};
	T[++cntt] = (textbox) {"Zone", 2.5, 14.75, 17, (Color) {0, 180, 106}, "Consolas"};

	for (i = 1; i <= cnta; ++i)PaintLine(A[i]);
	for (i = 1; i <= cnth; ++i)PaintLine(Catch[i]);
	for (i = 1; i <= cntb; ++i)PaintRLine(B[i]);
	for (i = 1; i <= cntc; ++i)PaintArc(C[i]);
	for (i = 1; i <= cntd; ++i)PaintRect(D[i]);
	for (i = 1; i <= cnts; ++i)PaintRect(Switch[i]);
	for (i = 1; i <= cntr; ++i)PaintLine(RepA[i]);
	for (i = 1; i <= cntf; ++i) if (sw[i]) PaintLine(F[i]);
	for (i = 1; i <= cntt; ++i)PaintText(T[i]);
	for (i = 1; i <= cntm; ++i)PaintArc(Mark[i]);
}

void Move_Catcher()
{
	double delta, speed = 0.02;
	if (T_mc[0] == 100 || T_mc[0] == -100) d_mc[0] = -d_mc[0];
	if (T_mc[1] == 100 || T_mc[1] == -100) d_mc[1] = -d_mc[1];
	T_mc[0] -= d_mc[0];
	T_mc[1] -= d_mc[1];
	if (T_mc[0] > 0 && T_mc[0] < 50)
	{
		delta = d_mc[0] * speed;
		Catch[1].ty += delta;
		Catch[2].ty += delta;
		Catch[3].ty += delta;
		Catch[4].ty += delta;
		Catch[5].sy -= delta;
		Catch[6].sy -= delta;
		Catch[7].sy -= delta;
		Catch[8].sy -= delta;
		Catch[9] .sy += delta; Catch[9] .ty += delta;
		Catch[10].sy += delta; Catch[10].ty += delta;
		Catch[11].sy += delta; Catch[11].ty += delta;
		Catch[12].sy += delta; Catch[12].ty += delta;
		Catch[13].sy += delta; Catch[13].ty += delta;
		Catch[14].sy += delta; Catch[14].ty += delta;
		Catch[15].sy += delta; Catch[15].ty += delta;
		Catch[16].sy += delta; Catch[16].ty += delta;
		Catch[17].sy += delta; Catch[17].ty += delta;
		Catch[18].sy += delta; Catch[18].ty += delta;
		Catch[19].sy -= delta; Catch[19].ty -= delta;
		Catch[20].sy -= delta; Catch[20].ty -= delta;
		Catch[21].sy -= delta; Catch[21].ty -= delta;
		Catch[22].sy -= delta; Catch[22].ty -= delta;
		Catch[23].sy -= delta; Catch[23].ty -= delta;
		Catch[24].sy -= delta; Catch[24].ty -= delta;
		Catch[25].sy -= delta; Catch[25].ty -= delta;
		Catch[26].sy -= delta; Catch[26].ty -= delta;
		Catch[27].sy -= delta; Catch[27].ty -= delta;
		Catch[28].sy -= delta; Catch[28].ty -= delta;
	}
	if (T_mc[0] < 0 && T_mc[0] > -50)
	{
		delta = d_mc[0] * speed;
		Catch[1].ty -= delta;
		Catch[2].ty -= delta;
		Catch[3].ty -= delta;
		Catch[4].ty -= delta;
		Catch[5].sy += delta;
		Catch[6].sy += delta;
		Catch[7].sy += delta;
		Catch[8].sy += delta;
		Catch[9] .sy -= delta; Catch[9] .ty -= delta;
		Catch[10].sy -= delta; Catch[10].ty -= delta;
		Catch[11].sy -= delta; Catch[11].ty -= delta;
		Catch[12].sy -= delta; Catch[12].ty -= delta;
		Catch[13].sy -= delta; Catch[13].ty -= delta;
		Catch[14].sy -= delta; Catch[14].ty -= delta;
		Catch[15].sy -= delta; Catch[15].ty -= delta;
		Catch[16].sy -= delta; Catch[16].ty -= delta;
		Catch[17].sy -= delta; Catch[17].ty -= delta;
		Catch[18].sy -= delta; Catch[18].ty -= delta;
		Catch[19].sy += delta; Catch[19].ty += delta;
		Catch[20].sy += delta; Catch[20].ty += delta;
		Catch[21].sy += delta; Catch[21].ty += delta;
		Catch[22].sy += delta; Catch[22].ty += delta;
		Catch[23].sy += delta; Catch[23].ty += delta;
		Catch[24].sy += delta; Catch[24].ty += delta;
		Catch[25].sy += delta; Catch[25].ty += delta;
		Catch[26].sy += delta; Catch[26].ty += delta;
		Catch[27].sy += delta; Catch[27].ty += delta;
		Catch[28].sy += delta; Catch[28].ty += delta;
	}
	if (T_mc[1] > 0 && T_mc[1] < 50)
	{
		delta = d_mc[1] * speed;
		Catch[29].ty += delta;
		Catch[30].ty += delta;
		Catch[31].ty += delta;
		Catch[32].ty += delta;
		Catch[33].sy -= delta;
		Catch[34].sy -= delta;
		Catch[35].sy -= delta;
		Catch[36].sy -= delta;
		Catch[37].sy += delta; Catch[37].ty += delta;
		Catch[38].sy += delta; Catch[38].ty += delta;
		Catch[39].sy += delta; Catch[39].ty += delta;
		Catch[40].sy += delta; Catch[40].ty += delta;
		Catch[41].sy += delta; Catch[41].ty += delta;
		Catch[42].sy += delta; Catch[42].ty += delta;
		Catch[43].sy += delta; Catch[43].ty += delta;
		Catch[44].sy += delta; Catch[44].ty += delta;
		Catch[45].sy += delta; Catch[45].ty += delta;
		Catch[46].sy += delta; Catch[46].ty += delta;
		Catch[47].sy -= delta; Catch[47].ty -= delta;
		Catch[48].sy -= delta; Catch[48].ty -= delta;
		Catch[49].sy -= delta; Catch[49].ty -= delta;
		Catch[50].sy -= delta; Catch[50].ty -= delta;
		Catch[51].sy -= delta; Catch[51].ty -= delta;
		Catch[52].sy -= delta; Catch[52].ty -= delta;
		Catch[53].sy -= delta; Catch[53].ty -= delta;
		Catch[54].sy -= delta; Catch[54].ty -= delta;
		Catch[55].sy -= delta; Catch[55].ty -= delta;
		Catch[56].sy -= delta; Catch[56].ty -= delta;
	}
	if (T_mc[1] < 0 && T_mc[1] > -50)
	{
		delta = d_mc[1] * speed;
		Catch[29].ty -= delta;
		Catch[30].ty -= delta;
		Catch[31].ty -= delta;
		Catch[32].ty -= delta;
		Catch[33].sy += delta;
		Catch[34].sy += delta;
		Catch[35].sy += delta;
		Catch[36].sy += delta;
		Catch[37].sy -= delta; Catch[37].ty -= delta;
		Catch[38].sy -= delta; Catch[38].ty -= delta;
		Catch[39].sy -= delta; Catch[39].ty -= delta;
		Catch[40].sy -= delta; Catch[40].ty -= delta;
		Catch[41].sy -= delta; Catch[41].ty -= delta;
		Catch[42].sy -= delta; Catch[42].ty -= delta;
		Catch[43].sy -= delta; Catch[43].ty -= delta;
		Catch[44].sy -= delta; Catch[44].ty -= delta;
		Catch[45].sy -= delta; Catch[45].ty -= delta;
		Catch[46].sy -= delta; Catch[46].ty -= delta;
		Catch[47].sy += delta; Catch[47].ty += delta;
		Catch[48].sy += delta; Catch[48].ty += delta;
		Catch[49].sy += delta; Catch[49].ty += delta;
		Catch[50].sy += delta; Catch[50].ty += delta;
		Catch[51].sy += delta; Catch[51].ty += delta;
		Catch[52].sy += delta; Catch[52].ty += delta;
		Catch[53].sy += delta; Catch[53].ty += delta;
		Catch[54].sy += delta; Catch[54].ty += delta;
		Catch[55].sy += delta; Catch[55].ty += delta;
		Catch[56].sy += delta; Catch[56].ty += delta;
	}
}

void Move_Blocks()
{
	double speed = 0.02; int i;
	if (dir_mb == 60 || dir_mb == -60) dy_mb = -dy_mb;
	dir_mb -= dy_mb;
	if (dir_mb > 0)
	{
		for (i = cntd; i > cntd - 5; --i)
		{
			D[i].sy += speed;
			D[i].ty += speed;
		}
		for (i = cntr; i > cntr - 20; --i)
		{
			RepA[i].sy += speed;
			RepA[i].ty += speed;
		}
	}
	if (dir_mb < 0)
	{
		for (i = cntd; i > cntd - 5; --i)
		{
			D[i].sy -= speed;
			D[i].ty -= speed;
		}
		for (i = cntr; i > cntr - 20; --i)
		{
			RepA[i].sy -= speed;
			RepA[i].ty -= speed;
		}
	}
}

void Repaint_Map()
{
	int i;
	/*for(i=1;i<=cnta;++i)PaintLine(A[i]);*/
	PaintLine(A[1]);
	for (i = 1; i <= cnth; ++i)PaintLine(Catch[i]);
	for (i = 1; i <= cntb; ++i)PaintRLine(B[i]);
	PaintArc(C[5]);
	/*for(i=1;i<=cntd;++i)PaintRect(D[i]);*/
	for (i = cntd; i > cntd - 5; --i) PaintRect(D[i]);
	for (i = 1; i <= cnts; ++i)PaintRect(Switch[i]);
	for (i = cntr - 43; i <= cntr; ++i)PaintLine(RepA[i]);
	for (i = 1; i <= cntf; ++i)
	{
		if (sw[i])PaintLine(F[i]), fl_rm[i] = 1;
		else if (fl_rm[i])
		{
			fl_rm[i] = 0;
			SetEraseMode(TRUE);
			PaintLine(F[i]);
			SetEraseMode(FALSE);
		}
	}
}

void Fix_line()
{
	int i;
	for (i = cntr - 19; i >= 1; --i)PaintLine(RepA[i]);
}

void Repaint_coins()
{
	int i;
	for (i = 1; i <= cntm; ++i)if (!eaten[i])PaintArc(Mark[i]);
}

double dis(double xa, double ya, double xb, double yb)
{
	return sqrt((xa - xb) * (xa - xb) + (ya - yb) * (ya - yb));
}

int CheckLine(double xa, double ya, double xb, double yb, double xc, double yc, double r)
{
	xb *= pica; xc *= pica; r *= pica;
	yb = (18 - yb) * pica; yc = (18 - yc) * pica;
	double xAB = xb - xa, yAB = yb - ya;
	double xAC = xc - xa, yAC = yc - ya;
	double xBC = xc - xb, yBC = yc - yb;
	double d = fabs(xAB * yAC - yAB * xAC) / dis(xb, yb, xc, yc);
	if (d > r) return 0;
	double dot1 = -xAB * xBC - yAB * yBC;
	double dot2 = xAC * xBC + yAC * yBC;
	if (dot1 >= 0 && dot2 >= 0) return 1;
	if (dis(xa, ya, xb, yb) < r || dis(xa, ya, xc, yc) < r) return 1;
	return 0;
}

int CheckArc(double xa, double ya, double cx, double cy, double startang, double endang, double R, double r)
{
	cx *= pica; cy = (18 - cy) * pica; R *= pica; r *= pica;
	double ang = atan((ya - cy) / (xa - cx)), temp;
	if (xa < cx)ang += pi;
	while (ang < -pi)ang += 2 * pi;
	while (ang > pi) ang -= 2 * pi;
	if (startang > endang) temp = startang, startang = endang, endang = temp;
	if (ang < startang || ang > endang)
	{
		double lx = cx + cos(endang) * R, ly = cy + sin(endang) * R;
		double rx = cx + cos(startang) * R, ry = cy + sin(startang) * R;
		if (dis(lx, ly, xa, ya) < r || dis(rx, ry, xa, ya) < r) return 1;
		return 0;
	}
	temp = dis(cx, cy, xa, ya);
	if (temp < R + r && temp > R - r) return 1;
	return 0;
}

int CheckRLine(double xa, double ya, double cx, double cy, double angle, double len, double r)
{
	return CheckLine(xa, ya, cx, cy, cx + len * cos(angle), cy - len * sin(angle), r);
}

void Finalfunc(int fl)
{
	finalfl = fl;
	mousefl = 0;
	cancelTimer(1);
	cancelTimer(2);
	cancelTimer(3);
	cancelTimer(4);
	cancelTimer(5);
	startTimer(6, 100);
	startTimer(7, 1500);
}

void Finalfunc_re()
{
	int i;
	cancelTimer(6);
	mousefl = 0;
	rect temprect = (rect) {10, 3, 22, 15, FALSE, (Color) {0, 0, 0}, (Color) {255, 255, 255}};
	textbox temptext1 = (textbox) {"CONGRATULATIONS!", 16, 5, 30, (Color) {255, 0, 0}, "Consolas"};
	textbox temptext2 = (textbox) {"Time:", 12.5, 7, 23, (Color) {236, 133, 0}, "Consolas"};
	textbox temptext3 = (textbox) {"Distance:", 12.5, 9, 23, (Color) {236, 133, 0}, "Consolas"};
	textbox temptext4 = (textbox) {"Coins:", 12.5, 11, 23, (Color) {236, 133, 0}, "Consolas"};
	textbox temptext5 = (textbox) {"Level:", 12.5, 13, 23, (Color) {236, 133, 0}, "Consolas"};
	textbox temptext6 = (textbox) {"1", 18, 7, 23, (Color) {28, 82, 155}, "Consolas"};
	textbox temptext7 = (textbox) {"1", 18, 9, 23, (Color) {28, 82, 155}, "Consolas"};
	textbox temptext8 = (textbox) {"1", 18, 11, 23, (Color) {28, 82, 155}, "Consolas"};
	textbox temptext9 = (textbox) {"1", 18, 13, 23, (Color) {28, 82, 155}, "Consolas"};
	if (!finalfl)strcpy(temptext1.str, "BOOM!"), temptext1.col = (Color) {60, 127, 51};
	sprintf(temptext6.str, "%d ms.", (clock() - t0) * 1000 / CLOCKS_PER_SEC);
	sprintf(temptext7.str, "%.2f inch.", sumd);
	sprintf(temptext8.str, "%02d / 64", coins);
	sprintf(temptext9.str, "%s", Level);
	Add_Line(temprect);
	PaintRect(temprect);
	PaintText(temptext1);
	PaintText(temptext2);
	PaintText(temptext3);
	PaintText(temptext4);
	PaintText(temptext5);
	PaintText(temptext6);
	PaintText(temptext7);
	PaintText(temptext8);
	PaintText(temptext9);
	for (i = cntr; i > cntr - 4; --i) PaintLine(RepA[i]);
	Inited = 0; started = 0;
	/*Beep(880,500);
	Beep(880/pw,250);
	Beep(880/pw/pw,250);
	Beep(880/pw/pw/pw,1500);*/
}

int Check_Trap()
{
	double x = ScaleXInches(lastmousex), y = ScaleYInches(lastmousey); int i;
	for (i = 1; i <= cnth; ++i)
	{
		if (!Catch[i].dangerous) continue;
		if (CheckLine(x, y, Catch[i].sx, Catch[i].sy, Catch[i].tx, Catch[i].ty, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = 1; i <= cntb; ++i)
	{
		if (!B[i].dangerous) continue;
		if (CheckRLine(x, y, B[i].sx, B[i].sy, B[i].angle, B[i].len, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = cntr; i > cntr - 20; --i)
	{
		if (!RepA[i].dangerous) continue;
		if (CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = 1; i <= cntf; ++i) if (sw[i])
		{
			if (!F[i].dangerous) continue;
			if (CheckLine(x, y, F[i].sx, F[i].sy, F[i].tx, F[i].ty, BALLSIZE)) { Finalfunc(0); return 1; }
		}
}

int	Check_Move()
{
	double x = ScaleXInches(lastmousex), y = ScaleYInches(lastmousey); int i;
	for (i = 1; i <= cnta; ++i)
	{
		if (!A[i].dangerous) continue;
		if (CheckLine(x, y, A[i].sx, A[i].sy, A[i].tx, A[i].ty, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = 1; i <= cntc; ++i)
	{
		if (!C[i].dangerous) continue;
		if (CheckArc(x, y, C[i].rx, C[i].ry, C[i].start_angle, C[i].end_angle, C[i].R, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = 1; i <= cntr - 20; ++i)
	{
		if (i > cntr - 44  && i <= cntr - 40 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			A[cnta - 2].col = (Color) {255, 255, 255};
			A[cnta - 2].dangerous = FALSE;
			PaintLine(A[cnta - 2]);
			Switch[1].fcol = (Color) {255, 138, 0};
		}
		if (i > cntr - 40  && i <= cntr - 36 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			sw[cntf - 3] = 0;
			Switch[2].fcol = (Color) {255, 138, 0};
		}
		if (i > cntr - 36  && i <= cntr - 32 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			sw[cntf - 2] = 0;
			Switch[3].fcol = (Color) {255, 138, 0};
		}
		if (i > cntr - 32  && i <= cntr - 28 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			sw[cntf - 1] = 0;
			Switch[4].fcol = (Color) {255, 138, 0};
		}
		if (i > cntr - 28  && i <= cntr - 24 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			sw[cntf] = 0;
			Switch[5].fcol = (Color) {255, 138, 0};
		}
		if (i > cntr - 24  && i <= cntr - 20 && CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE))
		{
			sw[1] = 0; sw[2] = 0; sw[3] = 0;
			Switch[6].fcol = (Color) {255, 138, 0};
		}

		if (!RepA[i].dangerous) continue;
		if (CheckLine(x, y, RepA[i].sx, RepA[i].sy, RepA[i].tx, RepA[i].ty, BALLSIZE)) { Finalfunc(0); return 1; }
	}
	for (i = 1; i <= cntm; ++i)
	{
		if (!eaten[i] && CheckArc(x, y, Mark[i].rx, Mark[i].ry, Mark[i].start_angle, Mark[i].end_angle, Mark[i].R, BALLSIZE))
		{
			SetEraseMode(TRUE);
			PaintArc(Mark[i]);
			SetEraseMode(FALSE);
			eaten[i] = 1; coins++;
		}
	}
	return 0;
}

int Inbox(double x, double y, rect* L)
{
	int temp; x /= pica; y /= pica;
	if ((*L).sx > (*L).tx) temp = (*L).sx, (*L).sx = (*L).tx, (*L).tx = temp;
	if ((*L).sy > (*L).ty) temp = (*L).sy, (*L).sy = (*L).ty, (*L).ty = temp;
	if (x >= (*L).sx && x <= (*L).tx && y <= (18 - (*L).sy) && y >= (18 - (*L).ty)) return 1;
	return 0;
}

void Timer_recall(int id)
{
	int i;
	if (id == 1)
	{
		SetEraseMode(TRUE);
		Repaint_Map();
		for (i = 1; i <= 9; ++i)Rotate_RLine(&B[i]);
		SetEraseMode(FALSE);
		Move_Catcher();
		Move_Blocks();
		Repaint_Map();
	}
	if (id == 2)
	{
		if (!fl_tr)for (i = 1; i <= cntf; i += 2) F[i].col = (Color) {0, 162, 255};
		else for (i = 1; i <= cntf; i += 2) F[i].col = (Color) {228, 0, 255};
		if (fl_tr)for (i = 2; i <= cntf; i += 2) F[i].col = (Color) {0, 162, 255};
		else for (i = 2; i <= cntf; i += 2) F[i].col = (Color) {228, 0, 255};
		fl_tr ^= 1;
		if (fl1_tr)Repaint_Map();
		else Fix_line();
	}
	if (id == 3)
	{
		for (i = 4; i <= cntf - 4; ++i) sw[i] ^= 1;
		fl1_tr ^= 1;
	}
	if (id == 4)
	{
		if (Inbox(ScaleXInches(lastmousex), ScaleYInches(lastmousey), &D[1]))
			BALLSIZE += 0.05;
		Check_Trap();
	}
	if (id == 5) {Repaint_coins();}
	if (id == 6)
	{
		SetEraseMode(TRUE);
		MovePen(ScaleXInches(lastmousex) + BALLSIZE * pica, ScaleYInches(lastmousey));
		DrawArc(BALLSIZE * pica, 0, 360);
		SetEraseMode(FALSE);
		if (ballfl_tr) SetPenColor("Red");
		else SetPenColor("Blue");
		MovePen(ScaleXInches(lastmousex) + BALLSIZE * pica, ScaleYInches(lastmousey));
		DrawArc(BALLSIZE * pica, 0, 360);
		ballfl_tr ^= 1;
	}
	if (id == 7)
	{
		cancelTimer(6);
		cancelTimer(7);
		Finalfunc_re();
	}
	return ;
}

void Init()
{
	if (Inited) return ;
	sumd = 0;
	BALLSIZE = last_size = INITBALLSIZE;
	mousefl = 0, started = 0, t0 = 0, coins = 0;
	lastmousex = 0, lastmousey = 0;
	memset(sw, 0, sizeof(sw));
	sw[1] = sw[2] = sw[3] = sw[40] = sw[41] = sw[42] = sw[43] = 1;
	memset(eaten, 0, sizeof(eaten));

	T_mc[0] = 100, T_mc[1] = 0;
	d_mc[0] = -1, d_mc[1] = +1;
	dir_mb = 0, dy_mb = -1;
	memset(fl_rm, 0, sizeof(fl_rm));
	fl_tr = 0, fl1_tr = 0;
	cnta = cntb = cntc = cntr = cntd = cntf = cnts = cnth = cntt = cntm = 0;
	rect temprt = (rect) {0, 0, 100, 100, TRUE, (Color) {0, 0, 0}, (Color) {255, 255, 255}};
	PaintRect(temprt);
	InitMap();
	startTimer(1, 10);
	startTimer(2, 100);
	startTimer(3, 900);
	startTimer(4, 50);
	startTimer(5, 250);
	Inited = 1;
}

void Mouse_recall(int xx, int yy, int button, int event)
{
	int i;
	if (event == BUTTON_DOWN)
	{
		if (!started && Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[3]))
		{
			Init();
			mousefl = !mousefl, started = 1, t0 = clock();
			D[3].fcol = (Color) {146, 220, 132}; PaintRect(D[2]); PaintRect(D[3]);
			for (i = 5; i <= 12; ++i) PaintLine(RepA[i]);
			for (i = 1; i <= cntt; ++i)PaintText(T[i]);
		}
		if (started && mousefl && Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[2]))Finalfunc(1);
	}

	if (event == MOUSEMOVE && !mousefl && !started)
	{
		if (!started && Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[3]))D[3].fcol = (Color) {87, 132, 79};
		else if (!started && !Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[3]))D[3].fcol = (Color) {146, 220, 132};
		PaintRect(D[2]); PaintRect(D[3]);
		for (i = 5; i <= 12; ++i) PaintLine(RepA[i]);
		for (i = 2; i <= cntt; ++i)PaintText(T[i]);
	}

	if (event == MOUSEMOVE && mousefl)
	{
		if (started && Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[2]))D[2].fcol = (Color) {87, 132, 79};
		else if (started && !Inbox(ScaleXInches(xx), ScaleYInches(yy), &D[2]))D[2].fcol = (Color) {146, 220, 132};
		if (lastmousex && lastmousey)
			sumd += dis(ScaleXInches(lastmousex), ScaleYInches(lastmousey), ScaleXInches(xx), ScaleYInches(yy));
		SetEraseMode(TRUE);
		MovePen(ScaleXInches(lastmousex) + last_size * pica, ScaleYInches(lastmousey));
		DrawArc(last_size * pica, 0, 360);
		SetEraseMode(FALSE);
		PaintRect(D[1]); PaintRect(D[2]); PaintRect(D[3]);
		for (i = 1; i <= 12; ++i) PaintLine(RepA[i]);
		SetPenColor("Red");
		MovePen(ScaleXInches(xx) + BALLSIZE * pica, ScaleYInches(yy));
		DrawArc(BALLSIZE * pica, 0, 360);
		last_size = BALLSIZE;
		lastmousex = xx, lastmousey = yy;
		Check_Move(); Check_Trap();
	}
}

void Main()
{
	pi = acos(-1); pw = pow(2, 1.0 / 12);
	char ch;
	SetWindowSize(16, 9);
	SetWindowTitle("Rescue The Ball");
	InitConsole();
	printf("Choose a levle!\n");
	printf("1.Too Simple 2.Easy 3.Normal 4.Advanced 5.Hard 6.Crazy\n");
	while (ch = getch())
	{
		if (ch >= '1' && ch <= '6') break;
		else printf("Try again!\n");
	}
	if (ch == '1') INITBALLSIZE = BALLSIZE = last_size = 0.2, Level = "Too Simple";
	if (ch == '2') INITBALLSIZE = BALLSIZE = last_size = 0.65, Level = "Easy";
	if (ch == '3') INITBALLSIZE = BALLSIZE = last_size = 0.75, Level = "Normal";
	if (ch == '4') INITBALLSIZE = BALLSIZE = last_size = 0.82, Level = "Advanced";
	if (ch == '5') INITBALLSIZE = BALLSIZE = last_size = 0.86, Level = "Hard";
	if (ch == '6') INITBALLSIZE = BALLSIZE = last_size = 0.9, Level = "Crazy";
	FreeConsole();
	InitGraphics();
	InitMap();
	registerTimerEvent(Timer_recall);
	registerMouseEvent(Mouse_recall);
	startTimer(1, 10);
	startTimer(2, 100);
	startTimer(3, 900);
	startTimer(4, 50);
	startTimer(5, 250);
	Inited = 1;
	return ;
}
