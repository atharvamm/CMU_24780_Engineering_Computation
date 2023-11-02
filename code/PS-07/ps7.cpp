#include <stdio.h>
#include <time.h>
#include <math.h>
#include "fssimplewindow.h"
#include <iostream>
#include <vector>

const double YsPi=3.1415927;

class Mark{

private:
	char state;

public:
	Mark(){
		state = ' ';
	}

	char getState(){
		return state;
	}

	void setState(char newState){
		state = newState;
	}

	void DrawCircle(double cx,double cy, int lb)
	{   
		double rad = 80.0;
		if (0 == lb){
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1,0xff00);
		}
		else{
			glDisable(GL_LINE_STIPPLE);
		}
		glColor3ub(0,0,0);
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
	
	void DrawCross(double cx,double cy, int lb)
	{   
		
		if (0 == lb){
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1,0xff00);
		}
		else{
			glDisable(GL_LINE_STIPPLE);
		}
		glColor3ub(0,0,0);
		glBegin(GL_LINES);
		double x1,y1,x2,y2,x3,y3,x4,y4;
		
		x1 = cx - 80; x4 = x1;
		x2 = cx + 80; x3 = x2;

		y1 = cy - 80; y3 = y1; 
		y2 = cy + 80; y4 = y2;

		glVertex2f(x1,y1);
		glVertex2f(x2,y2);

		glVertex2f(x3,y3);
		glVertex2f(x4,y4);
		glEnd();
	}

	void DrawPoint(int cx, int cy){
		glColor3ub(0,0,0);
		glPointSize(4);
		glBegin(GL_POINTS);
		glVertex2i(cx,cy);
		glEnd();
	}

	void drawState(int cx, int cy, char turn, int lb){
		switch (turn){
			case 'X':
				DrawCross(cx,cy,lb);
				break;
			case 'O':
				DrawCircle(cx,cy,lb);
				break;
			default:
				DrawPoint(cx,cy);
				break;
		}
	}
};


class Board{

private:
	Mark states[9];

public:

	Board(){
		;
	}


	int mouse2colInd(int mx, int my){
		int x,y;
	
		mx = (mx>600) ? 600 : (mx < 50) ? 50 : mx;
		my = (my>600) ? 600 : (my < 50) ? 50 : my;

		x = mx/201;
		y = my/201;
		return 3*y + x;
	}

	Mark& getStateObj(int stateInd){
		return states[stateInd];
	}


	std::vector<int> colInd2wind(int colInd){

		std::vector<int> windCord;
		windCord.push_back(0);
		windCord.push_back(0);
		int y = colInd / 3;
		int x = colInd % 3;

		windCord[0] = x*200 + 100;
		windCord[1] = y*200 + 100;
		return windCord;
	}

	char Check3inaRow(char turn)
	{
	if((turn==states[0].getState() && turn==states[1].getState() && turn==states[2].getState()) ||
	   (turn==states[3].getState() && turn==states[4].getState() && turn==states[5].getState()) ||
	   (turn==states[6].getState() && turn==states[7].getState() && turn==states[8].getState()) ||

	   (turn==states[0].getState() && turn==states[3].getState() && turn==states[6].getState()) ||
	   (turn==states[1].getState() && turn==states[4].getState() && turn==states[7].getState()) ||
	   (turn==states[2].getState() && turn==states[5].getState() && turn==states[8].getState()) ||

	   (turn==states[0].getState() && turn==states[4].getState() && turn==states[8].getState()) ||
	   (turn==states[2].getState() && turn==states[4].getState() && turn==states[6].getState()))
	{
		return turn;
	}
	return (char)0;
}

	bool checkBoard(){
	for (int i = 0 ; i < 9; i++){
		char cur_state = states[i].getState();
		if (' ' ==  cur_state){
			return false;
		}
	}
	return true;
	}

	void DrawBoard()
	{
	for(int y=0; y<3; ++y)
	{
		for(int x=0; x<3; ++x)
		{	

			if(states[y*3+x].getState()=='O')
			{
				std::vector<int> coords = this -> colInd2wind(y*3+x);
				states[y*3+x].drawState(coords[0],coords[1],'O',1);
			}
			else if(states[y*3+x].getState()=='X')
			{	
				std::vector<int> coords = this -> colInd2wind(y*3+x);
				states[y*3+x].drawState(coords[0],coords[1],'X',1);
			}
		}
	}
	glDisable(GL_LINE_STIPPLE);
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
	
};


int main(void)
{	
	Board ticTacToe;
	FsOpenWindow(0,0,600,600,1);
	int lb,mb,rb,mx,my;
	char turn='X';

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	ticTacToe.DrawBoard();
	FsSwapBuffers();

	for(;;)
	{
		FsPollDevice();

		FsGetMouseEvent(lb,mb,rb,mx,my);
		int colInd = ticTacToe.mouse2colInd(mx,my);

		std::vector<int> coords = ticTacToe.colInd2wind(colInd);
		int x = coords[0],y = coords[1];

		if(' '!=ticTacToe.getStateObj(colInd).getState())
		{
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			ticTacToe.DrawBoard();
			FsSwapBuffers();
			continue;
		}
		else{
		if (0 == lb){
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			ticTacToe.getStateObj(colInd).drawState(coords[0],coords[1],turn,0);
			ticTacToe.DrawBoard();
			FsSwapBuffers();
		}

		else{

		ticTacToe.getStateObj(colInd).setState(turn);

		if((char)0!=ticTacToe.Check3inaRow(turn))
		{
			printf("%c wins!\n",turn);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			ticTacToe.DrawBoard();
			FsSwapBuffers();
			auto t0=time(NULL);
			while(time(NULL)-t0<3)
			{
			}
			break;
		}

		if(true==ticTacToe.checkBoard())
		{
			printf("Draw!!!\n");
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			ticTacToe.DrawBoard();
			FsSwapBuffers();
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
		}



	}
	return 0;
}