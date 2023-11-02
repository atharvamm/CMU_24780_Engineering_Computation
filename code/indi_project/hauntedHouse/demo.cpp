#include "fssimplewindow.h"
#include <iostream>
#include <string>
#include "yssimplesound.h"
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>
#include <ctime>

const double PI = 3.14159265358979323846;


void sleepTime(int timeSleep){
    auto t0=time(NULL);
    while(time(NULL)-t0<timeSleep)
    {
    }
}

// PRIMITIVES_H
class Point{
    
    public:
        double x,y;
        Point(double initX, double initY);

        Point();

        void translatePoint(double dist, char dir, bool positive);
        void rotatePoint(double cx, double cy, double degrees, bool clockwise);
        void drawPoint();
};

Point::Point(double initX, double initY){
            x = initX;
            y = initY;
        }

Point::Point(){
        x = 0;
        y = 0;
    }

void Point::translatePoint(double dist, char dir, bool positive){
        int multiplier = (positive)?1:-1;
        if ('X' == dir){
            x += multiplier*dist; 
        }
        else{
            y += multiplier*dist;
        }
    }

void Point::rotatePoint(double cx, double cy, double degrees, bool clockwise) {
    double radians = (PI / 180.0) * degrees;

    double s = std::sin(radians);
    double c = std::cos(radians);

    x -= cx;
    y -= cy;

    if (clockwise) {
        double new_x = x * c - y * s;
        double new_y = x * s + y * c;
        x = new_x;
        y = new_y;
    } else {
        double new_x = x * c + y * s;
        double new_y = -x * s + y * c;
        x = new_x;
        y = new_y;
    }

    x += cx;
    y += cy;
    }

void Point::drawPoint(){
    glPointSize(8);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    glVertex2d(x,y);
    glEnd();    
}

void drawTriangle(float centerX,float centerY, float triangleSize, float rotationAngle, float r, float g, float b) {
    // Calculate the vertices of the equilateral triangle based on the center and size
    float x1 = centerX - triangleSize / 2;
    float y1 = centerY - (triangleSize * 0.866);
    float x2 = centerX + triangleSize / 2;
    float y2 = y1;
    float x3 = centerX;
    float y3 = centerY + (triangleSize * 0.866);

    // Convert the rotation angle from degrees to radians
    float angleRad = rotationAngle * PI / 180.0f;

    // Apply the rotation transformation
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    float x1Rotated = centerX + (x1 - centerX) * cosA - (y1 - centerY) * sinA;
    float y1Rotated = centerY + (x1 - centerX) * sinA + (y1 - centerY) * cosA;

    float x2Rotated = centerX + (x2 - centerX) * cosA - (y2 - centerY) * sinA;
    float y2Rotated = centerY + (x2 - centerX) * sinA + (y2 - centerY) * cosA;

    float x3Rotated = centerX + (x3 - centerX) * cosA - (y3 - centerY) * sinA;
    float y3Rotated = centerY + (x3 - centerX) * sinA + (y3 - centerY) * cosA;

    // Set the drawing color (e.g., red)
    glColor3f(r, g, b);

    // Draw the rotated triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(x1Rotated, y1Rotated);
    glVertex2f(x2Rotated, y2Rotated);
    glVertex2f(x3Rotated, y3Rotated);
    glEnd();
}

// PRIMITIVES_H

// TRIANGULATE_H
bool isEar(const Point& a, const Point& b, const Point& c, const std::vector<Point>& polygon) {
    // Check if triangle ABC is an ear by verifying if no other point is inside it.
    for (const Point& p : polygon) {
        if ((p.x != a.x || p.y != a.y) && (p.x != b.x || p.y != b.y) && (p.x != c.x || p.y != c.y)) {
            double det = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
            if (det == 0) continue;

            double alpha = ((c.y - a.y) * (p.x - a.x) - (c.x - a.x) * (p.y - a.y)) / det;
            double beta = ((a.y - b.y) * (p.x - a.x) - (a.x - b.x) * (p.y - a.y)) / det;

            if (alpha >= 0 && beta >= 0 && (alpha + beta) <= 1)
                return false;
        }
    }
    return true;
}

