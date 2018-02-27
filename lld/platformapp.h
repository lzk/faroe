#ifndef PLATFROMAPP_H
#define PLATFROMAPP_H
#include "scanner.h"
namespace JK {

class PlatformApp
{
public:
    PlatformApp(){}
    virtual void updateProgress(int progress ,int page) = 0;
    virtual const char* getTempFilename(int side) = 0;
    virtual bool saveScanImage(Scanner::Setting* setting ,Scanner::Para_Extra* para) = 0;
private:
};

}
#endif // PLATFROMAPP_H
