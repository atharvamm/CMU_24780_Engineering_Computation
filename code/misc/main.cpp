#include "fssimplewindow.h"

int main(void)
{
    FsOpenWindow(0,0,800,600,1);
    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
    }
    return 0;
}