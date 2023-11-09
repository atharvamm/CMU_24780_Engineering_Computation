#include <string>
#include <fstream>
#include <iostream>
#include "fssimplewindow.h"

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
}
// Destructor must clean up dynamically-allocated memory to make sure there will be no memory leak.
CharBitmap::~CharBitmap()
{
}
// CleanUp function must clean up the data structure.
// Return dynamcially-allocated memory if any and then nullify the pointer and dimension.
void CharBitmap::CleanUp(void)
{
}

// Copy constructor must correctly copy the incoming bitmap.
// There must not be memory leak after copying.
// If you have a correct CopyFrom function, you can just call CopyFrom function.
CharBitmap::CharBitmap( CharBitmap &incoming)
{
}

// Copy-assignment operator must correctly copy the incoming bitmap.
// There must not be memory leak after copying.
// If you have a correct CopyFrom function, you can call the function, and then
// return reference to this object.
CharBitmap &CharBitmap::operator=( CharBitmap &incoming)
{
}

// CopyFrom function must correctly copy incoming CharBitmap to this CharBitmap.
// There must not be memory leak during the process.
void CharBitmap::CopyFrom( CharBitmap &incoming)
{
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
}

// Create function creates a bitmap with wid X hei dimension.
// There must not be memory leak.
// If wid or hei is zero or negative, make bitmap empty.
void CharBitmap::Create(int wid,int hei)
{
}

// This function returns the width of the bitmap.
int CharBitmap::GetWidth(void) 
{
}
// This function returns the height of the bitmap.
int CharBitmap::GetHeight(void) 
{
}

// Print bitmap on the console (terminal).
void CharBitmap::Print(void) 
{
}

// Draw bitmap on the graphics window.
// One pixel of this bitmap must be expanded to 16x16-pixel square on the graphics window.
void CharBitmap::Draw(void) 
{
}

// Returns the pixel value at (x,y).  If (x,y) is outside of the pixel, return zero.
char CharBitmap::GetPixel(int x,int y) 
{
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

