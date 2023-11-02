#include <iostream>
#include "fssimplewindow.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>

const int scrWidth = 800, scrHeight = 600;

class Line{
    public:
        int x1,y1,x2,y2;
        uint8_t r,g,b;
    
};


void ParseString(int &nWord, int wordTop[], int wordLen[],char incoming[],int wordLimit){
	int state=0;
	for(int i=0; 0!=incoming[i]; ++i)
	{
		auto c=incoming[i];
		if(0==state)
		{
			if(' '!=c && '\n'!=c && '\t'!=c && '\r'!=c)
			{
				state=1;
				wordTop[nWord]=i;
				wordLen[nWord]=1;
				++nWord;
			}
		}
		else
		{
			if(' '!=c && '\n'!=c && '\t'!=c && '\r'!=c)
			{
				++wordLen[nWord-1];
			}
			else
			{
				if(nWord==wordLimit)
				{
					break;
				}
				state=0;
			}
		}
	}
}

class Drawing{
    private:
        int nLines;
        Line *lines;
        Drawing(const Drawing& other);
        Drawing& operator=(const Drawing& other);


    public:
        Drawing(){
            lines = NULL;
        }
        ~Drawing(){
            Close();
        }

        void Close(){
            if (lines != NULL){
                delete [] lines;
                lines = NULL;
            }
        }

        void Open(std::string fName){
            Close();
            FILE *fp;
            char str[256];

            fp = fopen(fName.c_str(),"r");
            if(fp!=NULL && fgets(str,255,fp)!=NULL){
                int nRead;
                nLines = atoi(str);
                lines = new Line[nLines];
                if (lines != NULL){
                    nRead = 0;
                    while(fgets(str,255,fp)!=NULL && nRead < nLines){
                        int nWord = 0,wordTop[16],wordLength[16];
                        ParseString(nWord,wordTop,wordLength,str,16);
                        if (nWord>=2)
                        {
                            lines[nRead].x1 = atoi(str+wordTop[0]);
                            lines[nRead].y1 = atoi(str+wordTop[1]);
                            lines[nRead].x2 = atoi(str+wordTop[2]);
                            lines[nRead].y2 = atoi(str+wordTop[3]);
                            lines[nRead].r = atoi(str+wordTop[4]);
                            lines[nRead].g = atoi(str+wordTop[5]);
                            lines[nRead].b = atoi(str+wordTop[6]);
                            nRead++;
                        }
                    }
                fclose(fp);
                }
            }
        }

        void Draw(int offsetX,int offsetY){
            int i;
            glBegin(GL_LINES);
            for (i = 0 ; i < nLines - 1; i++){
                glColor3ub(lines[i].r,lines[i].g,lines[i].b);
                glVertex2i(lines[i].x1 + scrWidth/2 + offsetX,lines[i].y1 + scrHeight/2 + offsetY);
                glVertex2i(lines[i].x2 + scrWidth/2 + offsetX,lines[i].y2 + scrHeight/2 + offsetY);
            }
            glEnd();
            glFlush();
        }
};

int main(void){
    FsChangeToProgramDir();
    FsOpenWindow(100,100,scrWidth,scrHeight,0);
    Drawing drawObj;
    std::string fName = "concorde.txt";
    drawObj.Open(fName);
    int offsetX = 0,offsetY = 0;
    bool fOpen = false;

    for(;;){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        auto key = FsInkey();
        switch (key)
        {
        case FSKEY_R:
            if (fName == "redarrow.txt"){break;}
            drawObj.Open("redarrow.txt");
            offsetX = 0;offsetY = 0;
            fName = "redarrow.txt";
            break;
        case FSKEY_C:
            if (fName == "concorde.txt"){break;}
            drawObj.Open("concorde.txt");
            offsetX = 0;offsetY = 0;
            fName = "concorde.txt";
            break;
        case FSKEY_UP:
            offsetY -= 10;
            break;
        case FSKEY_DOWN:
            offsetY += 10;
            break;
        case FSKEY_LEFT:
            offsetX -= 10;
            break;
        case FSKEY_RIGHT:
            offsetX += 10;
            break;
        default:
            break;
        }

        drawObj.Draw(offsetX,offsetY);

        glFlush();
        FsSleep(10);
        if (FSKEY_ESC == key){
            break;
        }
    }
    return 0;
}
