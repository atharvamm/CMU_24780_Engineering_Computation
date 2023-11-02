#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"

const double YsPi=3.1415927;

void DrawCircle(double cx,double cy,double rad,unsigned char r,unsigned char g,unsigned char b)
{
	glColor3ub(r,g,b);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; ++i)
	{
		double ang=YsPi*(double)i/32.0;
		double s=sin(ang);
		double c=cos(ang);
		double x=cx+rad*c;
		double y=cy+rad*s;
		glVertex2d(x,y);
	}
	glEnd();
}

void PrintBoard(char b[9])
{
	for(int y=0; y<3; ++y)
	{
		for(int x=0; x<3; ++x)
		{
			printf("%c",b[y*3+x]);
			if(2!=x)
			{
				printf("|");
			}

			if(b[y*3+x]=='O')
			{
				double cx=x*200+100;
				double cy=y*200+100;
				DrawCircle(cx,cy,80,0,0,0);
			}
			else if(b[y*3+x]=='X')
			{
				double cx=x*200+100;
				double cy=y*200+100;
				glBegin(GL_LINES);
				glVertex2i(cx-80,cy-80);
				glVertex2i(cx+80,cy+80);
				glVertex2i(cx-80,cy+80);
				glVertex2i(cx+80,cy-80);
				glEnd();
			}
		}
		printf("\n");
		if(2!=y)
		{
			printf("-+-+-\n");
		}
	}

	glBegin(GL_LINES);
	glVertex2i(0,200);
	glVertex2i(600,200);

	glVertex2i(0,400);
	glVertex2i(600,400);

	glVertex2i(200,  0);
	glVertex2i(200,600);

	glVertex2i(400,  0);
	glVertex2i(400,600);
	glEnd();
}

bool Check3inaRow(char board[9],char turn)
{
	if((turn==board[0] && turn==board[1] && turn==board[2]) ||
	   (turn==board[3] && turn==board[4] && turn==board[5]) ||
	   (turn==board[6] && turn==board[7] && turn==board[8]) ||

	   (turn==board[0] && turn==board[3] && turn==board[6]) ||
	   (turn==board[1] && turn==board[4] && turn==board[7]) ||
	   (turn==board[2] && turn==board[5] && turn==board[8]) ||

	   (turn==board[0] && turn==board[4] && turn==board[8]) ||
	   (turn==board[2] && turn==board[4] && turn==board[6]))
	{
		return true;
	}
	return false;
}

int main(void)
{
	FsOpenWindow(0,0,600,600,1);

	char board[9];
	for(auto &b : board)
	{
		b=' ';
	}

	char turn='X';
	for(;;)
	{
		FsPollDevice();

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		PrintBoard(board);
		FsSwapBuffers();

		printf("%c's turn.\n",turn);
		printf("Enter X and Y:");
		int x,y;
		scanf("%d%d",&x,&y);

		if(0<=x && x<3 && 0<=y && y<3)
		{
			if(' '!=board[y*3+x])
			{
				printf("Already Taken.\n");
				continue;
			}

			board[y*3+x]=turn;

			if(true==Check3inaRow(board,turn))
			{
				printf("%c wins!\n",turn);
				PrintBoard(board);
				auto t0=time(NULL);
				while(time(NULL)-t0<3)
				{
				}
				break;
			}

			if('X'==turn)
			{
				turn='O';
			}
			else
			{
				turn='X';
			}
		}
		else
		{
			printf("X and Y needs to be between 0 and 2.\n");
		}
	}
	return 0;
}
