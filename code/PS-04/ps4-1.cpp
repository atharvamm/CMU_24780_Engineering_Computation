#include <time.h>
#include <stdio.h>
#include "fssimplewindow.h"
#include <vector>
#include <cmath>
#include <iostream>

const double PI = 3.1415927;

void DrawCircle(double cx,double cy)
{   
    double rad = 75.0;
	glColor3ub(0,0,0);
    glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; ++i)
	{
		double ang=PI*(double)i/32.0;
		double s=sin(ang);
		double c=cos(ang);
		double x=cx+rad*c;
		double y=cy+rad*s;
		glVertex2d(x,y);
	}
	glEnd();
}


void DrawCross(double cx,double cy)
{   
	glColor3ub(0,0,0);
	glBegin(GL_LINES);
    double x1,y1,x2,y2,x3,y3,x4,y4;
    
    x1 = cx - 50; x4 = x1;
    x2 = cx + 50; x3 = x2;

    y1 = cy - 50; y3 = y1; 
    y2 = cy + 50; y4 = y2;

    glVertex2f(x1,y1);
    glVertex2f(x2,y2);

    glVertex2f(x3,y3);
    glVertex2f(x4,y4);
	glEnd();
}

bool checkBoard(char board[9]){
	for (int i = 0 ; i < 9; i++){
		if ('X' != board[i] && 'O' != board[i]){
			return false;
		}
}
return true;
}

void displayBoard(std::vector<std::vector<int>> oCoords,std::vector<std::vector<int>> xCoords){
    double x,y;
    // Draw Board
    glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0b1111111100000000);
	glColor3ui(25,100,50);

	glBegin(GL_LINES);
	// Vertical Line 1
	glVertex2i(266,25);
	glVertex2i(266,575);
	// Vertical Line 2
	glVertex2i(533,25);
	glVertex2i(533,575);
	// Horizontal Line 1
	glVertex2i(50,200);
	glVertex2i(750,200);
	// Horizontal Line 2
	glVertex2i(50,400);
	glVertex2i(750,400);
	glEnd();

	for (std::vector<int> coord: oCoords){
		x = 800/6 + coord.at(0)*(800/3);
		y = 600/6 + coord.at(1)*(600/3);
		DrawCircle(x,y);
		}

	for (std::vector<int> coord: xCoords){
		x = 800/6 + coord.at(0)*(800/3);
		y = 600/6 + coord.at(1)*(600/3);
		DrawCross(x,y);
		}

	FsSwapBuffers();
	FsSleep(200);
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
		}
		printf("\n");
		if(2!=y)
		{
			printf("-+-+-\n");
		}
	}
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
	std::vector<std::vector<int> > oCoords, xCoords;
	std::vector<int> temp;
	char board[9];
	for(auto &b : board)
	{
		b=' ';
	}

	char turn='X';
	FsOpenWindow(50,50,800,600,1);
	for(;;)
	{
		displayBoard(oCoords,xCoords);

		printf("%c's turn.\n",turn);
		printf("Enter X and Y:");
		int x,y;
		scanf("%d%d",&x,&y);
		temp = {x,y};

		if(0<=x && x<3 && 0<=y && y<3)
		{
			if(' '!=board[y*3+x])
			{
				printf("Already Taken.\n");
				continue;
			}

			board[y*3+x]=turn;

			if('X'==turn)
			{
				xCoords.push_back(temp);
			}
			else
			{
				oCoords.push_back(temp);
			}


			if(true==Check3inaRow(board,turn))
			{
				printf("%c wins!\n",turn);
				displayBoard(oCoords,xCoords);
				auto t0=time(NULL);
				while(time(NULL)-t0<3)
				{
				}
				break;
			}

			if (false != checkBoard(board)){
				std::cout<<"Draw!!!!"<<std::endl;
				displayBoard(oCoords,xCoords);
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