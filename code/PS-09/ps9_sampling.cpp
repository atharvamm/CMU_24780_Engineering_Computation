#include <string>
#include <fstream>
#include <iostream>
#include "fssimplewindow.h"
#include <cstdio>
#include <cstdlib>


// CharBitmap class must be initialized to an empty bitmap on construction.
// Also const qualifiers must be correctly used.
// If you look careful, there are hints for where to use const qualifiers.
class CharBitmap
{
protected:
    int wid,hei;
    char *dat;
public:
    CharBitmap();
    ~CharBitmap();
    void CleanUp(void);

	bool Load(std::string fileName);

	CharBitmap( CharBitmap &incoming);
	CharBitmap &operator=( CharBitmap &incoming);
	void CopyFrom( CharBitmap &incoming);

    void Create(int wid,int hei);

    int GetWidth(void) ;
    int GetHeight(void) ;

    void Print(void) ;
    void Draw(void) ;
    char GetPixel(int x,int y) ;
};

// Default constructor.
CharBitmap::CharBitmap()
{
    dat = NULL;
}
// Destructor must clean up dynamically-allocated memory to make sure there will be no memory leak.
CharBitmap::~CharBitmap()
{
    CleanUp();
}
// CleanUp function must clean up the data structure.
// Return dynamcially-allocated memory if any and then nullify the pointer and dimension.
void CharBitmap::CleanUp(void)
{
    if (dat != NULL){
    delete [] dat;
    dat = NULL;
    }
}

// Copy constructor must correctly copy the incoming bitmap.
// There must not be memory leak after copying.
// If you have a correct CopyFrom function, you can just call CopyFrom function.
CharBitmap::CharBitmap( CharBitmap &incoming)
{
    // std::cout<<"Called Copy Constructor"<<std::endl;
    dat = NULL;
    CopyFrom(incoming);
}

// Copy-assignment operator must correctly copy the incoming bitmap.
// There must not be memory leak after copying.
// If you have a correct CopyFrom function, you can call the function, and then
// return reference to this object.
CharBitmap &CharBitmap::operator=( CharBitmap &incoming)
{   
    dat = NULL;
    CopyFrom(incoming);
    return *this;
}

// CopyFrom function must correctly copy incoming CharBitmap to this CharBitmap.
// There must not be memory leak during the process.
void CharBitmap::CopyFrom( CharBitmap &incoming)
{   
    // std::cout<<"Called CopyFrom"<<std::endl;
    if(this!=&incoming)
    {   
        this->wid = incoming.GetWidth();
        this->hei = incoming.GetHeight();
        int len = this->wid*this->hei;
        this->CleanUp();
        this->dat = new char[len+1];
        for (int y = 0; y < hei; y++){
        for (int x = 0; x < wid;x++){
            this->dat[y*wid + x] = incoming.GetPixel(x,y);
            }
        }
    }
}

// Load function reads a CharBitmap from a text-based data file.
// The first and the second lines are width and height respectively.
// Bitmap patten will then follow.
// The sample data for this assignment (pattern.txt) looks like:

// 16
// 16
// ....11....11....
// ...1..1..1..1...
// ...1.11..1.11...
// ...1.11..1.11...
// ...1.11..1.11...
// ...1.11..1.11...
// ...1.11..1.11...
// ...1.11111.11...
// ..1..........1..
// .1............1.
// 1....1....1....1
// 1..............1
// 1..............1
// 1......11......1
// .11..........11.
// ...1111111111...

// Also there must be no memory leak.
bool CharBitmap::Load(std::string fileName)
{   
    CleanUp();
    FILE *fp;
    char str[256];

    fp = fopen(fileName.c_str(),"r");
    if((fp!=NULL) & (fgets(str,255,fp)!=NULL)){

    int nRead;
    wid = atoi(str);
    hei = atoi(fgets(str,255,fp));
    dat = new char[wid*hei + 1];

    if (dat != NULL){
        nRead = 0;
        while((fgets(str,255,fp)!=NULL) & (nRead < hei)){
            for (int i = nRead*wid; i < (nRead+1)*wid;i++){
                dat[i] = str[i%16];
            }
            nRead++;
        }
    fclose(fp);
        }
    }
    return true;
}