std::vector<std::vector<Point>> triangulateConcavePolygon(const std::vector<Point>& polygon) {
    std::vector<std::vector<Point>> triangles;

    std::vector<Point> remainingPoints = polygon;

    while (remainingPoints.size() > 3) {
        int n = remainingPoints.size();
        for (int i = 0; i < n; i++) {
            int prevIdx = (i - 1 + n) % n;
            int currIdx = i;
            int nextIdx = (i + 1) % n;

            const Point& prev = remainingPoints[prevIdx];
            const Point& curr = remainingPoints[currIdx];
            const Point& next = remainingPoints[nextIdx];

            if (isEar(prev, curr, next, remainingPoints)) {
                triangles.push_back({prev, curr, next});
                remainingPoints.erase(remainingPoints.begin() + currIdx);
                break;
            }
        }
    }

    triangles.push_back(remainingPoints); // The remaining three points form the last triangle.

    return triangles;
}
// TRIANGULATE_H

// CANDLE_H
class Candle{
    private:
        float sx,sy;
        float mainBody[6][2],fireBody[12][2],wick[6][2];

    public:
        Candle(float,float);
        void fireAnimation();
        void fireStatic();
        void drawCandle();
        void moveCandle(int moveRange);
};


Candle::Candle(float startX, float startY)
{
    sx = startX;
    sy = startY;
    float mainBodyHelper[6][2] = {{6, 25}, {-6, 25}, {6, -25}, {-6, -25}, {6, -35}, {6, -45}},
    wickHelper[6][2] = {{1, - 37},{-2,-37 + 14/2},{1,-41},{-2,-41},{1,-45},{-2,-43}}, 
    fireHelper[12][2] = {{3,-40},{-3,-40},{5,-50},{-5,-51},{6,-52},{-3,-53},{5,-54},{-1,-55},{3,-56},{-1,-57},{0,-58},{-5,-61}};


    int i = 0;
    for (const auto& point : mainBodyHelper) {
    mainBody[i][0] = sx + point[0];
    mainBody[i][1] = sy + point[1];
    i++;
    }

    i = 0;
    for (const auto& point : wickHelper) {
    wick[i][0] = sx + point[0];
    wick[i][1] = sy + point[1];
    i++;
    }

    i = 0;
    for (const auto& point : fireHelper) {
    fireBody[i][0] = sx + point[0];
    fireBody[i][1] = sy + point[1];
    i++;
}
}

void Candle::moveCandle(int moveRange){
    int distX = (rand()%(2*moveRange + 1)) - moveRange;
    int distY = (rand()%(2*moveRange + 1)) - moveRange;

    sx += distX;sy += distY;

    for (int i = 0; i < 6; i++){
    mainBody[i][0] += distX;
    mainBody[i][1] += distY;
    wick[i][0] += distX;
    wick[i][1] += distY;
    }

    for (int i = 0; i < 12; i++){
    fireBody[i][0] += distX;
    fireBody[i][1] += distY;
    }
}

void Candle::fireAnimation(){
    glColor3f(1,0.5,0.0);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 12; i++){
        if(i > 3){
        fireBody[i][0] = fireBody[i][0] + ((rand()%5)/10.0 - 0.2);
        fireBody[i][1] = fireBody[i][1] + ((rand()%5)/10.0 - 0.2);
        }
        glVertex2f(fireBody[i][0],fireBody[i][1]);
        }
    glEnd();
    }

void Candle::fireStatic(){
            glColor3f(1,0.5,0.0);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 12; i++){
                    glVertex2f(fireBody[i][0],fireBody[i][1]);
                }
            glEnd();
        }

void Candle::drawCandle(){
            // Wick
            glColor3f(0.2,0.2,0.2);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 6; i++){
                glVertex2f(wick[i][0],wick[i][1]);
            }
            glEnd();

            // fireStatic();
            this->fireAnimation();

            // Main Body
            // glColor3f(0,0,0);
            glColor3f(0.9f, 0.9f, 0.9f);
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 6; i++){
                    glVertex2f(mainBody[i][0],mainBody[i][1]);
                }
            glEnd();
        }
// CANDLE_H


// PUMPKIN_H
class Pumpkin{
    public:
        float cx,cy;
        std::vector<std::vector<Point>> stemTris,mouthTris;
        std::vector<Point> nosePts;
        Point eye1,eye2;
        int eye1Size,eye2Size,eye1Angle,eye2Angle,eyeInd = -1,mouthInd = -1;
        float r,g,b;
        bool ghostPlay;

