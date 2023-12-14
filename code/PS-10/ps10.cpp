/*
1. Get input
2. Check Input - Done
3. Get Score
    - Counting score
    - Frame with strikes
    - Last Frame
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
// #include <string>
#include <vector>
#include <stdlib.h>
#include <unordered_map>

class TextString
{
protected:
    char *str;

public:
    TextString();
    TextString(const char from[]);
    TextString(const TextString &str);
    ~TextString();
    void CleanUp();
    void Set(const char from[]);
    void Add(const char from[]);
    void Add(const char c);
    void BackSpace(void);
    void Fgets();
    void DeleteLastControlCode(void);
    const char *GetPointer(void) const;
    const int Strlen(void) const;
    void Print(void) const;
    const TextString &operator=(const TextString &from);
};

TextString::TextString() : str(NULL)
{
}

TextString::TextString(const char from[]) : str(NULL)
{
    Set(from);
}

TextString::TextString(const TextString &from) : str(NULL)
{
    Set(from.GetPointer());
}

TextString::~TextString()
{
    CleanUp();
}

void TextString::CleanUp()
{
    if(NULL!=str)
    {
        delete [] str;
        str=NULL;
    }
}

void TextString::Set(const char from[])
{
    if(NULL!=from)
    {
        CleanUp();

        const int n=strlen(from);
        str=new char [n+1];
        if(NULL!=str)
        {
            strcpy(str,from);
        }
    }
}

void TextString::Add(const char from[])
{
    if(NULL!=from)
    {
        const int nCurrent=Strlen();
        const int nFrom=strlen(from);

        char *newStr=new char [nCurrent+nFrom+1];
        if(NULL!=newStr)
        {
            strcpy(newStr,GetPointer());
            strcpy(newStr+nCurrent,from);
            if(NULL!=str)
            {
                delete [] str;
            }
            str=newStr;
        }
    }
}

void TextString::Add(const char c)
{
    const int nCurrent=Strlen();

    char *newStr=new char [nCurrent+2];
    if(NULL!=newStr)
    {
        strcpy(newStr,GetPointer());
        newStr[nCurrent]=c;
        newStr[nCurrent+1]=0;
        if(NULL!=str)
        {
            delete [] str;
        }
        str=newStr;
    }
}

void TextString::BackSpace(void)
{
    if(NULL!=str)
    {
        int n=Strlen();
        if(0<n)
        {
            str[n-1]=0;
        }
    }
}

void TextString::DeleteLastControlCode(void)
{
    int n;
    for(n=strlen(str)-1; 0<=n; n--)
    {
        if(0!=isprint(str[n]))
        {
            break;
        }
        str[n]=0;
    }
}


void TextString::Fgets()
{       
 CleanUp();

        const int bufferSize = 255;
        char inputBuffer[bufferSize];
        std::cin.getline(inputBuffer, bufferSize);

        // Find the position of the first null character in the input buffer
        size_t inputLength = strnlen(inputBuffer, bufferSize);

        // Allocate memory for str and copy the input
        str = new char[inputLength + 1];
        strncpy(str, inputBuffer, inputLength);
        str[inputLength] = '\0'; // Null-terminate the string
}

// const char *TextString::Fgets(FILE *fp)
// {
//     if(NULL!=fp)
//     {
//         CleanUp();

//         int firstLine=1;
//         const int nBuf=8;
//         char buf[nBuf+1];

//         buf[nBuf]=0;
//         for(;;)
//         {
//             if(NULL==fgets(buf,nBuf,fp))
//             {
//                 if(0!=firstLine)
//                 {
//                     return NULL;
//                 }
//                 else
//                 {
//                     break;
//                 }
//             }
//             else
//             {
//                 firstLine=0;
//                 Add(buf);

//                 int nChar=strlen(buf);
//                 if(buf[nChar-1]=='\n' || buf[nChar-1]=='\a')
//                 {
//                     break;
//                 }
//             }
//         }

//         DeleteLastControlCode();
//         return str;
//     }
//     return NULL;
// }

const char *TextString::GetPointer(void) const
{
    if(NULL!=str)
    {
        return str;
    }
    else
    {
        return "";
    }
}

const int TextString::Strlen(void) const
{
    return strlen(GetPointer());
}

void TextString::Print(void) const
{
    if(NULL!=str)
    {
        printf("%s\n",str);
    }
}

const TextString &TextString::operator=(const TextString &from)
{
    Set(from.GetPointer());
    return *this;
}



// ### Main Function


void sleepTime(int timeSleep){
    auto t0=time(NULL);
    while(time(NULL)-t0<timeSleep)
    {
    }
}


const std::vector<char> charList = {'0','1','2','3','4','5','6','7','8','9','X','x','-',' ','/'};
bool validChar(char temp){
    for (char x: charList){
        if(temp == x){
            return true;
        }
    }
    return false;
}


const std::unordered_map<char, int> scoresMap{
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
    {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
    {'X', 10}, {'x', 10}, {'-', 0}, {' ', 0}};
int getScore(char ip){
    return scoresMap.at(ip);
}


int main(void){
    int scores[21];
    int totalScore = 0,curScore = 0;
    // frames*2 + 0/1 (10 frames)
    TextString curStr;
    int ipStrLen;
    // std::vector<std::string> ipStrVec;
    // ipStrVec.push_back("X X X X X X X X X XXX");
    // ipStrVec.push_back("9/72X-X-8-919/9/9-XX6");
    // ipStrVec.push_back("9/9/9/9/9/9/9/9/9/9/9");
    // ipStrVec.push_back("9-9-9-9-9-9-9-9-9-9--");
    // ipStrVec.push_back("9/90X X 9/X X X 9/XX8");


    curStr.Fgets();
    srand(time(NULL));
    ipStrLen = curStr.Strlen();


    if(ipStrLen < 21){
            std::cout<<"Too Short"<<std::endl;
    }
    else{

    if (ipStrLen > 21){
        while(curStr.Strlen() > 21){
            // std::cout<<"Cur String:";
            curStr.Print();
            // std::cout<< " with Length: "<< curStr.Strlen()<<std::endl;
            curStr.BackSpace();
        }
    }
    // std::cout<<"Final String: ";
    // curStr.Print();
    // std::cout<<" final length: "<< curStr.Strlen()<<std::endl;
    // sleepTime(2);

    auto strPtr = curStr.GetPointer();
    for(int i = 0; i<21;i++){
        char curChar = strPtr[i];
        if (true != validChar(curChar)){
            std::cout<<"Wrong Character"<<std::endl;
            break;
        }
    }

    for(int frame = 0; frame < 10; frame++){
        curScore = 0;
        if(9 == frame){
            char fEle = strPtr[frame*2 + 0], sEle = strPtr[frame*2 + 1], tEle = strPtr[frame*2 + 2];
            if('/' == sEle){
                curScore += 10 + getScore(tEle);
            }
            else{
                curScore += getScore(fEle) + getScore(sEle);
                if(curScore > 9){
                    curScore += getScore(tEle);
                }
            }
        }
        else{
            char fEle = strPtr[frame*2 + 0],sEle = strPtr[frame*2 + 1];

            if (('X' == fEle) | ('x' == fEle)){
                if (('X' == strPtr[frame*2+2]) | ('x' == strPtr[frame*2+2])){
                    curScore = 20 + getScore(strPtr[frame*2+4]);
                }
                else{
                    if('/' == strPtr[frame*2+3]){
                        curScore = 20;
                    }
                    else{
                        curScore = 10 + getScore(strPtr[frame*2+2]) + getScore(strPtr[frame*2+3]);
                    }
                }
            }
            else{
                if ('/' == sEle){
                    curScore = 10;
                }
                else{
                    curScore = getScore(fEle) + getScore(sEle);
                }
                if(10 == curScore){
                    curScore += getScore(strPtr[frame*2 + 2]);
                }
            }
        }
        totalScore += curScore;
    }

    std::cout<<totalScore<<std::endl;
    sleepTime(1);
    }
}
