#include "platformapp.h"
#include "scannerstruct.h"
#include "ImgFile/ImgFile.h"
#include <string.h>
using namespace JK;

PlatformApp::PlatformApp()
{
}


//dup:0 a side ,1 b side
void PlatformApp::saveImage(char* filaname ,unsigned char* buffer
                            ,int width ,int height ,int bitPerPixel)
{
    filaname=filaname;
    buffer = buffer;
    width = width;
    height = height;
    bitPerPixel = bitPerPixel;
}

void PlatformApp::updateProgress(float progrress)
{
    progrress = progrress;
}