        Pumpkin(float centerX, float centerY);
        void drawStem();
        void drawBody();
        void drawEyesandNose(bool);
        void drawMouth(bool);
        void drawPumpkin(bool);
};

    
Pumpkin::Pumpkin(float centerX,float centerY){
    cx = centerX;
    cy = centerY;

    // Stem
    std::vector<std::vector<int>> stemPointsHelper = {
        {-40,0},{-50,-10},{-25,-15},{-25,-25},
        {0,-35},{5,-32},{10,-26},{8,-20},
        {4,-27},{0,-30},{-15,-25},{-15,-15},
        {10,-10},{0,0},{-15,-5},{-25,-5}};
    std::vector<Point> stemPoints;
    for (auto point : stemPointsHelper){
        stemPoints.push_back(Point(cx+point[0],cy+point[1]));
    }
    stemTris = triangulateConcavePolygon(stemPoints);

    // Mouth
    std::vector<std::vector<int>> mouthPointsHelper = {{-25,50},{-45,30},{-20,35},{5,30},{-15,50},{-20,47}};
    std::vector<Point> mouthPoints;
    for (auto point : mouthPointsHelper){
        mouthPoints.push_back(Point(cx+point[0],cy+point[1]));
    }
    mouthTris = triangulateConcavePolygon(mouthPoints);

    // Nose
    nosePts = {Point(cx - 30,cy + 25),Point(cx - 20,cy + 5),Point(cx - 20,cy + 20),Point(cx - 10,cy + 25)};

    // Eye
    eye1.x = cx - 40;eye1.y = cy+5;
    eye2.x = cx;eye2.y = cy+5;
    eye1Size = 12;eye2Size = 12;
    eye1Angle = 135;eye2Angle = 225;


}

void Pumpkin::drawStem(){
        glColor3f(0.0,0.5,0.0);
        glBegin(GL_TRIANGLES);
        for (const auto& triangle : stemTris) {
            for (const Point& point : triangle) {
                glVertex2i(point.x,point.y);
            }
        }
        glEnd();
        }



void Pumpkin::drawBody() {
    // Set the drawing color for the pumpkin body
    glColor3f(1.0, 0.5, 0.0);  // Orange color

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159265359 / 180.0;
        float x = cx - 20 +  55 * cos(angle);  // Adjust the radius as needed
        float y = cy + 28 + 40 * sin(angle);  // Adjust the radius as needed
        glVertex2f(x, y);
    }
    glEnd();
}

        
void Pumpkin::drawEyesandNose(bool ghostPlay){
    std::vector<std::vector<float>> eyeCols = {
        {0.0f, 0.0f, 0.0f},      // Black
        {0.2f, 0.1f, 0.0f},      // Dim Orange
        {0.9f, 0.5f, 0.0f},      // Light Orange
        {1.0f, 0.6f, 0.0f},      // Bright Orange
        {0.8f, 0.4f, 0.0f},      // Flicker
        {0.9f, 0.8f, 0.0f},      // Dim Yellow
        {1.0f, 0.9f, 0.0f},      // Light Yellow
        {1.0f, 1.0f, 0.0f},      // Bright Yellow
        {0.9f, 0.9f, 0.0f}       // Flicker
    };

    if (true == ghostPlay){
        ++eyeInd;
        if (eyeInd > 8){eyeInd = 0;}
        r = eyeCols[eyeInd][0];
        g = eyeCols[eyeInd][1];
        b = eyeCols[eyeInd][2];
        sleepTime(1);
    }
    else
    {
        r = 0.0f;g=0.0f;b=0.0f;
    }


    drawTriangle(eye1.x,eye1.y,eye1Size,eye1Angle,r,g,b);
    drawTriangle(eye2.x,eye2.y,eye2Size,eye2Angle,r,g,b);


    glColor3f(0.0,0.0,0.0);
    glBegin(GL_TRIANGLE_STRIP);
    for (Point point : nosePts){
        glVertex2f(point.x,point.y);
    }
    glEnd();

        }

void Pumpkin::drawMouth(bool ghostPlay){
    std::vector<std::vector<float>> mouthCols = {
    {0.0, 0.0, 0.0}, // Black
    {0.5, 0.0, 0.0}, // Dim Red
    {0.7, 0.0, 0.0}, // Dark Red
    {1.0, 0.0, 0.0}, // Bright Red
    {0.8, 0.0, 0.0}, // Flicker
    {0.9, 0.8, 0.0}, // Dim Yellow
    {1.0, 0.9, 0.0}, // Light Yellow
    {1.0, 1.0, 0.0}, // Bright Yellow
    {0.9, 0.9, 0.0}  // Flicker
    };


    if (true == ghostPlay){
        ++mouthInd;
        if (mouthInd > 8){mouthInd = 0;}
        r = mouthCols[mouthInd][0];
        g = mouthCols[mouthInd][1];
        b = mouthCols[mouthInd][2];
    }
    else
    {
        r = 0.0f;g=0.0f;b=0.0f;
    }

    glColor3f(r,g,b);
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : mouthTris) {
        for (const Point& point : triangle) {
            glVertex2i(point.x,point.y);
        }
    }
    glEnd();
    }