// Create function creates a bitmap with wid X hei dimension.
// There must not be memory leak.
// If wid or hei is zero or negative, make bitmap empty.
void CharBitmap::Create(int wid,int hei)
{      
    // Create a copy of the dat array
    int ogHei = this->GetHeight(),ogWid = this->GetWidth(); 
    char *temp = NULL;
    temp = new char[ogHei * ogWid + 1];
    for(int y = 0; y < ogHei ; y++ ){
        for(int x = 0; x < ogWid; x++){
            temp[y*ogWid + x] = this->GetPixel(x,y);
        }
    }

    // Create updated dat array
    CleanUp();
    this->dat = new char[wid*hei + 1];
    this->wid = wid;
    this->hei = hei;
    if ((wid > 0) & (hei > 0)){
        int len = wid*hei + 1;
        for(int y = 0; y < hei; y++){
            for(int x = 0; x < wid; x++){
                
                int old_index = (y*ogHei/hei)*ogWid + (x*ogWid/wid);
                int new_index = y*wid + x;
                this->dat[new_index] = temp[old_index];
            }
        }
    }

    // Clear memory used by temp
    if (temp != NULL){
        delete [] temp;
    }
    temp = NULL;
}

// This function returns the width of the bitmap.
int CharBitmap::GetWidth(void) 
{
    return wid;
}
// This function returns the height of the bitmap.
int CharBitmap::GetHeight(void) 
{
    return hei;
}

// Print bitmap on the console (terminal).
void CharBitmap::Print(void) 
{
    for (int lineNum = 0; lineNum < hei; lineNum++){
        for (int i = lineNum*wid; i < (lineNum+1)*wid;i++){
        std::cout<<dat[i];
        }
        std::cout<<""<<std::endl;
    }
}

// Draw bitmap on the graphics window.
// One pixel of this bitmap must be expanded to 16x16-pixel square on the graphics window.
void CharBitmap::Draw(void) 
{   
    // std::cout<<"Called Draw"<<std::endl;
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int x = 0; x< 256; x ++){
        for (int y = 0; y < 256; y ++){
            if(true == ('1' == dat[(y/this->hei)*this->wid+(x/this->wid)])){
                glColor3f(0.0,0.0,0.0);}
            else{
                glColor3f(1.0,1.0,1.0);
            }
            glVertex2i(x,y);
        }
    }
    glEnd();
}

// Returns the pixel value at (x,y).  If (x,y) is outside of the pixel, return zero.
char CharBitmap::GetPixel(int x,int y) 
{
    if ((x < wid) & (y < hei)){
        return dat[y*wid+x];
    }
    else{
        return 0;
    }
}

// Do not touch RealMain and main.  Leave these as is.
void RealMain(void)
{
    FsOpenWindow(16,16,256,256,1);
    FsChangeToProgramDir();

    CharBitmap bmp0;
    if(true!=bmp0.Load("pattern.txt"))
    {
		std::cout << "Load error." << std::endl;
		return;
	}
	std::cout << "Dimension: " << bmp0.GetWidth() << " X " << bmp0.GetHeight() << std::endl;
    bmp0.Print();

	CharBitmap bmp1(bmp0); // For testing the copy constructor.

	CharBitmap bmp;
	bmp=bmp1; // For testing the copy-assignment operator

    bmp.Create(20,20);
    bmp.Print();

    while(FSKEY_NULL==FsInkey())
    {
        FsPollDevice();

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        bmp.Draw();
        FsSwapBuffers();

        FsSleep(10);
    }
}

// At the end of program, Closing. should be printed on the console window.
// i.e., RealMain must not crash.
int main(void)
{
	RealMain();
	// Returned from RealMain means the destructor did not crash.
	std::cout << "Closing." << std::endl;
    return 0;
}

