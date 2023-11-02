#include <cmath>
#include "fssimplewindow.h"
#include <iostream>
#include <random>
#include <cstdlib>
const double Pi=3.1415927;
const int nBoxes = 6;
const int winH = 600,winW = 800; 
const double g = 9.81;
int shooterStartX = 50,shooterStartY = 500,shooterH = 25,shooterW = 25,shooterL = 30, targetSpeed = 10;
const int bH = 100, bW = 100;
const double m2pix = 0.25;

void DrawCircle(double cx,double cy,double rad, float r, float g, float b)
{   
    glColor3f(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0; i<64; ++i)
	{
		double ang=Pi*(double)i/32.0;
		double s=sin(ang);
		double c=cos(ang);
		double x=cx+rad*c;
		double y=cy+rad*s;
		glVertex2d(x,y);
	}
	glEnd();
}

void TargetAnimation(double cx, double cy) {
    for (int i = 0; i < 200; i++){
    double rad = (rand() % 75) + 2; 
    float r = (rand() % 256) / 255.0; 
    float g = (rand() % 256) / 255.0; 
    float b = (rand() % 256) / 255.0; 

    double dx = (rand() % 11) - 5; 
    double dy = (rand() % 11) - 5; 
    cx += dx;
    cy += dy;
    DrawCircle(cx, cy, rad, r, g, b);
    }
    }


void DrawBoxes(int x,int y,int w,int h,int targetInd, int ind)
{   
	if (targetInd == ind){
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);
    }
	glBegin(GL_QUADS);
	glVertex2i(x,y);
	glVertex2i(x+w,y);
	glVertex2i(x+w,y+h);
	glVertex2i(x,y+h);
	glEnd();
}

void DrawCircle(double cx,double cy,double rad,unsigned char r,unsigned char g,unsigned char b)
{
	glColor3ub(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0; i<64; ++i)
	{
		double ang=Pi*(double)i/32.0;
		double s=sin(ang);
		double c=cos(ang);
		double x=cx+rad*c;
		double y=cy+rad*s;
		glVertex2d(x,y);
	}
	glEnd();
}

std::vector<float> getShooterPt(int theta){
    float px,py;
    float radians = ((theta)/(float)180)*Pi;

    if (0 == theta){px = shooterL + shooterStartX + shooterW/2;py = shooterStartY + shooterH/2;}
    else if (90 == theta){px = shooterStartX + shooterW/2;py = shooterStartY - shooterL + shooterH/2;}
    else{
        px = shooterL * cos(radians) + shooterStartX + shooterW/2;
        py = shooterStartY - (shooterL * sin(radians)) + shooterH/2;
    }
    std::vector<float> coords = {px,py};
    return coords;
}

void DrawShooter(float px, float py){
    glColor3ub(0,0,255);
    glBegin(GL_QUADS);
	glVertex2i(shooterStartX,shooterStartY);
	glVertex2i(shooterStartX+shooterW,shooterStartY);
	glVertex2i(shooterStartX+shooterW,shooterStartY+shooterH);
	glVertex2i(shooterStartX,shooterStartY+shooterH);
	glEnd();

    glBegin(GL_LINES);
    glVertex2f(shooterStartX + shooterW/2,shooterStartY + shooterH/2);
    glVertex2f(px,py);
    glEnd();
}

int getTargetLoc(int bY[nBoxes],int targetInd,int targetDir){
    int curY = bY[targetInd];
    if (curY > 0){
        if (curY > 500){curY = 500;targetDir = 1;}
        else{
            if (targetDir == 1){curY = curY - targetSpeed;}
            else{ curY = curY + targetSpeed;}

        }
    }
    else{curY = 50; targetDir = -1;}
    bY[targetInd] = curY;
    return targetDir;
}

bool CheckCollision(int mx,int my,int tx,int ty,int tw,int th)
{
	return (
		tx<=mx && mx<=tx+tw &&
		ty<=my && my<=ty+th
	);
}

void DrawMissile(int mx,int my,int numMissile){
    float r,g,b;
    float rad = 4;
    switch(numMissile){
    case 0:
        r = 0,g = 0,b = 1;
        break;
    case 1:
        r = 0,g = 1,b = 1;
        break;
    case 2:
        r = 1, g = 1,b = 0;
        break;
    case 3:
        r = 1, g = 0, b = 1;
        break;
    default:
        r = 1, g = 0, b = 0;
        break;
    }
    DrawCircle(mx,my,rad,r,g,b);
}