void Pumpkin::drawPumpkin(bool ghostPlay){
    drawStem();
    drawBody();
    drawMouth(ghostPlay);
    drawEyesandNose(ghostPlay);
}
// PUMPKIN_H


// HOUSE
void drawHouse(double houseCenterX, double houseCenterY) {
    const int windowWidth = 600;
    const int windowHeight = 600;
    const double houseSize = 200.0;  // Set the house size
    // Set house body color
    glColor3f(0.7, 0.7, 0.7);

    double houseLeft = houseCenterX - (houseSize / 2);
    double houseRight = houseCenterX + (houseSize / 2);
    double houseBottom = houseCenterY + (houseSize / 2);
    double houseTop = houseCenterY - (houseSize / 2);

    // Draw house body using a quad
    glBegin(GL_QUADS);
    glVertex2f(houseLeft, houseBottom);
    glVertex2f(houseRight, houseBottom);
    glVertex2f(houseRight, houseTop);
    glVertex2f(houseLeft, houseTop);
    glEnd();

    // Set roof color
    glColor3f(0.2, 0.1, 0.1);

    // Calculate the coordinates for the roof based on the center coordinates
    double roofBottom = houseTop;
    double roofLeft = houseCenterX - (houseSize / 2);
    double roofRight = houseCenterX + (houseSize / 2);
    double roofTop = houseTop - (houseSize / 5);

    // Draw the roof using a triangle strip
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(roofLeft, roofBottom);
    glVertex2f(roofRight, roofBottom);
    glVertex2f(houseCenterX, roofTop);
    glEnd();

    // Set door color
    glColor3f(0.5, 0.0, 0.0); 

    // Calculate the coordinates for the door based on the center coordinates
    double doorLeft = houseCenterX - (houseSize / 6);
    double doorRight = houseCenterX + (houseSize / 6);
    double doorBottom = houseBottom ;
    double doorTop = houseCenterY + (houseSize / 16);

    // Draw the door using a polygon
    glBegin(GL_POLYGON);
    glVertex2f(doorLeft,doorBottom);
    glVertex2f(doorLeft,doorTop);
    glVertex2f(doorRight,doorTop);
    glVertex2f(doorRight,doorBottom);
    glEnd();

}
// HOUSE


std::vector<Point> boltPoints = {{450, 100},{500, 50},{475, 50},{550, 0},{500, 50},{525, 50}};
std::vector<std::vector<Point>> boltCoords = triangulateConcavePolygon(boltPoints);

// THUNDER
void thunderDraw(int boltInd){
std::vector<std::vector<float>> thunderboltColors = {
    {1.0f, 0.8f, 0.5f},   // Initial Glow (Warm White)
    {0.0f, 0.5f, 1.0f},   // Electric Arc (Bright Blue)
    {1.0f, 1.0f, 1.0f},   // Flashes (White)
    {0.7f, 0.7f, 1.0f},   // Glow Around the Bolt (Light Blue)
    {0.2f, 0.2f, 0.2f}    // Fade Out (Dark Gray)
    };

    float redComponent = thunderboltColors[boltInd][0]; 
    float greenComponent = thunderboltColors[boltInd][1]; 
    float blueComponent = thunderboltColors[boltInd][2];  


    glColor3f(redComponent,greenComponent,blueComponent);
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : boltCoords) {
        for (const Point& point : triangle) {
            glVertex2i(point.x,point.y);
        }
    }
    glEnd();
}
// THUNDER

// MOON AND STARS
void drawMoonAndStars(int cx, int cy, int radius) {
    int numSegments = 400; // Number of triangle fan segments

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of the moon

    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);

        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        // Offset the x-coordinate to create the moon's shape
        if (i % 2 == 0) {
            x += radius / 4.0f;
        }

        glVertex2f(cx + x, cy + y);
    }
    glEnd();


    #include <vector>

    std::vector<std::vector<float>> starColors = {
        {1.0f, 1.0f, 1.0f},  // Bright White
        {0.8f, 0.8f, 0.8f},  // Light Gray
        {1.0f, 1.0f, 0.5f}   // Pale Yellow
    };

    std::vector<Point> stars = {Point(132,15),Point(287,42),Point(10,85),Point(398,31),Point(76,76)};

    float colInd = 0;
    glPointSize(10);
    glBegin(GL_POINTS);
    for (Point pt : stars){
        colInd = rand()%3;
        glColor3f(starColors[colInd][0],starColors[colInd][1],starColors[colInd][2]);
        glVertex2f(pt.x,pt.y);
    }
    glEnd();
}

