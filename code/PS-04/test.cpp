#include <cmath>
#include "fssimplewindow.h"
#include <iostream>
#include <random>
const double Pi=3.1415927;
const int nBoxes = 5;
const int winH = 600,winW = 800; 
const double g = 9.81;
int shooterStartX = 50,shooterStartY = 500,shooterH = 25,shooterW = 25,shooterL = 30, targetSpeed = 10;
const int bH = 100, bW = 100;

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

    for (int i = 0; i < 20; i++){
    double rad = (rand() % 75) + 5; 
    float r = (rand() % 256) / 255.0; 
    float g = (rand() % 256) / 255.0; 
    float b = (rand() % 256) / 255.0; 

    double dx = (rand() % 51) - 25; 
    double dy = (rand() % 51) - 25; 
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


void calculateMisCoord(std::vector<int>& x, std::vector<int>& y, std::vector<int>& v, int ind) {
    // Get the initial coordinates and time step for the selected object
    double x0 = x.at(ind);
    double y0 = y.at(ind);
    double vy = v.at(ind);
    double vx = 40; 
    double dt = 0.2;
    std::cout << "IP:" << x0 << y0 << vy<<std::endl;


    // Calculate new velocities (updated using gravitational acceleration)
    vy = vy + g * 0.001 * dt;  // Update vertical velocity due to gravity

    // Calculate new positions
    double x_new = x0 + vx * dt;
    double y_new = y0 + vy * dt;

    // Update the vectors with new values
    x.at(ind) = x_new;
    y.at(ind) = y_new;
    v.at(ind) = vy;
    std::cout << "OP:" << x_new << y_new << vy<<std::endl;
}



int main(void){
    int bX[nBoxes],bY[nBoxes],bState[nBoxes], targetInd, targetDir = 1, lastMissile = 0;
    double theta = 45;
    std::vector<int> missileX,missileY,mState,missileV,animation;
    std::vector<float> coords;
    bool gameOver = false,condn1,condn2,condn3;
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
            missileV.push_back(0);
            mState.push_back(1);
			break;
		}

    for (int j = lastMissile; j < (mState.size() - lastMissile); j++){
        if (0 != mState[j]){
            std::cout<<"Before "<<"j:"<<j<<" "<<"x:"<<missileX.at(j)<<" y:"<<missileY.at(j)<<std::endl;
        }
    }


    // Get Missile and Target Location
    targetDir = getTargetLoc(bY, targetInd,targetDir);
    for (int i = lastMissile; i < (int)mState.size(); i++){
        if (0 != mState[i]){
            calculateMisCoord(missileX,missileY,missileV,i);
        }
    }

    // Check Collision
    for (int i = 0; i < nBoxes; i++){
        for (int j = lastMissile; j < (mState.size() - lastMissile); j++){
            condn1 = 0!=mState.at(j);
            condn2 = 0!=bState[i];
            condn3 = true == CheckCollision(missileX.at(j),missileY.at(j),bX[i],bY[i],bW,bH);
            if(condn1 && condn2 && condn3){
                bState[i] = 0;
                mState.at(j) = 0;
                lastMissile ++;
                if (targetInd == i){gameOver = true;animation = {bX[targetInd],bY[targetInd]};}
            }
        }
    }

    for (int i = 0; i < nBoxes; i++){
        if (false != bState[i]){
            DrawBoxes(bX[i],bY[i],100,100,targetInd,i);
        }
    }
    for (int j = lastMissile; j < (mState.size() - lastMissile); j++){
        if (0 != mState[j]){
            DrawMissile(missileX.at(j),missileY.at(j),j);
        }
    }

    for (int j = lastMissile; j < (mState.size() - lastMissile); j++){
        if (0 != mState[j]){
            std::cout<<"After "<<"j:"<<j<<" "<<"x:"<<missileX.at(j)<<" y:"<<missileY.at(j)<<std::endl;
        }
    }

    coords = getShooterPt(theta);
    DrawShooter(coords.at(0),coords.at(1));
    // if (false != gameOver){TargetAnimation(bX[targetInd],bY[targetInd]);break;}
    if (false != gameOver){TargetAnimation(animation.at(0),animation.at(1));}
    FsSwapBuffers();
    FsSleep(25);
    
}
std::cout<<"Game Over!!!!"<<std::endl;
return 0;
}