void calculateMisCoord(std::vector<int>& x, std::vector<int>& y, std::vector<float>& vx, std::vector<float>& vy, int ind) {
    double x0 = x.at(ind);
    double y0 = y.at(ind);
    double vx0 = vx.at(ind);
    double vy0 = vy.at(ind); 
    double dtx = 0.2;
    double dty = dtx * m2pix;
    double vy_new = vy0 + g * dty; 
    double x_new = x0 + vx0 * dtx;
    double y_new = y0 + vy_new * dtx;
    x.at(ind) = x_new;
    y.at(ind) = y_new;
    vy.at(ind) = vy_new;
}



int main(void){
    int bX[nBoxes],bY[nBoxes],bState[nBoxes], targetInd, targetDir = 1;
    float radians = 0.0,theta = 45,temp_vx, temp_vy;
    std::vector<int> missileX,missileY,mState,animation;
    std::vector<float> coords,missileVx,missileVy;
    bool gameOver = false,condn1,condn2,condn3;
    double sumTimePassed = 0.0;
    srand(time(NULL));
    

    targetInd = rand() % nBoxes;
    bX[targetInd] = 700;
    bY[targetInd] = 540;
    bState[targetInd] = true;

    for (int i = 0; i < nBoxes; i++){
        if (targetInd == i) continue;
        bX[i] = rand()%600 + 125;
        bY[i] = rand()%500;
        bState[i] = true;
    }

    FsOpenWindow(50,50,800,600,1);
    for(;;)
	{   
        if (false!= gameOver){
            FsPassedTime();
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
        {
			break;
            }
        switch(key)
		{
		case FSKEY_UP:
            theta = theta + 3;
            if (theta > 90){theta = 90;}
			break;
		case FSKEY_DOWN:
            theta = theta - 3;
            if (theta < 0) {theta = 0;}
			break;
		case FSKEY_SPACE:
            coords = getShooterPt(theta);
            missileX.push_back(coords.at(0));
            missileY.push_back(coords.at(1));
            radians = ((theta)/(float)180)*Pi;
            temp_vx = 40*cos(radians),temp_vy = -40*sin(radians);
            missileVx.push_back(temp_vx);
            missileVy.push_back(temp_vy);
            mState.push_back(1);
            DrawMissile(coords.at(0),coords.at(1),(int)mState.size() - 1);
			break;
		}

    targetDir = getTargetLoc(bY, targetInd,targetDir);
    for (int i = 0; i < (int)mState.size(); i++){
        if (0 != mState[i]){
            calculateMisCoord(missileX,missileY,missileVx,missileVy,i);
        }
    }


    for (int i = 0; i < nBoxes; i++){
        for (int j = 0; j < mState.size(); j++){
            condn1 = 0!=mState.at(j);
            condn2 = 0!=bState[i];
            condn3 = true == CheckCollision(missileX.at(j),missileY.at(j),bX[i],bY[i],bW,bH);
            if(condn1 && condn2 && condn3){
                bState[i] = 0;
                mState.at(j) = 0;
                if (targetInd == i){gameOver = true;animation = {bX[targetInd],bY[targetInd]};}
            }
        }
    }

    for (int i = 0; i < nBoxes; i++){
        if (false != bState[i]){
            DrawBoxes(bX[i],bY[i],100,100,targetInd,i);
        }
    }
    for (int j = 0; j < mState.size(); j++){
        if (0 != mState.at(j)){
            if (missileX.at(j) > winW || missileY.at(j) > winH){
                mState.at(j) = 0;
            }
            else{DrawMissile(missileX.at(j),missileY.at(j),j);}
        }
    }

    coords = getShooterPt(theta);
    DrawShooter(coords.at(0),coords.at(1));
    if (false != gameOver){
        TargetAnimation(animation.at(0),animation.at(1));
        sumTimePassed  = sumTimePassed + (double)FsPassedTime();
        if (sumTimePassed > 150){break;}
        }
    FsSwapBuffers();
    FsSleep(25);
    
}
std::cout<<"Game Over!!!!"<<std::endl;
return 0;
}