// MOON AND STARS






int main(void){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    FsPassedTime();
    FsChangeToProgramDir();
    std::string ghostf = "ghost.wav",hauntedf = "haunted.wav",lightningf = "lightning.wav";
    int bgColInd = 0;
    int numCandles = 5;
    double bgTime = 0.0,timePassed,curTime = 0.0;
    std::vector<std::vector<float>> thundCols = {
    {1.0, 1.0, 1.0},  // White
    {0.8, 0.8, 0.8},  // Light Gray
    {0.9, 0.9, 0.9},  // Slightly Off-White
    {0.7, 0.7, 0.7},  // Medium Gray
    {1.0, 1.0, 1.0}   // White (or back to the first color for a flash effect)
    };


    // Define an array of 10 background colors in RGBA format
    std::vector<std::vector<float>> bgCols = {
        {0.3f, 0.0f, 0.4f, 1.0f}, 
        {0.35f, 0.05f, 0.45f, 1.0f},
        {0.4f, 0.1f, 0.5f, 1.0f},
        {0.45f, 0.15f, 0.55f, 1.0f},
        {0.5f, 0.2f, 0.6f, 1.0f},
        {0.55f, 0.25f, 0.65f, 1.0f},
        {0.6f, 0.3f, 0.7f, 1.0f},
        {0.65f, 0.35f, 0.75f, 1.0f},
        {0.7f, 0.4f, 0.8f, 1.0f},
        {0.3f, 0.0f, 0.4f, 1.0f},
    };

    std::vector<Candle> candles;
    
    for (int i = 0; i< numCandles; i++){
        candles.push_back(Candle(rand()%500 + 50,rand()%500 + 50));
    }
    Pumpkin pumpkin(300,400);
    int lightningTime = 10000;
    int ghostTime = 8000;
    bool ghostPlay = false;
    YsSoundPlayer player;
    YsSoundPlayer::SoundData lightning,ghost,haunted;
    player.MakeCurrent();
	player.Start();
    lightning.LoadWav(lightningf.c_str());
    ghost.LoadWav(ghostf.c_str());
    haunted.LoadWav(hauntedf.c_str());


    FsOpenWindow(100,100,600,600,1);
    player.PlayBackground(haunted);

    for (;;){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(bgCols[bgColInd][0], bgCols[bgColInd][1], bgCols[bgColInd][2], 1.0);
    
    drawHouse(150,200);
    for (Candle& curCandle : candles){
        curCandle.drawCandle();
        curCandle.moveCandle(rand()%20);
    }
    pumpkin.drawPumpkin(ghostPlay);
    drawMoonAndStars(200,40,20);

    FsSwapBuffers();
    FsPollDevice();
    auto key = FsInkey();
    if (FSKEY_ESC == key){
        break;
    }

    curTime = FsPassedTime();
    bgTime += curTime;
    timePassed += curTime;
    if (bgTime > 1000){
        bgTime = 0.01;
        ++bgColInd;
        if (bgColInd > 9){
            bgColInd = 0;
        }
    }


    if (((long)timePassed%ghostTime) < 100){
        player.PlayOneShot(ghost);
    }
    if (player.IsPlaying(ghost)){
        ghostPlay = true;
    }
    else{
        ghostPlay = false;
    }

    if (((long)timePassed%lightningTime) < 75){
        player.Stop(ghost);
        player.PlayOneShot(lightning);
        int thundInd = 0,boltInd = 0;
        while(player.IsPlaying(lightning)){
        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(thundCols[thundInd][0], thundCols[thundInd][1], thundCols[thundInd][2], 1.0);
        
        drawMoonAndStars(200,40,20);
        drawHouse(150,200);
        thunderDraw(boltInd);
        

        ++boltInd;
        if (boltInd > 4){
            boltInd = 0;
        }
        ++thundInd;
        if (thundInd > 4){
            thundInd = 0;
        }  
        FsSwapBuffers();
        FsSleep(100);
        }
    }

    std::cout<<"Time Passed: "<<timePassed<<std::endl;
    if (timePassed > 65000){
        break;
    }
    FsSleep(100);
    }
    player.End();
    return 0;
}




// g++ -std=c++11 demo.cpp fssimplewindowcpp.cpp fssimplewindowobjc.o yssimplesound.cpp yssimplesound_macosx_objc.o -framework Cocoa -framework OpenGL -framework AVFoundation -o test.app/Contents/MacOS/test


// ./test.app/Contents/MacOS/test 








